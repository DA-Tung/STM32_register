#ifndef _NET_H_
#define _NET_H_

#include "stm32f4xx.h"
#include "stm32f407xx.h"

void NET_SendFrame(uint8_t *EthFrame, uint16_t length);
void NET_Check(void);
void NET_Read(uint8_t *Net_Buffer, uint16_t length);

#endif








