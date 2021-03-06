/*
 * This work was authored by Two Six Labs, LLC and is sponsored by a subcontract
 * agreement with Galois, Inc.  This material is based upon work supported by
 * the Defense Advanced Research Projects Agency (DARPA) under Contract No.
 * HR0011-19-C-0103.
 *
 * The Government has unlimited rights to use, modify, reproduce, release,
 * perform, display, or disclose computer software or computer software
 * documentation marked with this legend. Any reproduction of technical data,
 * computer software, or portions thereof marked with this legend must also
 * reproduce this marking.
 *
 * Copyright 2019-2020 Two Six Labs, LLC.  All rights reserved.
 */

#include <errno.h>
#include <linux/limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#if HAVE_STD_ATOMIC
#include <stdatomic.h>
typedef atomic_int pirate_atomic_int;
#define ATOMIC_DEC(PTR) atomic_fetch_sub(PTR, 1)
#else
typedef int pirate_atomic_int;
#define ATOMIC_DEC(PTR) __atomic_fetch_sub(PTR, 1, __ATOMIC_SEQ_CST)
#endif

#include "libpirate.h"
#include "libpirate_internal.h"
#include "device.h"
#include "pipe.h"
#include "unix_socket.h"
#include "unix_seqpacket.h"
#include "tcp_socket.h"
#include "udp_socket.h"
#include "shmem_interface.h"
#include "udp_shmem_interface.h"
#include "uio_interface.h"
#include "serial.h"
#include "mercury.h"
#include "ge_eth.h"
#include "pirate_common.h"
#include "channel_funcs.h"

typedef union {
    common_ctx         common;
    device_ctx         device;
    pipe_ctx           pipe;
    unix_socket_ctx    unix_socket;
    unix_seqpacket_ctx unix_seqpacket;
    tcp_socket_ctx     tcp_socket;
    udp_socket_ctx     udp_socket;
    shmem_ctx          shmem;
    udp_shmem_ctx      udp_shmem;
    uio_ctx            uio;
    serial_ctx         serial;
    mercury_ctx        mercury;
    ge_eth_ctx         ge_eth;
} pirate_channel_ctx_t;

typedef struct {
    pirate_channel_param_t param;
    pirate_channel_ctx_t ctx;
} pirate_channel_t;

static pirate_channel_t gaps_channels[PIRATE_NUM_CHANNELS];
static pirate_stats_t gaps_stats[PIRATE_NUM_CHANNELS];

static pirate_channel_t gaps_nofd_channels[PIRATE_NUM_CHANNELS];
static pirate_stats_t gaps_nofd_stats[PIRATE_NUM_CHANNELS];

#define PIRATE_NOFD_CHANNELS_LIMIT (-PIRATE_NUM_CHANNELS - 2)

