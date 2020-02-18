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

#ifndef __PIRATE_GE_ETH_H
#define __PIRATE_GE_ETH_H

#include "primitives.h"

#define GE_ETH_MTU 1454

int pirate_ge_eth_open(int gd, int flags, pirate_channel_t *channels);
ssize_t pirate_ge_eth_read(int gd, pirate_channel_t *readers, void *buf,
                                size_t count);
ssize_t pirate_ge_eth_write(int gd, pirate_channel_t *writers, const void *buf,
                                size_t count);

#endif /* __PIRATE_GE_ETH_H */
