
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "usart_interrupt.h"

// Declare variable***********************************************************
char rx_data[10];

// Function***********************************************************
void USART2_IRQHandler(void)
{
	usart_receiver_string(USART2, rx_data,8);
}

// Program main***********************************************************
int main()
{

	SysClock_configure();
	
	usart_init(USART2);
	
	usart_enable_irq(USART2,USART2_IRQn, Usart_RxNEIE_IRQ,1);
	
	while(1)
	{

	}
}
