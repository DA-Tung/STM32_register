#ifndef _ARP_H_
#define _ARP_H_

#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "string.h"

void ARP_read_request(uint8_t *ARP_buffer, uint16_t length);
void ARP_send_request(uint8_t *ip_address);

#endif



