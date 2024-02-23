/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "ProxyNVM_common.h"
#include "ProxyNVM.h"
#include "ChanMux/ChanMuxClient.h"
#include "camkes.h"
#include "lib_macros/Test.h"

#define TEST_STORAGE_START_ADDR             0
#define TEST_STORAGE_MID_ADDR               (memorySizeBytes / 2)
#define TEST_STORAGE_END_ADDR               (memorySizeBytes - TEST_DATA_SIZE)
#define TEST_STORAGE_OVERFLOW_ADDR          (memorySizeBytes - 1)
#define TEST_STORAGE_UNDERFLOW_ADDR         ((size_t)-1)
#define TEST_STORAGE_FAR_OUT_OF_BOUNDS_ADDR (memorySizeBytes * 2)

static const ChanMuxClientConfig_t chanMuxClientConfig =
{
    .port   = CHANMUX_DATAPORT_ASSIGN(
                chanMux_chan_portRead,
                chanMux_chan_portWrite),

    .wait   = chanMux_chan_eventHasData_wait,
    .write  = chanMux_Rpc_write,
    .read   = chanMux_Rpc_read
};


static ProxyNVM testProxyNVM;
static ChanMuxClient testChanMuxClient;

static uint8_t const initData[] =
"KXyLGoRV1jnSKZ22e1EV16vX7XiyqLTf2LhpyMf3MxSKz3ZbOCzJTQ5bV0XRMeH9O03k5p8X6Rlrf6"
"5bIPj1RUsFTGbURwSDyfvE9uDc2Yu86vBILHZYNHgQ2g1lGClmmZNNnvdJPFBTKRK4pNAIfS366vP1"
"DaPs97VEAkEW3PfmXFtf7ERRMgH1Bvudeuttk1QGe7DnVcJndyxzRSwfDNZmsPlhKYSrEjtde7zuLO"
"peWUXp98yZmVG0ewVaEqGzqhWk0ib4IHsYMOfGSWbZr5amqaSrcFPIyx0oRyBEVZJclCphd43poUIU"
"cDQkQXEYyvf7Nk1yDvHCOWPhnd9nghZZlZPKj6p3lpKASC1TcnxPMmaI3ZUTJSeVNiBUAEEM4P8fIJ"
"eNzUSpMK3qsc5xxikzncG8RD3mJImKN6SbnqlgHTJovqsoQkvYdbOsUR4Qwg1hsstuO4ZZo2uKA1my"
"VBk65dhu6rwtuTKmMkY8bphZnP4s5BQiPSgXWh3g9izuGnw95RqTwfObvQKqNgW9TJxM3nGGKhHAxC"
"pM7KS5TqT3hGvDluxC4G2zc9EdhhMUrqX4GhsqMcnoOf44UskFjnx7TJ62fws8pUuwndzqd99yzGuC"
"NUvBjXWjYqul8nmCQUJv3YYHryUdxsHvTiCN9zAoYXy2BkQ2DCLelZTueAR6kF7151cAcW85Yj6ZFD"
"ndznRWItKxd2vIyOSsmaMG8IBZlbvu0QGkuuFx2beYtV9MhzV6moFKyzauHVlL06wOvFFasSncoxE4"
"MGEAdM9MNPIaY2oiyAEDammlcqwBad0ZC0HAdgkarfW4dyBaYwCQoq4vnR5kGD2zZv85u2pD6W234A"
"d4sXj4j1SdtvCoFcPPOkH1ytmUwcqw2bPqc6D87PMkwb313gXvHCblk24B295yoZheSXefflwtdo3P"
"a3EJhkfQr9xoesMiNTal7FDtqTuXILPoJnmxPQfJnJ2QcI0cEldq5KZLyufP20U2cI0cEldq5KZLy";

static uint8_t writeData[TEST_DATA_SIZE] = {0};
static uint8_t  readData[TEST_DATA_SIZE] = {0};

static
bool
writeTest(
    const size_t address,
    char const * const testName)
{
    const size_t ret_value = ProxyNVM_write(
                                ProxyNVM_TO_NVM(&testProxyNVM),
                                address,
                                (const char*)writeData,
                                TEST_DATA_SIZE);

    if (ret_value == TEST_DATA_SIZE)
    {
        Debug_LOG_INFO("%s: Write succeeded!", testName);
    }
    else
    {
        Debug_LOG_ERROR(
            "%s: Write failed! Tried to write %u bytes but written only %zu "
            "bytes.",
            testName,
            TEST_DATA_SIZE,
            ret_value);

        return false;
    }

    return true;
}

static
bool
readTest(
    const size_t address,
    char const * const testName)
{
    const size_t ret_value = ProxyNVM_read(
                                ProxyNVM_TO_NVM(&testProxyNVM),
                                address,
                                (char*)readData,
                                TEST_DATA_SIZE);

    if (ret_value == TEST_DATA_SIZE)
    {
        Debug_LOG_INFO("%s: Read succeeded!", testName);
    }
    else
    {
        Debug_LOG_ERROR(
            "%s: Read failed! Tried to read %u bytes but read only %zu bytes.",
            testName,
            TEST_DATA_SIZE,
            ret_value);

        return false;
    }

    for (int i = 0; i < TEST_DATA_SIZE; i++)
    {
        if (writeData[i] != readData[i])
        {
            Debug_LOG_ERROR(
                "%s: Read values corrupted! On position %d written %02x, but "
                "read %02x",
                testName,
                i,
                writeData[i],
                readData[i]);

            return false;
        }
    }

    Debug_LOG_INFO("%s: Read values match the write values!", testName);
    return true;
}

