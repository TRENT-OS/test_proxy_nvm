/* Copyright (C) 2020, Hensoldt Cyber GmbH */

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
ProxyNVMTest_init(char* proxyBuffer);

void
ProxyNVMTest_testStorageStartAddr(void);

void
ProxyNVMTest_testStorageMidAddr(void);

void
ProxyNVMTest_testStorageEndAddr(void);

void
ProxyNVMTest_testStorageOverflow(void);

void
ProxyNVMTest_testStorageUnderflow(void);

void
ProxyNVMTest_testStorageFarOutOfBoundAddr(void);
