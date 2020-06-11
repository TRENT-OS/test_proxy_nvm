#
# Test ChanMux/Proxy NVM
#
# Copyright (C) 2020, Hensoldt Cyber GmbH
#

cmake_minimum_required(VERSION 3.7.2)

os_set_config_file(system_config.h)

os_use_libs(
    OS_LIBS
    OS_CHANMUX
)


project(system_config C)
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR})


project(tests_proxy_nvm C)

DeclareCAmkESComponent(ProxyNVM_Tester1
    SOURCES
        src/ProxyNVM_Tester1.c
        src/ProxyNVM_common.c
    C_FLAGS
        -Wall -Werror
    LIBS
        system_config
        os_core_api
        os_libs
        proxy_nvm
)

DeclareCAmkESComponent(ProxyNVM_Tester2
    SOURCES
        src/ProxyNVM_Tester2.c
        src/ProxyNVM_common.c
    C_FLAGS
        -Wall -Werror
    LIBS
        system_config
        os_core_api
        os_libs
        proxy_nvm
)

DeclareCAmkESComponent_UART(
    UART
)

DeclareCAmkESComponent_ChanMux(
    ChanMux
    components/ChanMux/ChanMux_config.c
    system_config
)

#ROOT SERVER
DeclareAndCreateCamkESSystem(main.camkes)
GenerateSimulateScript()
