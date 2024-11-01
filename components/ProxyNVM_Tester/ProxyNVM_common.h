/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @brief   Common functions used for initializing and using the ProxyNVM.
 * @file
 */
#pragma once

#include <stddef.h>
#include <stdbool.h>

#define TEST_DATA_SIZE                  (2 * PAGE_SIZE)

/**
 * @brief   Initializes the ChanMuxClient and the ProxyNVM and maps the passed
 *          channel to the defined dataport.
 */
int
init(char* proxyBuffer);

void
testStorageStartAddr(void);

void
testStorageMidAddr(void);

void
testStorageEndAddr(void);

void
testStorageOverflow(void);

void
testStorageUnderflow(void);

void
testStorageFarOutOfBoundAddr(void);
