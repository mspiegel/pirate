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

#include <argp.h>
#include <stdio.h>
#include <string.h>
#include "gaps_packet.h"
#include "common.h"
#include "ts_crypto.h"

#ifndef GAPS_DISABLE
#pragma pirate enclave declare(purple)
#endif

extern const char *program_name;

typedef struct {
    verbosity_t verbosity;

    struct {
        const char *conf_file;
        const char *conf_sect;
        void *tsa;
    } ts;

    gaps_app_t app;

    gaps_channel_ctx_t * const proxy_to_signer;
    gaps_channel_ctx_t * const signer_to_proxy;
} signer_t;

/* Command-line options */
extern const char *argp_program_version;
static struct argp_option options[] = {
    { "conf",      'c', "PATH",    0, "Configuration file path",       0 },
    { "conf_sect", 's', "SECTION", 0, "Configuration section",         0 },
    { "verbose",   'v', NULL,      0, "Increase verbosity level",      0 },
    { "proxy-to-signer", 1000, "CONFIG", 0, "Proxy to signer channel", 1 },
    { "signer-to-proxy", 1001, "CONFIG", 0, "Signer to proxy channel", 1 },
    { NULL,         0,  NULL,      0, GAPS_CHANNEL_OPTIONS,            2 },
    { NULL,         0,  NULL,      0, 0,                               0 }
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    signer_t *signer = (signer_t *) state->input;

    switch (key) {

    case 1000:
        signer->proxy_to_signer->conf = arg;
        break;

    case 1001:
        signer->signer_to_proxy->conf = arg;
        break;

    case 'c':
        signer->ts.conf_file = arg;
        break;

    case 's':
        signer->ts.conf_sect = arg;
        break;

    case 'v':
        if (signer->verbosity < VERBOSITY_MAX) {
            signer->verbosity++;
        }
        break;

    default:
        break;

    }

    return 0;
}

static void parse_args(int argc, char *argv[], signer_t *signer) {
    struct argp argp = {
        .options = options,
        .parser = parse_opt,
        .args_doc = NULL,
        .doc = "Timestamp signing service",
        .children = NULL,
        .help_filter = NULL,
        .argp_domain = NULL
    };

    argp_parse(&argp, argc, argv, 0, 0, signer);
}

/* Run the signer */
static void *signer_thread(void *arg) {
    signer_t *signer = (signer_t *) arg;
    ssize_t len = 0;
    tsa_request_t req = TSA_REQUEST_INIT;
    tsa_response_t rsp = TSA_RESPONSE_INIT;

    while (gaps_running()) {
        /* Receive sign request */
        len = gaps_packet_read(signer->proxy_to_signer->gd, &req, sizeof(req));
        if (len != sizeof(req)) {
            ts_log(WARN, "Failed to receive sign request");
            continue;
        }
        log_tsa_req(signer->verbosity, "Timestamp request received", &req);

        /* Sign with a timestamp */
        ts_sign(signer->ts.tsa, &req, &rsp);

        /* Reply */
        if (gaps_packet_write(signer->signer_to_proxy->gd, &rsp.hdr, sizeof(rsp.hdr)) != 0) {
            ts_log(WARN, "Failed to send sign response header");
            continue;
        }
        if (gaps_packet_write(signer->signer_to_proxy->gd, &rsp.ts, rsp.hdr.len) != 0) {
            ts_log(WARN, "Failed to send sign response body");
            continue;
        }
        log_tsa_rsp(signer->verbosity, "Timestamp response sent", &rsp);
    }

    return 0;  /* Should never get here */
}

/* Release signer resources */
static void signer_term(signer_t *signer) {
    /* Terminate the timestamp authority */
    ts_term(signer->ts.tsa);
    signer->ts.tsa = NULL;
}


int signing_service_main(int argc, char *argv[]) PIRATE_ENCLAVE_MAIN("purple") {
    program_name = CLR(WHITE, "Signing Service");

    signer_t signer = {
        .verbosity = VERBOSITY_NONE,

        .ts = {
            .conf_file = DEFAULT_CONF_PATH,
            .conf_sect = DEFAULT_CONF_SECTION
        },

        .app = {
            .threads = {
                THREAD_ADD(signer_thread, &signer, "ts_signer"),
                THREAD_END
            },
            .on_shutdown = NULL,
            .ch = {
                GAPS_CHANNEL(O_RDONLY, DEFAULT_PROXY_TO_SIGNER_CONF, "proxy->signer"),
                GAPS_CHANNEL(O_WRONLY, DEFAULT_SIGNER_TO_PROXY_CONT, "proxy<-signer"),
                GAPS_CHANNEL_END
            }
        },

        .proxy_to_signer = &signer.app.ch[0],
        .signer_to_proxy = &signer.app.ch[1]
    };

    parse_args(argc, argv, &signer);

    ts_log(INFO, "Starting");

    /* Initialize signer crypto resources */
    signer.ts.tsa = ts_init(signer.ts.conf_file, signer.ts.conf_sect);
    if (signer.ts.tsa == NULL) {
        ts_log(ERROR, "Failed to initialize timestamp context");
        return -1;
    }

    /* Run the signer */
    if (gaps_app_run(&signer.app) != 0) {
        ts_log(ERROR, "Failed to start the signing proxy");
        signer_term(&signer);
        return -1;
    }

    int rv = gaps_app_wait_exit(&signer.app);

    /* Release signer crypto resources */
    ts_term(signer.ts.tsa);
    signer.ts.tsa = NULL;
    return rv;
}

#ifdef GAPS_DISABLE
int main(int argc, char *argv[]) {
    return signing_service_main(argc, argv);
}
#endif
