/* 
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#include <stdio.h>
#include "lib_debug/Debug.h"
#include "camkes.h"
#include "ProxyNVM_common.h"
#include "system_config.h"

static char proxyBuffer[PAGE_SIZE];

int run()
{
    int ret_value = init(proxyBuffer);

    if (ret_value < 0)
    {
        Debug_LOG_ERROR(
            "%s(): %s Error initializing ProxyNVM! Errno:%d",
            __func__,
            get_instance_name(),
            ret_value);
        return -1;
    }

    testStorageStartAddr();
    testStorageMidAddr();
    testStorageEndAddr();
    testStorageOverflow();
    testStorageUnderflow();
    testStorageFarOutOfBoundAddr();

    return 0;
}
