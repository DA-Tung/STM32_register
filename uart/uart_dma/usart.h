#include "stm32f4xx.h"
#include "stm32f407xx.h"

#define Usart_IDLE_IRQ						4U
#define Usart_RxNEIE_IRQ					5U
#define Usart_TCIE_IRQ						6U
#define Usart_TxEIE_IRQ						7U
#define Usart_PEIE_IRQ						8U


void usart_init(USART_TypeDef* USARTx);
void usart_send_string(USART_TypeDef* USARTx, char *str);
void usart_send(USART_TypeDef* USARTx,uint8_t data);
uint8_t usart_receiver(USART_TypeDef* USARTx);
void usart_receiver_string(USART_TypeDef* USARTx, char *str, uint32_t length);
void usart_enable_irq(USART_TypeDef *USARTx, IRQn_Type line_irq, uint32_t message_irq, uint32_t level_priority);




