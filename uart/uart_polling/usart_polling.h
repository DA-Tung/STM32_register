#include "stm32f4xx.h"
#include "stm32f407xx.h"

void usart_init(USART_TypeDef* USARTx);
void usart_send_string(USART_TypeDef* USARTx, char *str);
void usart_send(USART_TypeDef* USARTx,uint8_t data);
uint8_t usart_receiver(USART_TypeDef* USARTx);
void usart_receiver_string(USART_TypeDef* USARTx, char *str, uint32_t length);





