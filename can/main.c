
// System***********************************************************
#include "string.h"
#include "math.h"

// Library***********************************************************
#include "systick.h"
#include "gpio.h"
#include "can.h"
#include "interrupt.h"

// Declare variable***********************************************************
timing_para            data_timing;
filter_config_para     data_filter;

can_Tx_message         Tx_msg;
can_Rx_message         Rx_msg;

uint8_t tx_data[8];
uint8_t rx_data[8];

uint32_t tx_mailbox;

int datacheck = 0;

irq_config para_irq_handler;

// Function***********************************************************
void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PR14)
	{
		EXTI->PR |= EXTI_PR_PR14;
		
		tx_data[0] = 50;
		tx_data[1] = 100;
			
		can_transmit_message(CAN1, Tx_msg, tx_data, &tx_mailbox); 
	}
}

void CAN1_RX0_IRQHandler(void)
{
	if(CAN1->IER & CAN_IER_FMPIE0)
	{
		if(CAN1->RF0R & CAN_RF0R_FMP0)
		{
			can_receive_message(CAN1, &Rx_msg, rx_data, Rx_FIFO_0);
			if(Rx_msg.Standard_ID == 0x103)
			{
				datacheck = 1;
			}
		}
	}
}

// Program main***********************************************************
int main()
{

	SysClock_configure();
	
	para_irq_handler.exti_port = SYSCFG_EXTICR4_EXTI14_PC;
	para_irq_handler.trigger_level = rising_trigger;
	para_irq_handler.irq_type = EXTI15_10_IRQn;
	para_irq_handler.priority_level_irq = 0;
	interrupt_config(GPIOC, PIN14, para_irq_handler);

	data_timing.CAN_BR_NUM = 20;
	data_timing.CAN_SJW_NUM = CAN_SJW_1_QUANTA;
	data_timing.CAN_TS1_NUM = CAN_TS1_5_QUANTA;
	data_timing.CAN_TS2_NUM = CAN_TS2_2_QUANTA;	
	can_config_reg(CAN1,data_timing);
	
	data_filter.Filter_bank_num = 4;
	data_filter.Filter_FIFO_assign = Filter_FIFO_0;
	data_filter.Filter_scale = Filter_scale_32bit;
	data_filter.filter_mask_low = 0;
	data_filter.filter_mask_high = 0x103;
	data_filter.filter_ID_low = 0;
	data_filter.Filter_ID_high = 0x103;
	data_filter.Filter_mode = Filter_mode_ID_Mask;	
	can_config_filter(CAN1, data_filter);

	can_enable_irq(CAN1, CAN1_RX0_IRQn, can_irq_Rx0_fifo_msg_pending, 0);
	
	Tx_msg.Identifer_type = Identifer_standard;
	Tx_msg.request_frame = Data_frame;
	Tx_msg.data_length = 2;
	Tx_msg.Standard_ID = 0x407;	
	
	while(1)
	{
		
// CAN TRANSMIT/RECEIVE-------------------------------------------------------
		if(datacheck == 1)
		{
			gpio_output(GPIOD, PORTD, PIN12, PIN_SET);
			gpio_output(GPIOD, PORTD, PIN13, PIN_RESET);	
			delay_systick_ms(2000);
			datacheck = 0;
			tx_data[0] = 50;
			tx_data[1] = 100;			
			can_transmit_message(CAN1, Tx_msg, tx_data, &tx_mailbox);
		}
		else if(datacheck == 0)
		{
			gpio_output(GPIOD, PORTD, PIN13, PIN_SET);
			gpio_output(GPIOD, PORTD, PIN12, PIN_RESET);
			gpio_output(GPIOD, PORTD, PIN15, PIN_TOGGLE);		
			delay_systick_ms(1000);
		}
		
	}
}

