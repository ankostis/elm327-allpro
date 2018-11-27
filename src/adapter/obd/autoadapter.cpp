/**
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2009-2016 ObdDiag.Net. All rights reserved.
 *
 */

#include "autoadapter.h"

void AutoAdapter::getDescription()
{
    AdptSendReply("AUTO");
}

void AutoAdapter::getDescriptionNum()
{
    bool useAutoSP = config_->getBoolProperty(PAR_USE_AUTO_SP);
    const char* reply = useAutoSP ? "A0" : "0";
    AdptSendReply(reply);
}

int AutoAdapter::onRequest(const uint8_t* data, uint32_t len, uint32_t numOfRes)
{
    return REPLY_NO_DATA;
}

int AutoAdapter::doConnect(int protocol, bool sendReply)
{
    ProtocolAdapter* adapter = ProtocolAdapter::getAdapter(protocol);
    protocol = adapter->onConnectEcu(sendReply);
    if (protocol != 0) {
        sts_ = adapter->getStatus();
        return protocol;
    }
    return 0;
}

int AutoAdapter::onConnectEcu(bool sendReply)
{
    int protocol = 0;
    connected_ = false;
    sts_ = REPLY_NO_DATA;
    
    // PWM
    protocol = doConnect(ADPTR_PWM, sendReply);
    if (protocol > 0)
        return protocol;
    
    // VPW
    protocol = doConnect(ADPTR_VPW, sendReply);
    if (protocol > 0)
        return protocol;
    
    // ISO
    protocol = doConnect(ADPTR_ISO, sendReply);
    if (protocol > 0)
        return protocol;
    
    // CAN
    protocol = doConnect(ADPTR_CAN, sendReply);
    if (protocol > 0)
        return protocol;

    // CAN 29
    return doConnect(ADPTR_CAN_EXT, sendReply);
}

