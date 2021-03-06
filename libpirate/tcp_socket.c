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
 * Copyright 2019 Two Six Labs, LLC.  All rights reserved.
 */

#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "pirate_common.h"
#include "tcp_socket.h"

static void pirate_tcp_socket_init_param(pirate_tcp_socket_param_t *param) {
    if (param->min_tx == 0) {
        param->min_tx = PIRATE_DEFAULT_MIN_TX;
    }
    if (strnlen(param->reader_addr, 1) == 0) {
        strncpy(param->reader_addr, "0.0.0.0", sizeof(param->reader_addr) - 1);
    }
    if (strnlen(param->writer_addr, 1) == 0) {
        strncpy(param->writer_addr, "0.0.0.0", sizeof(param->writer_addr) - 1);
    }
}

int pirate_tcp_socket_parse_param(char *str, void *_param) {
    pirate_tcp_socket_param_t *param = (pirate_tcp_socket_param_t *)_param;
    char *ptr = NULL, *key, *val;
    char *saveptr1, *saveptr2;

    if (((ptr = strtok_r(str, OPT_DELIM, &saveptr1)) == NULL) ||
        (strcmp(ptr, "tcp_socket") != 0)) {
        return -1;
    }

    if ((ptr = strtok_r(NULL, OPT_DELIM, &saveptr1)) == NULL) {
        errno = EINVAL;
        return -1;
    }
    strncpy(param->reader_addr, ptr, sizeof(param->reader_addr) - 1);

    if ((ptr = strtok_r(NULL, OPT_DELIM, &saveptr1)) == NULL) {
        errno = EINVAL;
        return -1;
    }
    param->reader_port = strtol(ptr, NULL, 10);

    if ((ptr = strtok_r(NULL, OPT_DELIM, &saveptr1)) == NULL) {
        errno = EINVAL;
        return -1;
    }
    strncpy(param->writer_addr, ptr, sizeof(param->writer_addr) - 1);

    if ((ptr = strtok_r(NULL, OPT_DELIM, &saveptr1)) == NULL) {
        errno = EINVAL;
        return -1;
    }
    param->writer_port = strtol(ptr, NULL, 10);

    while ((ptr = strtok_r(NULL, OPT_DELIM, &saveptr1)) != NULL) {
        int rv = pirate_parse_key_value(&key, &val, ptr, &saveptr2);
        if (rv < 0) {
            return rv;
        } else if (rv == 0) {
            continue;
        }
        if (strncmp("buffer_size", key, strlen("buffer_size")) == 0) {
            param->buffer_size = strtol(val, NULL, 10);
        } else if (strncmp("min_tx_size", key, strlen("min_tx_size")) == 0) {
            param->min_tx = strtol(val, NULL, 10);
        } else if (strncmp("mtu", key, strlen("mtu")) == 0) {
            param->mtu = strtol(val, NULL, 10);
        } else {
            errno = EINVAL;
            return -1;
        }
    }
    return 0;
}

int pirate_tcp_socket_get_channel_description(const void *_param, char *desc, int len) {
    const pirate_tcp_socket_param_t *param = (const pirate_tcp_socket_param_t *)_param;
    char min_tx_str[32];
    char buffer_size_str[32];
    char mtu_str[32];

    min_tx_str[0] = 0;
    buffer_size_str[0] = 0;
    mtu_str[0] = 0;
    if (param->min_tx != 0) {
        snprintf(min_tx_str, 32, ",min_tx_size=%u", param->min_tx);
    }
    if (param->mtu != 0) {
        snprintf(mtu_str, 32, ",mtu=%u", param->mtu);
    }
    if (param->buffer_size != 0) {
        snprintf(buffer_size_str, 32, ",buffer_size=%u", param->buffer_size);
    }
    return snprintf(desc, len, "tcp_socket,%s,%u,%s,%u%s%s%s",
        param->reader_addr, param->reader_port,
        param->writer_addr, param->writer_port,
        buffer_size_str, min_tx_str, mtu_str);
}

