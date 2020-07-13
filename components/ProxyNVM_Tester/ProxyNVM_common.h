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

/**
 * @brief   Tests the ProxyNVM by writing and then reading a passed amount of
 *          data from the NVM.
 *
 * @retval  true  - All operations were successful.
 * @retval  false - Failure was encountered during one of the test operations.
 */
bool
ProxyNVMTest_run(
    size_t address,         //!< [in] The address in the NVM memory.
    const char* testName    //!< [in] The specific name of the test, which will
                            //!<      be printed for debugging.
);