static const pirate_channel_funcs_t gaps_channel_funcs[PIRATE_CHANNEL_TYPE_COUNT] = {
    {NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    PIRATE_DEVICE_CHANNEL_FUNCS,
    PIRATE_PIPE_CHANNEL_FUNCS,
    PIRATE_UNIX_SOCKET_CHANNEL_FUNCS,
    PIRATE_UNIX_SEQPACKET_CHANNEL_FUNCS,
    PIRATE_TCP_SOCKET_CHANNEL_FUNCS,
    PIRATE_UDP_SOCKET_CHANNEL_FUNCS,
    PIRATE_SHMEM_CHANNEL_FUNCS,
    PIRATE_UDP_SHMEM_CHANNEL_FUNCS,
    PIRATE_UIO_CHANNEL_FUNCS,
    PIRATE_SERIAL_CHANNEL_FUNCS,
    PIRATE_MERCURY_CHANNEL_FUNCS,
    PIRATE_GE_ETH_CHANNEL_FUNCS
};

int pirate_close_channel(pirate_channel_t *channel);

static inline pirate_channel_t *pirate_get_channel(int gd) {
    pirate_channel_t *channel;

    if ((gd == -1) || (gd >= PIRATE_NUM_CHANNELS) || (gd <= PIRATE_NOFD_CHANNELS_LIMIT)) {
        errno = EBADF;
        return NULL;
    }

    if (gd >= 0) {
        channel = &gaps_channels[gd];
    } else {
        channel = &gaps_nofd_channels[-gd - 2];
    }
    if (channel->param.channel_type == INVALID) {
        errno = EBADF;
        return NULL;
    }

    return channel;
}

static inline int pirate_channel_type_valid(channel_enum_t t) {
    if ((t <= INVALID) || (t >= PIRATE_CHANNEL_TYPE_COUNT)) {
        errno = EINVAL;
        return -1;
    }
    return 0;
}

void pirate_init_channel_param(channel_enum_t channel_type, pirate_channel_param_t *param) {
    memset(param, 0, sizeof(*param));
    param->channel_type = channel_type;
}

static const char* pirate_common_keys[] = {"drop", NULL};

int pirate_parse_is_common_key(const char *key) {
    for (int i = 0; pirate_common_keys[i] != NULL; i++) {
        if (strncmp(pirate_common_keys[i], key, strlen(pirate_common_keys[i])) == 0) {
            return 1;
        }
    }
    return 0;
}

static int pirate_parse_common_kv(const char *key, const char *val, pirate_channel_param_t *param) {
    if (strncmp("drop", key, strlen("drop")) == 0) {
        param->drop = atoi(val);
    }
    return 0;
}

#define TWO_OPT_DELIM OPT_DELIM OPT_DELIM

static int pirate_parse_common_param(char *str, pirate_channel_param_t *param) {
    char *token, *key, *val;
    char *saveptr1, *saveptr2;
    const char *needle = TWO_OPT_DELIM;
    int rv;

    // strtok cannot handle empty tokens
    if (strstr(str, needle) != NULL) {
        errno = EINVAL;
        return -1;
    }

    while ((token = strtok_r(str, OPT_DELIM, &saveptr1)) != NULL) {
        str = NULL;
        key = strtok_r(token, KV_DELIM, &saveptr2);
        if (key == NULL) {
            continue;
        }
        val = strtok_r(NULL, KV_DELIM, &saveptr2);
        if (val == NULL) {
            continue;
        }
        if (!pirate_parse_is_common_key(key)) {
            continue;
        }
        rv = pirate_parse_common_kv(key, val, param);
        if (rv < 0) {
            return rv;
        }
    }
    return 0;
}

int pirate_parse_channel_param(const char *str, pirate_channel_param_t *param) {
    // Channel configuration function is allowed to modify the string
    // while braking it into delimiter-separated tokens
    char opt[256];
    int rv;
    strncpy(opt, str, sizeof(opt) - 1);
    pirate_parse_param_t parse_func;

    pirate_init_channel_param(INVALID, param);

    rv = pirate_parse_common_param(opt, param);
    if (rv < 0) {
        return rv;
    }

    strncpy(opt, str, sizeof(opt) - 1);

    if (strncmp("device", opt, strlen("device")) == 0) {
        param->channel_type = DEVICE;
    } else if (strncmp("pipe", opt, strlen("pipe")) == 0) {
        param->channel_type = PIPE;
    } else if (strncmp("unix_socket", opt, strlen("unix_socket")) == 0) {
        param->channel_type = UNIX_SOCKET;
    } else if (strncmp("unix_seqpacket", opt, strlen("unix_seqpacket")) == 0) {
        param->channel_type = UNIX_SEQPACKET;
    } else if (strncmp("tcp_socket", opt, strlen("tcp_socket")) == 0) {
        param->channel_type = TCP_SOCKET;
    } else if (strncmp("udp_socket", opt, strlen("udp_socket")) == 0) {
        param->channel_type = UDP_SOCKET;
    } else if (strncmp("shmem", opt, strlen("shmem")) == 0) {
        param->channel_type = SHMEM;
    } else if (strncmp("udp_shmem", opt, strlen("udp_shmem")) == 0) {
        param->channel_type = UDP_SHMEM;
    } else if (strncmp("uio", opt, strlen("uio")) == 0) {
        param->channel_type = UIO_DEVICE;
    } else if (strncmp("serial", opt, strlen("serial")) == 0) {
        param->channel_type = SERIAL;
    } else if (strncmp("mercury", opt, strlen("mercury")) == 0) {
        param->channel_type = MERCURY;
    } else if (strncmp("ge_eth", opt, strlen("ge_eth")) == 0) {
        param->channel_type = GE_ETH;
    }

    if (pirate_channel_type_valid(param->channel_type) != 0) {
        return -1;
    }

    parse_func = gaps_channel_funcs[param->channel_type].parse_param;

    if (parse_func == NULL) {
        errno = ESOCKTNOSUPPORT;
        return -1;
    }

    return parse_func(opt, &param->channel);
}

int pirate_get_channel_param(int gd, pirate_channel_param_t *param) {
    pirate_channel_t *channel = NULL;

    if ((channel = pirate_get_channel(gd)) == NULL) {
        return -1;
    }
    memcpy(param, &channel->param, sizeof(pirate_channel_param_t));
    return 0;
}

pirate_stats_t *pirate_get_stats_internal(int gd) {
    if (gd >= 0) {
        return &gaps_stats[gd];
    } else {
        return &gaps_nofd_stats[-gd - 2];
    }
}

const pirate_stats_t *pirate_get_stats(int gd) {
    if ((gd == -1) || (gd >= PIRATE_NUM_CHANNELS) || (gd <= PIRATE_NOFD_CHANNELS_LIMIT)) {
        errno = EBADF;
        return NULL;
    }
    return pirate_get_stats_internal(gd);
}

int pirate_unparse_channel_param(const pirate_channel_param_t *param, char *desc, int len) {
    pirate_get_channel_description_t unparse_func;
    if (pirate_channel_type_valid(param->channel_type) != 0) {
        return -1;
    }

    unparse_func = gaps_channel_funcs[param->channel_type].get_channel_description;

    if (unparse_func == NULL) {
        errno = ESOCKTNOSUPPORT;
        return -1;
    }

    return unparse_func(&param->channel, desc, len);
}

int pirate_get_channel_description(int gd, char *desc, int len) {
    pirate_channel_t *channel = NULL;

    if ((channel = pirate_get_channel(gd)) == NULL) {
        return -1;
    }

    return pirate_unparse_channel_param(&channel->param, desc, len);
}

static pirate_atomic_int next_gd = -2;

int pirate_next_gd() {
    int next = ATOMIC_DEC(&next_gd);
    return next;
}

// Declared in libpirate_internal.h for testing purposes only
void pirate_reset_stats() {
    memset(gaps_stats, 0, sizeof(gaps_stats));
    memset(gaps_nofd_stats, 0, sizeof(gaps_nofd_stats));
}

static int pirate_open(pirate_channel_t *channel) {
    pirate_channel_param_t *param = &channel->param;
    pirate_channel_ctx_t *ctx = &channel->ctx;
    int access = channel->ctx.common.flags & O_ACCMODE;
    int nonblock = channel->ctx.common.flags & O_NONBLOCK;
    ssize_t mtu;
    pirate_open_t open_func;

    if ((access != O_RDONLY) && (access != O_WRONLY)) {
        errno = EINVAL;
        return -1;
    }

    mtu = pirate_write_mtu_estimate(param);
    if (mtu < 0) {
        return -1;
    }

    if (pirate_channel_type_valid(param->channel_type) != 0) {
        return -1;
    }

    if (nonblock && !pirate_nonblock_channel_type(param->channel_type, (size_t) mtu)) {
        errno = EINVAL;
        return -1;
    }

    open_func = gaps_channel_funcs[param->channel_type].open;
    if (open_func == NULL) {
        errno = ESOCKTNOSUPPORT;
        return -1;
    }

    return open_func(&param->channel, ctx);
}

// gaps descriptors must be opened from smallest to largest
int pirate_open_param(pirate_channel_param_t *param, int flags) {
    pirate_channel_t channel;
    int gd;

    memcpy(&channel.param, param, sizeof(pirate_channel_param_t));
    channel.ctx.common.flags = flags;

    gd = pirate_open(&channel);
    if ((gd >= PIRATE_NUM_CHANNELS) || (gd <= PIRATE_NOFD_CHANNELS_LIMIT)) {
        pirate_close_channel(&channel);
        errno = EMFILE;
        return -1;
    }
    if (gd == -1) {
        return -1;
    }

    if (gd >= 0) {
        memcpy(&gaps_channels[gd], &channel, sizeof(pirate_channel_t));
    } else {
        memcpy(&gaps_nofd_channels[-gd - 2], &channel, sizeof(pirate_channel_t));
    }
    return gd;
}

int pirate_open_parse(const char *param, int flags) {
    pirate_channel_param_t vals;

    if (pirate_parse_channel_param(param, &vals) < 0) {
        return -1;
    }

    return pirate_open_param(&vals, flags);
}

int pirate_nonblock_channel_type(channel_enum_t channel_type, size_t mtu) {
    switch (channel_type) {
    case UDP_SOCKET:
    case GE_ETH:
    case UNIX_SEQPACKET:
        return 1;
    case PIPE:
        return ((mtu > 0) && (mtu <= (PIPE_BUF - sizeof(pirate_header_t))));
    default:
        return 0;
    }
}

int pirate_close(int gd) {
    pirate_channel_t *channel;

    if ((channel = pirate_get_channel(gd)) == NULL) {
        return -1;
    }
    return pirate_close_channel(channel);
}

int pirate_close_channel(pirate_channel_t *channel) {
    int rv;
    pirate_close_t close_func;

    if (pirate_channel_type_valid(channel->param.channel_type) != 0) {
        return -1;
    }

    close_func = gaps_channel_funcs[channel->param.channel_type].close;

    if (close_func == NULL) {
        errno = ESOCKTNOSUPPORT;
        return -1;
    }

    rv = close_func(&channel->ctx);
    if (rv < 0) {
        return rv;
    }
    channel->param.channel_type = INVALID;
    return rv;
}

ssize_t pirate_read(int gd, void *buf, size_t count) {
    pirate_channel_t *channel = NULL;
    ssize_t rv;
    pirate_read_t read_func;

    if ((channel = pirate_get_channel(gd)) == NULL) {
        return -1;
    }

    if ((channel->ctx.common.flags & O_ACCMODE) != O_RDONLY) {
        errno = EBADF;
        return -1;
    }

    pirate_stats_t *stats = pirate_get_stats_internal(gd);
    pirate_channel_param_t *param = &channel->param;

    if (pirate_channel_type_valid(param->channel_type) != 0) {
        return -1;
    }

    read_func = gaps_channel_funcs[param->channel_type].read;

    if (read_func == NULL) {
        errno = ESOCKTNOSUPPORT;
        return -1;
    }

    stats->requests += 1;

    rv = read_func(&param->channel, &channel->ctx, buf, count);
    if (rv < 0) {
        if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
            stats->errs += 1;
        }
    } else {
        stats->success += 1;
        stats->bytes += rv;
    }
    return rv;
}