static int populate_port(struct addrinfo *addr, int port) {
    struct sockaddr_in* ip4_addr;
    struct sockaddr_in6* ip6_addr;

    switch (addr->ai_family) {
        case AF_INET:
            ip4_addr = ((struct sockaddr_in*) addr->ai_addr);
            ip4_addr->sin_port = htons(port);
            return 0;
        case AF_INET6:
            ip6_addr = ((struct sockaddr_in6*) addr->ai_addr);
            ip6_addr->sin6_port = htons(port);
            return 0;
        default:
            return -1;
    }
}

static int tcp_socket_match_ipv4(int server_fd, tcp_socket_ctx *ctx, struct sockaddr_in *dest_addr) {
    int err;
    struct sockaddr_in ip4_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    ctx->sock = accept(server_fd, (struct sockaddr *) &ip4_addr, &addrlen);
    if (ctx->sock < 0) {
        err = errno;
        close(server_fd);
        ctx->sock = -1;
        errno = err;
        return -1;
    }
    if ((dest_addr->sin_addr.s_addr != INADDR_ANY) && (dest_addr->sin_addr.s_addr != ip4_addr.sin_addr.s_addr)) {
        return 0;
    }
    if ((dest_addr->sin_port != 0) && (dest_addr->sin_port != ip4_addr.sin_port)) {
        return 0;
    }
    return 1;
}

static int tcp_socket_match_ipv6(int server_fd, tcp_socket_ctx *ctx, struct sockaddr_in6 *dest_addr) {
    int err;
    struct sockaddr_in6 ip6_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in6);
    ctx->sock = accept(server_fd, (struct sockaddr *) &ip6_addr, &addrlen);
    if (ctx->sock < 0) {
        err = errno;
        close(server_fd);
        ctx->sock = -1;
        errno = err;
        return -1;
    }
    if ((memcmp((void*) &dest_addr->sin6_addr, (void*) &in6addr_any, sizeof(struct in6_addr)) != 0) &&
        (memcmp((void*) &dest_addr->sin6_addr, (void*) &ip6_addr.sin6_addr, sizeof(struct in6_addr)) != 0)) {
        return 0;
    }
    if ((dest_addr->sin6_port != 0) && (dest_addr->sin6_port != ip6_addr.sin6_port)) {
        return 0;
    }
    return 1;
}

