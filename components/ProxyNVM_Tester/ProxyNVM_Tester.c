/* Copyright (C) 2020, Hensoldt Cyber GmbH */
#include <stdio.h>
#include "LibDebug/Debug.h"
#include "camkes.h"
#include "ProxyNVM_common.h"
#include "system_config.h"

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

    ProxyNVMTest_testStorageStartAddr();
    ProxyNVMTest_testStorageMidAddr();
    ProxyNVMTest_testStorageEndAddr();
    ProxyNVMTest_testStorageOverflow();
    ProxyNVMTest_testStorageUnderflow();
    ProxyNVMTest_testStorageFarOutOfBoundAddr();

    return 0;
}
