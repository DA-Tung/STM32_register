
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "usart_polling.h"

// Declare variable***********************************************************
char rx_data[10];

// Function***********************************************************

// Program main***********************************************************
int main()
{

	SysClock_configure();
	
	usart_init(USART2);	
	
	while(1)
		
	{
// USART TRAMSMIT-------------------------------------------------------
		usart_send_string(USART2, "hello\n");
	  delay_systick_ms(500);
	
// USART RECEIVE-------------------------------------------------------
		uint8_t str = usart_receiver(USART2);				
		usart_send(USART2, str);	
		delay_systick_ms(50);

		usart_receiver_string(USART2, rx_data, 10);
		delay_systick_ms(500);
	}
}