static int tcp_socket_reader_open(pirate_tcp_socket_param_t *param, tcp_socket_ctx *ctx) {
    int err, rv;
    int server_fd;
    struct addrinfo hints, *src_addr = NULL, *dest_addr = NULL;
    struct linger lo;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_NUMERICHOST;
    src_addr = NULL;
    dest_addr = NULL;

    rv = getaddrinfo(param->reader_addr, NULL, &hints, &src_addr);
    if (rv != 0) {
        errno = EINVAL;
        ctx->sock = -1;
        goto end;
    }

    rv = getaddrinfo(param->writer_addr, NULL, &hints, &dest_addr);
    if (rv != 0) {
        errno = EINVAL;
        ctx->sock = -1;
        goto end;
    }

    rv = populate_port(src_addr, param->reader_port);
    if (rv != 0) {
        errno = EINVAL;
        ctx->sock = -1;
        goto end;
    }

    rv = populate_port(dest_addr, param->writer_port);
    if (rv != 0) {
        errno = EINVAL;
        ctx->sock = -1;
        goto end;
    }

    if (src_addr->ai_family != dest_addr->ai_family) {
        errno = EINVAL;
        ctx->sock = -1;
        goto end;
    }

    lo.l_onoff = 1;
    lo.l_linger = 0;

    server_fd = socket(src_addr->ai_family, SOCK_STREAM, 0);
    if (server_fd < 0) {
        ctx->sock = -1;
        goto end;
    }

    int enable = 1;
    rv = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (rv < 0) {
        err = errno;
        close(server_fd);
        ctx->sock = -1;
        errno = err;
        goto end;
    }

    rv = setsockopt(server_fd, SOL_SOCKET, SO_LINGER, &lo, sizeof(lo));
    if (rv < 0) {
        err = errno;
        close(server_fd);
        ctx->sock = -1;
        errno = err;
        goto end;
    }

    if (param->buffer_size > 0) {
        rv = setsockopt(server_fd, SOL_SOCKET, SO_RCVBUF, &param->buffer_size,
                    sizeof(param->buffer_size));
        if (rv < 0) {
            err = errno;
            close(server_fd);
            ctx->sock = -1;
            errno = err;
            goto end;
        }
    }

    rv = bind(server_fd, src_addr->ai_addr, src_addr->ai_addrlen);
    if (rv < 0) {
        err = errno;
        close(server_fd);
        ctx->sock = -1;
        errno = err;
        goto end;
    }

    rv = listen(server_fd, 0);
    if (rv < 0) {
        err = errno;
        close(server_fd);
        ctx->sock = -1;
        errno = err;
        goto end;
    }

    for (;;) {
        switch (dest_addr->ai_family) {
            case AF_INET:
                rv = tcp_socket_match_ipv4(server_fd, ctx, (struct sockaddr_in*) dest_addr->ai_addr);
                break;
            case AF_INET6:
                rv = tcp_socket_match_ipv6(server_fd, ctx, (struct sockaddr_in6*) dest_addr->ai_addr);
                break;
            default:
                rv = -1;
                errno = EINVAL;
                break;
        }
        if (rv < 0) {
            goto end;
        } else if (rv > 0) {
            break;
        }
        err = errno;
        shutdown(ctx->sock, SHUT_RDWR);
        close(ctx->sock);
        errno = err;
    }

    err = errno;
    shutdown(server_fd, SHUT_RDWR);
    close(server_fd);
    errno = err;

    // Test the gaps channel. This test performs a write operation
    // in the wrong direction of the gaps channel. This is acceptable
    // for now as the TCP protocol cannot be supported to unidirectional
    // network hardware. The TCP channel type is intended for non-gaps
    // hardware. If this assumption no longer holds then we will change
    // the implementation.
    //
    // The channel test is performed to detect two error conditions.
    // The error conditions are listed below along with other potential
    // solutions to the error conditions.
    //
    //     (1) The writers connect to this reader in a different order
    //     than the reader channels are opened. The writers may connect
    //     from different processes across multiple processors. It is
    //     likely the writers will arrive out of order with respect to
    //     opening the reader channels. We currently solve this problem
    //     by closing the socket if it is opened out of order. The writer
    //     must test the socket to confirm that the reader has not closed
    //     the connection. The writer must perform a blocking read()
    //     because if it performed a write() it could write data on the socket
    //     before the reader has closed the socket.
    //
    //     An alternative approach to this problem is to cache the sockets
    //     that are opened out of order and then provide these cached sockets
    //     to the gaps channels as they are opened.
    //
    //     A second alternative approach is to create a service that
    //     opens all the gaps channels at once. This would require
    //     changing the libpirate API with something like pirate_setup()
    //     that accepted all of the channel configuration strings.
    //     All gaps channels descriptors would have to be registered
    //     before any of the gaps channels are opened.
    //
    //     (2) A subsequent writer can connect to the server socket
    //     before the reader has had a chance to close the server
    //     socket. In Linux the sockets that are waiting in the listen()
    //     queue are fully-connected sockets. When the server socket
    //     is closed then the sockets in the listen() queue are terminated.
    //     The subsequent writer does not know that its socket has
    //     a closed connection until the writer tests the connection.
    //
    //     The service approach that opens all the gaps channels at once
    //     described above would solve this problem.
    char zero = 0;
    rv = read(ctx->sock, &zero, 1);
    if (rv < 0) {
        err = errno;
        close(ctx->sock);
        errno = err;
        return rv;
    }
    rv = send(ctx->sock, &zero, 1, MSG_NOSIGNAL);
    if (rv < 0) {
        err = errno;
        close(ctx->sock);
        errno = err;
        return -1;
    }

end:
    if (src_addr != NULL) {
        freeaddrinfo(src_addr);
    }
    if (dest_addr != NULL) {
        freeaddrinfo(dest_addr);
    }
    return ctx->sock;
}