static
bool
eraseTest(
    const size_t address,
    char const * const testName)
{
    size_t ret_value = ProxyNVM_erase(
                            ProxyNVM_TO_NVM(&testProxyNVM),
                            address,
                            TEST_DATA_SIZE);

    if (ret_value == TEST_DATA_SIZE)
    {
        Debug_LOG_INFO("%s: Erase succeeded!", testName);
    }
    else
    {
        Debug_LOG_ERROR(
            "%s: Erase failed! Tried to erase %u bytes but erased only %zu "
            "bytes.",
            testName,
            TEST_DATA_SIZE,
            ret_value);

        return false;
    }

    ret_value = ProxyNVM_read(
                    ProxyNVM_TO_NVM(&testProxyNVM),
                    address,
                    (char*)readData,
                    TEST_DATA_SIZE);

    if (ret_value == TEST_DATA_SIZE)
    {
        Debug_LOG_INFO("%s: Read after the erase succeeded!", testName);
    }
    else
    {
        Debug_LOG_ERROR(
            "%s: Read after the erase failed! Tried to read %u bytes but read "
            "only %zu bytes.",
            testName,
            TEST_DATA_SIZE,
            ret_value);

        return false;
    }

    for (size_t i = 0; i < TEST_DATA_SIZE; i++)
    {
        if (0xFF != readData[i])
        {
            Debug_LOG_ERROR(
                "%s: Read values corrupted! On position %zu expected 0xFF, but "
                "read %02x",
                testName,
                i,
                readData[i]);

            return false;
        }
    }
    Debug_LOG_INFO("%s: Memory was erased successfully!", testName);

    return true;
}

/**
 * @brief   Tests the ProxyNVM by writing, reading and erasing a passed amount
 *          of data from the NVM.
 *
 * @retval  true  - All operations were successful.
 * @retval  false - Failure was encountered during one of the test operations.
 */
void
run_pos(
    size_t address,         //!< [in] The address in the NVM memory.
    const char* testName    //!< [in] The specific name of the test, which will
                            //!<      be printed for debugging.
)
{
    const bool result = (writeTest(address, testName)
                      &&  readTest(address, testName)
                      && eraseTest(address, testName));
    ASSERT_TRUE(result);
}

/**
 * @brief   Tests negatively the ProxyNVM by writing, reading and erasing a
 *          passed amount of data from the NVM.
 *
 * @note    This is a negative test, so all operation are expected to fail!
 *
 * @retval  true  - All operations failed.
 * @retval  false - One of the test operations were successful.
 */
void
run_neg(
    size_t address,         //!< [in] The address in the NVM memory.
    const char* testName    //!< [in] The specific name of the test, which will
                            //!<      be printed for debugging.
)
{
    const bool result = (!writeTest(address, testName)
                      && !readTest(address, testName)
                      && !eraseTest(address, testName));
    ASSERT_TRUE(result);
}


// Public functions ------------------------------------------------------------

int init(char* proxyBuffer)
{
    bool isSuccess = ChanMuxClient_ctor(
                        &testChanMuxClient,
                        &chanMuxClientConfig);

    if (!isSuccess)
    {
        Debug_LOG_ERROR("Failed to construct testChanMuxClient!");
        return -1;
    }

    isSuccess = ProxyNVM_ctor(
                    &testProxyNVM,
                    &testChanMuxClient,
                    proxyBuffer,
                    PAGE_SIZE);

    if (!isSuccess)
    {
        Debug_LOG_ERROR("Failed to construct testProxyNVM!");
        return -1;
    }

    for(
        uint8_t* iterator = writeData;
        iterator < &writeData[TEST_DATA_SIZE];)
    {
        size_t leftSpaceSize      = &writeData[TEST_DATA_SIZE] - iterator;
        size_t dataToBeCopiedSize = leftSpaceSize > sizeof(initData)
                                        ? sizeof(initData)
                                        : leftSpaceSize;

        memcpy(iterator, initData, dataToBeCopiedSize);
        iterator += dataToBeCopiedSize;
    }

    return 0;
}

void
testStorageStartAddr(void)
{
    TEST_START();
    run_pos(TEST_STORAGE_START_ADDR, "TEST STORAGE START ADDRESS");
    TEST_FINISH();
}

void
testStorageMidAddr(void)
{
    TEST_START();
    run_pos(TEST_STORAGE_MID_ADDR, "TEST STORAGE MID ADDRESS");
    TEST_FINISH();
}

void
testStorageEndAddr(void)
{
    TEST_START();
    run_pos(TEST_STORAGE_MID_ADDR, "TEST STORAGE END ADDRESS");
    TEST_FINISH();
}

void
testStorageOverflow(void)
{
    TEST_START();
    run_neg(TEST_STORAGE_OVERFLOW_ADDR,  "TEST STORAGE OVERFLOW");
    TEST_FINISH();
}

void
testStorageUnderflow(void)
{
    TEST_START();
    run_neg(TEST_STORAGE_UNDERFLOW_ADDR, "TEST STORAGE UNDERFLOW");
    TEST_FINISH();
}

void
testStorageFarOutOfBoundAddr(void)
{
    TEST_START();
    run_neg(
        TEST_STORAGE_FAR_OUT_OF_BOUNDS_ADDR,
        "TEST STORAGE FAR OUT OF BOUNDS ADDRESS");
    TEST_FINISH();
}
