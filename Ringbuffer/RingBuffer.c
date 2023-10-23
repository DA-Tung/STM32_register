#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "Ringbuffer.h"
#include "usart.h"

// RING BUFFER PUSH DATA_______________________________________________________________
uint8_t RingBuffer_push(Ring_Buffer_Data *Ring_Buff, uint8_t data)
{
	int next;
	
	next = Ring_Buff->head + 1; 				// Next is where head will point to after this write
	
	if(next >= Ring_Buff->Max_len)
	{
		next = 0;
	}
	if( next == Ring_Buff->tail) 				// if the head +1 = tail, circular buffer is full
	{
		return -1;
	}
	
	Ring_Buff->buffer[Ring_Buff->head] = data;	// Load data and then move
	Ring_Buff->head = next;						// head to next data offset
	
	return 0;
}
// RING BUFFER POP DATA_______________________________________________________________
uint8_t RingBuffer_pop(Ring_Buffer_Data *Ring_Buff, uint8_t* data)
{
	int next = 0;
	if(Ring_Buff->head == Ring_Buff->tail)		// if the head = tail, don't have any data
	{
		return -1;
	}
	next = Ring_Buff->tail + 1;					// next is where tail will point to after the read
	
	if(next >= Ring_Buff->Max_len)
	{
		next = 0;
	}
	
	*data = Ring_Buff->buffer[Ring_Buff->tail];	// Read data and the move
	Ring_Buff->tail = next;						// tail to next buffer
	
	return 0;
}

// RING BUFFER INIT_______________________________________________________________
void RingBuffer_Init(Ring_Buffer_Data *Ring_Buff, uint8_t *buff, uint16_t len)
{
	Ring_Buff->buffer = buff;
	Ring_Buff->head = 0;
	Ring_Buff->tail = 0;
	Ring_Buff->Max_len = len;
}