static int tcp_socket_writer_connect(tcp_socket_ctx *ctx, struct addrinfo* dest_addr) {
    int err, rv;

    err = errno;
    rv = connect(ctx->sock, dest_addr->ai_addr, dest_addr->ai_addrlen);
    if (rv < 0) {
        // ECONNREFUSED: the reader is not ready for connections
        // ECONNRESET: either scenario (1) or (2) from above has occurred
        if ((errno == ECONNREFUSED) || (errno = ECONNRESET)) {
            struct timespec req;
            close(ctx->sock);
            errno = err;
            req.tv_sec = 0;
            req.tv_nsec = (rand() % 10) * 1e7;
            rv = nanosleep(&req, NULL);
            if (rv == 0) {
                return 0;
            }
        }
        err = errno;
        close(ctx->sock);
        errno = err;
        return -1;
    }
    return 1;
}

static int tcp_socket_writer_test(tcp_socket_ctx *ctx) {
    int err, rv;
    char zero = 0;

    err = errno;
    rv = send(ctx->sock, &zero, 1, MSG_NOSIGNAL);
    if (rv <= 0) {
        // ECONNRESET: either scenario (1) or (2) from above has occurred
        if ((rv == 0) || (errno == ECONNRESET) || (errno == EPIPE)) {
            struct timespec req;
            close(ctx->sock);
            errno = err;
            req.tv_sec = 0;
            req.tv_nsec = (rand() % 10) * 1e7;
            rv = nanosleep(&req, NULL);
            if (rv == 0) {
                return 0;
            }
        }
        err = errno;
        close(ctx->sock);
        errno = err;
        return -1;
    }

    err = errno;
    rv = read(ctx->sock, &zero, 1);
    if (rv <= 0) {
        // ECONNRESET: either scenario (1) or (2) from above has occurred
        if ((rv == 0) || (errno == ECONNRESET)) {
            struct timespec req;
            close(ctx->sock);
            errno = err;
            req.tv_sec = 0;
            req.tv_nsec = (rand() % 10) * 1e7;
            rv = nanosleep(&req, NULL);
            if (rv == 0) {
                return 0;
            }
        }
        err = errno;
        close(ctx->sock);
        errno = err;
        return -1;
    }
    return 1;
}

static int tcp_socket_writer_open(pirate_tcp_socket_param_t *param, tcp_socket_ctx *ctx) {
    int err, rv;
    struct addrinfo hints, *src_addr = NULL, *dest_addr = NULL;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_NUMERICHOST;
    src_addr = NULL;
    dest_addr = NULL;

    rv = getaddrinfo(param->writer_addr, NULL, &hints, &src_addr);
    if (rv != 0) {
        errno = EINVAL;
        ctx->sock = -1;
        goto end;
    }

    rv = getaddrinfo(param->reader_addr, NULL, &hints, &dest_addr);
    if (rv != 0) {
        errno = EINVAL;
        ctx->sock = -1;
        goto end;
    }

    rv = populate_port(src_addr, param->writer_port);
    if (rv != 0) {
        errno = EINVAL;
        ctx->sock = -1;
        goto end;
    }

    rv = populate_port(dest_addr, param->reader_port);
    if (rv != 0) {
        errno = EINVAL;
        ctx->sock = -1;
        goto end;
    }

    if (src_addr->ai_family != dest_addr->ai_family) {
        errno = EINVAL;
        ctx->sock = -1;
        goto end;
    }

    for (;;) {

        ctx->sock = socket(src_addr->ai_family, SOCK_STREAM, 0);
        if (ctx->sock < 0) {
            goto end;
        }

        if (param->buffer_size > 0) {
            rv = setsockopt(ctx->sock, SOL_SOCKET, SO_SNDBUF, &param->buffer_size,
                sizeof(param->buffer_size));
            if (rv < 0) {
                err = errno;
                close(ctx->sock);
                ctx->sock = -1;
                errno = err;
                goto end;
            }
        }

        rv = bind(ctx->sock, src_addr->ai_addr, src_addr->ai_addrlen);
        if (rv < 0) {
            err = errno;
            close(ctx->sock);
            ctx->sock = -1;
            errno = err;
            goto end;
        }

        rv = tcp_socket_writer_connect(ctx, dest_addr);
        if (rv < 0) {
            ctx->sock = -1;
            goto end;
        } else if (rv == 0) {
            continue;
        }
        // See comment in tcp_socket_reader_open()
        // on testing the connection.
        rv = tcp_socket_writer_test(ctx);
        if (rv < 0) {
            ctx->sock = -1;
            goto end;
        } else if (rv == 0) {
            continue;
        } else {
            break;
        }
    }

end:
    if (src_addr != NULL) {
        freeaddrinfo(src_addr);
    }
    if (dest_addr != NULL) {
        freeaddrinfo(dest_addr);
    }
    return ctx->sock;
}