ssize_t pirate_write(int gd, const void *buf, size_t count) {
    pirate_channel_t *channel = NULL;
    ssize_t rv;
    pirate_write_t write_func;

    if ((channel = pirate_get_channel(gd)) == NULL) {
        return -1;
    }

    if ((channel->ctx.common.flags & O_ACCMODE) != O_WRONLY) {
        errno = EBADF;
        return -1;
    }

    pirate_stats_t *stats = pirate_get_stats_internal(gd);
    pirate_channel_param_t *param = &channel->param;

    if (pirate_channel_type_valid(param->channel_type) != 0) {
        return -1;
    }

    write_func = gaps_channel_funcs[param->channel_type].write;
    if (write_func == NULL) {
        errno = ESOCKTNOSUPPORT;
        return -1;
    }

    if ((param->drop > 0) && ((stats->requests % param->drop) == 0)) {
        stats->requests += 1;
        stats->fuzzed += 1;
        return count;
    } else {
        stats->requests += 1;
    }

    rv = write_func(&param->channel, &channel->ctx, buf, count);

    if (rv < 0) {
        if ((errno != EAGAIN) && (errno != EWOULDBLOCK)) {
            stats->errs += 1;
        }
    } else {
        stats->success += 1;
        stats->bytes += rv;
    }

    return rv;
}

ssize_t pirate_write_mtu_estimate(const pirate_channel_param_t *param) {
    pirate_write_mtu_t write_mtu_func;
    if (pirate_channel_type_valid(param->channel_type) != 0) {
        return -1;
    }

    write_mtu_func = gaps_channel_funcs[param->channel_type].write_mtu;

    if (write_mtu_func == NULL) {
        errno = ESOCKTNOSUPPORT;
        return -1;
    }

    return write_mtu_func(&param->channel, NULL);
}

ssize_t pirate_write_mtu(int gd) {
    pirate_channel_t *channel = NULL;
    pirate_channel_param_t *param = NULL;
    pirate_write_mtu_t write_mtu_func;

    if ((channel = pirate_get_channel(gd)) == NULL) {
        return -1;
    }

    param = &channel->param;

    if (pirate_channel_type_valid(param->channel_type) != 0) {
        return -1;
    }

    write_mtu_func = gaps_channel_funcs[param->channel_type].write_mtu;

    if (write_mtu_func == NULL) {
        errno = ESOCKTNOSUPPORT;
        return -1;
    }

    return write_mtu_func(&param->channel, &channel->ctx);
}
