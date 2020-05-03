#include <stdio.h>
#include "LibDebug/Debug.h"
#include "camkes.h"
#include "ProxyNVM_common.h"
#include "system_config.h"


#define MEM_SIZE                        (1024*1024*36) // currently the fixed disk size provided by proxy for the 1st NVM channel

#define TEST_SMALL_SECTION_LEN          (MEM_SIZE / PAGE_SIZE) //arbitrary small chunk of data
#define TEST_WHOLE_MEM_LEN              (1024*128) // currently a placeholder, as writing 36 MiB lasts to long based on proxy
#define TEST_SIZE_OUT_OF_BOUNDS_LEN     PAGE_SIZE
#define TEST_ADDR_OUT_OF_BOUNDS_LEN     PAGE_SIZE

#define TEST_SMALL_SECTION_ADDR         (MEM_SIZE / PAGE_SIZE) //arbitrary memory address != 0
#define TEST_WHOLE_MEM_ADDR             0
#define TEST_SIZE_OUT_OF_BOUNDS_ADDR    (MEM_SIZE - 1)
#define TEST_ADDR_OUT_OF_BOUNDS_ADDR    (MEM_SIZE * 2)

static char proxyBuffer[PAGE_SIZE];

int run()
{
    int ret_value = ProxyNVMTest_init(proxyBuffer);

    if (ret_value < 0)
    {
        Debug_LOG_ERROR("%s(): (tester 1) Error initializing ProxyNVM! Errno:%d",
                        __func__, ret_value);
        return -1;
    }

    bool isSuccess;

    isSuccess = ProxyNVMTest_run(TEST_SMALL_SECTION_ADDR, TEST_SMALL_SECTION_LEN,
                                 "TEST SMALL SECTION");
    if (!isSuccess)
    {
        Debug_LOG_ERROR("Failed TEST SMALL SECTION!\n");
    }

    isSuccess = ProxyNVMTest_run(TEST_WHOLE_MEM_ADDR, TEST_SMALL_SECTION_LEN,
                                 "TEST WHOLE MEMORY");
    if (!isSuccess)
    {
        Debug_LOG_ERROR("Failed TEST WHOLE MEMORY!\n");
    }

    isSuccess = ProxyNVMTest_run(TEST_SIZE_OUT_OF_BOUNDS_ADDR,
                                 TEST_SIZE_OUT_OF_BOUNDS_LEN,
                                 "TEST SIZE OUT OF BOUNDS");
    if (!isSuccess)
    {
        Debug_LOG_ERROR("Failed TEST SIZE OUT OF BOUNDS!\n");
    }

    isSuccess = ProxyNVMTest_run(TEST_ADDR_OUT_OF_BOUNDS_ADDR,
                                 TEST_ADDR_OUT_OF_BOUNDS_LEN,
                                 "TEST ADDRESS OUT OF BOUNDS");
    if (!isSuccess)
    {
        Debug_LOG_ERROR("Failed TEST ADDRESS OUT OF BOUNDS!\n");
    }

    return 0;
}
