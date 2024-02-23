#
# Test ChanMux/Proxy NVM
#
# Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
# 
# SPDX-License-Identifier: GPL-2.0-or-later
#
# For commercial licensing, contact: info.cyber@hensoldt.net
#

cmake_minimum_required(VERSION 3.7.2)

#-------------------------------------------------------------------------------
# use the SDK
find_package("os-sdk" REQUIRED)
os_sdk_set_defaults()
os_sdk_setup(CONFIG_FILE "system_config.h" CONFIG_PROJECT "system_config")


#-------------------------------------------------------------------------------
project(tests_proxy_nvm C)

DeclareCAmkESComponent(
    ProxyNVM_Tester
    SOURCES
        components/ProxyNVM_Tester/ProxyNVM_Tester.c
        components/ProxyNVM_Tester/ProxyNVM_common.c
    C_FLAGS
        -Wall
        -Werror
    LIBS
        system_config
        os_core_api
        lib_debug
        lib_macros
        proxy_nvm
        chanmux_client
)

ChanMux_UART_DeclareCAmkESComponents(
    ChanMux_UART
    components/ChanMux/ChanMux_config.c
    system_config
)


os_sdk_create_CAmkES_system("main.camkes")