int pirate_tcp_socket_open(void *_param, void *_ctx) {
    pirate_tcp_socket_param_t *param = (pirate_tcp_socket_param_t *)_param;
    tcp_socket_ctx *ctx = (tcp_socket_ctx *)_ctx;
    int rv = -1;
    int access = ctx->flags & O_ACCMODE;

    pirate_tcp_socket_init_param(param);
    if (param->reader_port == 0) {
        errno = EINVAL;
        return -1;
    }
    if (access == O_RDONLY) {
        rv = tcp_socket_reader_open(param, ctx);
    } else {
        rv = tcp_socket_writer_open(param, ctx);
    }
    if ((ctx->min_tx_buf = calloc(param->min_tx, 1)) == NULL) {
        return -1;
    }
    return rv;
}

int pirate_tcp_socket_close(void *_ctx) {
    tcp_socket_ctx *ctx = (tcp_socket_ctx *)_ctx;
    int err, rv = -1;
    int access = ctx->flags & O_ACCMODE;

    if (ctx->min_tx_buf != NULL) {
        free(ctx->min_tx_buf);
        ctx->min_tx_buf = NULL;
    }
    if (ctx->sock <= 0) {
        errno = ENODEV;
        return -1;
    }

    err = errno;
    shutdown(ctx->sock, SHUT_RDWR);
    errno = err;

    rv = close(ctx->sock);
    ctx->sock = -1;
    // Reader closes with RST packet.
    // If the reader closes before the writer then a
    // connection reset error is expected.
    if ((access == O_WRONLY) && (errno == ENOTCONN)) {
        errno = 0;
    }
    return rv;
}

ssize_t pirate_tcp_socket_read(const void *_param, void *_ctx, void *buf, size_t count) {
    const pirate_tcp_socket_param_t *param = (const pirate_tcp_socket_param_t *)_param;
    return pirate_stream_read((common_ctx*) _ctx, param->min_tx, buf, count);
}

ssize_t pirate_tcp_socket_write_mtu(const void *_param, void *_ctx) {
    (void) _ctx;
    const pirate_tcp_socket_param_t *param = (const pirate_tcp_socket_param_t *)_param;
    size_t mtu = param->mtu;
    if (mtu == 0) {
        return 0;
    }
    if (mtu < sizeof(pirate_header_t)) {
        errno = EINVAL;
        return -1;
    }
    return mtu - sizeof(pirate_header_t);
}

ssize_t pirate_tcp_socket_write(const void *_param, void *_ctx, const void *buf, size_t count) {
    const pirate_tcp_socket_param_t *param = (const pirate_tcp_socket_param_t *)_param;
    ssize_t mtu = pirate_tcp_socket_write_mtu(param, _ctx);
    return pirate_stream_write((common_ctx*)_ctx, param->min_tx, mtu, buf, count);
}
