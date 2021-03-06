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
 * Copyright 2020 Two Six Labs, LLC.  All rights reserved.
 */

#ifndef __PIRATE_COMMON_H
#define __PIRATE_COMMON_H

#include "libpirate.h"

#include <sys/types.h>
#include <BaseTsd.h>

#ifndef MIN
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#endif

#ifndef MAX
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    UINT32 count;
} pirate_header_t;

typedef struct {
    int flags;
} common_ctx;

int pirate_parse_key_value(char **key, char **val, char *ptr, char **saveptr);

#ifdef __cplusplus
}
#endif

#endif /* __PIRATE_COMMON_H */
