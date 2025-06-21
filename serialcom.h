#ifndef SERIALCOM_H
#define SERIALCOM_H
#pragma once

#include "comminterface.h"

int TWSBus1_Send(CommInterface* comInt, int address, int cmd,
                 const unsigned char* tx_buf, int tx_len,
                 unsigned char* rx_buf, unsigned short* rx_len);

// Send a command with data, no response expected
int TWSBus1_Send(CommInterface* comInt, int address, int cmd,
                 const unsigned char* tx_buf, int tx_len);

// Send a command only (no data, no response)
int TWSBus1_Send(CommInterface* comInt, int address, int cmd);

// Send a command and update serialComStatus (for status-only commands)
int TWSBus1_Send_Command(CommInterface* commInt, unsigned char NSlave, unsigned char Cmd, unsigned char* RxData, bool activated = true);
int TWSBus1_Send_Command(CommInterface* commInt, unsigned char NSlave, unsigned char Cmd, unsigned short TxData, unsigned char* RxData, bool activated = true);


#endif // SERIALCOM_H
