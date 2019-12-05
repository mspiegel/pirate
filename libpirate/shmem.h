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

#ifndef __SHMEM_H
#define __SHMEM_H

#include "primitives.h"
#include "shmem_buffer.h"

#define DEFAULT_SHMEM_BUFFER (131072)

int shmem_buffer_open(int gd, int flags, char *name, pirate_channel_t *channel);

ssize_t shmem_buffer_read(shmem_buffer_t *shmem_buffer, void *buf,
                          size_t count);

ssize_t shmem_buffer_write(shmem_buffer_t *shmem_buffer, const void *buf,
                           size_t size);

int shmem_buffer_close(int flags, shmem_buffer_t *shmem_buffer);

#endif
