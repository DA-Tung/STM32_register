#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include "stm32f4xx.h"
#include "stm32f407xx.h"

typedef struct
{
	uint8_t * buffer;
	int head;
	int tail;
	uint16_t Max_len;
}Ring_Buffer_Data;

void RingBuffer_Init(Ring_Buffer_Data *Ring_Buff, uint8_t *buff, uint16_t len);
uint8_t RingBuffer_push(Ring_Buffer_Data *Ring_Buff, uint8_t data);
uint8_t RingBuffer_pop(Ring_Buffer_Data *Ring_Buff, uint8_t* data);

#endif // _RING_BUFFER_H_


