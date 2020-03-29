/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * System configuration
 *
 */
#pragma once


//-----------------------------------------------------------------------------
// Debug
//-----------------------------------------------------------------------------

#if !defined(NDEBUG)
#   define Debug_Config_STANDARD_ASSERT
#   define Debug_Config_ASSERT_SELF_PTR
#else
#   define Debug_Config_DISABLE_ASSERT
#   define Debug_Config_NO_ASSERT_SELF_PTR
#endif

#define Debug_Config_LOG_LEVEL              Debug_LOG_LEVEL_DEBUG
#define Debug_Config_INCLUDE_LEVEL_IN_MSG
#define Debug_Config_LOG_WITH_FILE_LINE


//-----------------------------------------------------------------------------
// Memory
//-----------------------------------------------------------------------------

#define Memory_Config_USE_STDLIB_ALLOC


//-----------------------------------------------------------------------------
// CHanMux
//-----------------------------------------------------------------------------

enum
{
    CHANNEL_UNUSED_0,        // 0
    CHANNEL_UNUSED_1,        // 1
    CHANNEL_UNUSED_2,        // 2
    CHANNEL_UNUSED_3,        // 3
    CHANNEL_UNUSED_4,        // 4
    CHANNEL_UNUSED_5,        // 5
    CHANNEL_NVM_USER1_DATA,  // 6
    CHANNEL_UNUSED_7,        // 7
    CHANNEL_UNUSED_8,        // 8
    CHANNEL_NVM_USER2_DATA,  // 9

    CHANMUX_NUM_CHANNELS,    // 10
};
