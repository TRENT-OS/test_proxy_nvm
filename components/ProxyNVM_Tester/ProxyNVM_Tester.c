/* Copyright (C) 2020, Hensoldt Cyber GmbH */
#include <stdio.h>
#include "LibDebug/Debug.h"
#include "camkes.h"
#include "ProxyNVM_common.h"
#include "system_config.h"

#define TEST_STORAGE_START_ADDR             0
#define TEST_STORAGE_MID_ADDR               (memorySizeBytes / 2)
#define TEST_STORAGE_END_ADDR               (memorySizeBytes - TEST_DATA_SIZE)
#define TEST_STORAGE_OVERFLOW_ADDR          (memorySizeBytes - 1)
#define TEST_STORAGE_UNDERFLOW_ADDR         ((size_t)-1)
#define TEST_STORAGE_FAR_OUT_OF_BOUNDS_ADDR (memorySizeBytes * 2)

static char proxyBuffer[PAGE_SIZE];

int run()
{
    int ret_value = ProxyNVMTest_init(proxyBuffer);

    if (ret_value < 0)
    {
        Debug_LOG_ERROR(
            "%s(): %s Error initializing ProxyNVM! Errno:%d",
            __func__,
            get_instance_name(),
            ret_value);
        return -1;
    }

    bool isSuccess;

    isSuccess = ProxyNVMTest_run(
                    TEST_STORAGE_START_ADDR,
                    "TEST STORAGE START ADDRESS");

    if (!isSuccess)
    {
        Debug_LOG_ERROR("Failed TEST STORAGE START ADDRESS!");
    }

    isSuccess = ProxyNVMTest_run(
                    TEST_STORAGE_MID_ADDR,
                    "TEST STORAGE MID ADDRESS");

    if (!isSuccess)
    {
        Debug_LOG_ERROR("Failed TEST STORAGE MID ADDRESS!");
    }

    isSuccess = ProxyNVMTest_run(
                    TEST_STORAGE_END_ADDR,
                    "TEST STORAGE END ADDRESS");

    if (!isSuccess)
    {
        Debug_LOG_ERROR("Failed TEST STORAGE END ADDRESS!");
    }

    isSuccess = ProxyNVMTest_run(
                    TEST_STORAGE_OVERFLOW_ADDR,
                    "TEST STORAGE OVERFLOW");
    if (!isSuccess)
    {
        Debug_LOG_ERROR("Failed TEST STORAGE OVERFLOW!");
    }

    isSuccess = ProxyNVMTest_run(
                    TEST_STORAGE_UNDERFLOW_ADDR,
                    "TEST STORAGE UNDERFLOW");
    if (!isSuccess)
    {
        Debug_LOG_ERROR("Failed TEST STORAGE UNDERFLOW!");
    }

    isSuccess = ProxyNVMTest_run(
                    TEST_STORAGE_FAR_OUT_OF_BOUNDS_ADDR,
                    "TEST STORAGE FAR OUT OF BOUNDS ADDRESS");

    if (!isSuccess)
    {
        Debug_LOG_ERROR("Failed TEST STORAGE FAR OUT OF BOUNDS ADDRESS!");
    }

    return 0;
}
