/*
 *  Channel MUX
 *
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#include "system_config.h"
#include "ChanMux/ChanMux.h"
#include <camkes.h>


//------------------------------------------------------------------------------
static unsigned int
resolveChannel(
    unsigned int  sender_id,
    unsigned int  chanNum_local)
{
    switch (sender_id)
    {
    //----------------------------------
    case CHANMUX_ID_TESTER_1:
        // TODO: check that chanNum_local is 0, reject anything else
        return CHANMUX_CHANNEL_NVM_1;
    //----------------------------------
    case CHANMUX_ID_TESTER_2:
        // TODO: check that chanNum_local is 0, reject anything else
        return CHANMUX_CHANNEL_NVM_2;
    //----------------------------------
    default:
        break;
    }

    return INVALID_CHANNEL;
}


//------------------------------------------------------------------------------
static uint8_t nvm_fifo[2][PAGE_SIZE];
static ChanMux_Channel_t nvm_channel[2];


//------------------------------------------------------------------------------
static const ChanMux_ChannelCtx_t channelCtx[] = {

    CHANMUX_CHANNEL_CTX(
        CHANMUX_CHANNEL_NVM_1,
        &nvm_channel[0],
        nvm_fifo[0], // must be the buffer and not a pointer
        CHANMUX_DATAPORT_ASSIGN(tester1_chan_portRead, tester1_chan_portWrite),
        tester1_chan_DataAvailable_emit),

    CHANMUX_CHANNEL_CTX(
        CHANMUX_CHANNEL_NVM_2,
        &nvm_channel[1],
        nvm_fifo[1], // must be the buffer and not a pointer
        CHANMUX_DATAPORT_ASSIGN(tester2_chan_portRead, tester2_chan_portWrite),
        tester2_chan_DataAvailable_emit),

};


//------------------------------------------------------------------------------
// this is used by the ChanMux component
const ChanMux_Config_t cfgChanMux =
{
    .resolveChannel = &resolveChannel,
    .numChannels    = ARRAY_SIZE(channelCtx),
    .channelCtx     = channelCtx,
};
