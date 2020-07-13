/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "ProxyNVM.h"
#include "ChanMux/ChanMuxClient.h"
#include "camkes.h"


/* Defines -------------------------------------------------------------------*/
#define TEST_DATA_SIZE                  (1024*128)

/* Instance variables ---------------------------------------------------------*/

static const ChanMuxClientConfig_t chanMuxClientConfig = {
    .port = CHANMUX_DATAPORT_ASSIGN(chanMux_chan_portRead,
                                    chanMux_chan_portWrite),
    .wait        = chanMux_chan_EventHasData_wait,
    .write       = chanMux_Rpc_write,
    .read        = chanMux_Rpc_read
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

/* Public functions ----------------------------------------------------------*/

int ProxyNVMTest_init(char* proxyBuffer)
{

    bool isSuccess = ChanMuxClient_ctor(&testChanMuxClient,
                                        &chanMuxClientConfig);

    if (!isSuccess)
    {
        Debug_LOG_ERROR("Failed to construct testChanMuxClient!\n");
        return -1;
    }

    isSuccess = ProxyNVM_ctor(&testProxyNVM, &testChanMuxClient,
                              proxyBuffer, PAGE_SIZE);

    if (!isSuccess)
    {
        Debug_LOG_ERROR("Failed to construct testProxyNVM!\n");
        return -1;
    }

    for(uint8_t* iterator = writeData;  iterator < &writeData[TEST_DATA_SIZE];)
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

bool ProxyNVMTest_run(size_t address, size_t length, const char* testName)
{
    size_t ret_value = ProxyNVM_write(ProxyNVM_TO_NVM(&testProxyNVM),
                                      address, (const char*)writeData, length);

    if (ret_value == length)
    {
        Debug_LOG_INFO("\n%s: Write succeded!", testName);
    }
    else
    {
        Debug_LOG_ERROR("\n%s: Write failed!\nTried to write %zu bytes but written only %zu bytes.",
                        testName, length, ret_value);
        return false;
    }

    ret_value = ProxyNVM_read(ProxyNVM_TO_NVM(&testProxyNVM), address,
                              (char*)readData, length);
    if (ret_value == length)
    {
        Debug_LOG_INFO("\n%s: Read succeded!", testName);
    }
    else
    {
        Debug_LOG_ERROR("\n%s: Read failed!\nTried to read %zu bytes but read only %zu bytes.",
                        testName, length, ret_value);
        return false;
    }

    for (int i = 0; i < length; i++)
    {
        if (writeData[i] != readData[i])
        {
            Debug_LOG_ERROR("\n%s: Read values corrupted!\nOn position %d written %02x, but read %02x",
                            testName, i, writeData[i], readData[i]);
            return false;
        }
    }
    Debug_LOG_INFO("\n%s: Read values match the write values!", testName);

    return true;
}
