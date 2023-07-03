#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "can.h"

// CAN CONFIG_______________________________________________________________
void can_config_reg(CAN_TypeDef *CANx, timing_para para_timing)
{
	// Enable GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	
	// Select mode alternate funcion
	GPIOB->MODER |= GPIO_MODER_MODE9_1 | GPIO_MODER_MODE8_1;
	
	// Config out speed very high
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR8;
	
	// No pull up and pull down
	GPIOB->PUPDR |= 0;
	
	// Select alternate funcion high register CAN_Tx = PB9, CAN_Rx = PB8
	GPIOB->AFR[1] |= (GPIO_AFRH_AFSEL9_0 | GPIO_AFRH_AFSEL9_3) | (GPIO_AFRH_AFSEL8_0 | GPIO_AFRH_AFSEL8_3);
	
	// Enable CAN bus
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN | RCC_APB1ENR_CAN2EN;
	
	//Select Initialization mode
	CANx->MCR |= CAN_MCR_INRQ;
	
	// Wait bxCan confirm mode initialization
	while(!(CANx->MSR & CAN_MSR_INAK));
	
	// Leave Sleep mode
	CANx->MCR &= ~CAN_MCR_SLEEP;
	
	// Wait confirm state acknowledge of Sleep mode
	while(CANx->MSR & CAN_MSR_SLAK);
	
	// Disenable can during debug
	CANx->MCR |= CAN_MCR_DBF;
	
	// Disenable Time Trigger communication
	CANx->MCR &= ~CAN_MCR_TTCM;
	
	// Off the bus-off state
	CANx->MCR &= ~CAN_MCR_ABOM;
	
	// Disable auto retransmission
	CANx->MCR |= CAN_MCR_NART;
	
	// Locked on Receive FIFO
	CANx->MCR &= ~CAN_MCR_RFLM;
	
	// Priority driven by the identifier the message
	CANx->MCR &= ~CAN_MCR_TXFP;
	
	// Disable Silent and Loop back mode
	CANx->BTR &= ~(CAN_BTR_SILM | CAN_BTR_LBKM);
	
	// Config parameter timing
	CANx->BTR = 0;
	CANx->BTR |= (para_timing.CAN_SJW_NUM) << 24;   // Config SJW
	CANx->BTR |= (para_timing.CAN_TS2_NUM) << 20;   // Config TS2
	CANx->BTR |= (para_timing.CAN_TS1_NUM) << 16;   // Config TS1
	CANx->BTR |= (para_timing.CAN_BR_NUM) << 0;     // Config Baudrate
	
	// Enable normal mode
	CANx->MCR &= ~(CAN_MCR_SLEEP | CAN_MCR_INRQ);
}

// CONFIG FILTER_______________________________________________________________
void can_config_filter(CAN_TypeDef *CANx, filter_config_para filter_para)
{
	uint32_t number_filter_bank;
	
	// Use all the filter CAN
	CANx->FMR |= CAN_FMR_CAN2SB;	
	
	// Select filter mode
	CANx->FMR |= CAN_FMR_FINIT;
	
	// Select number filter bank
	number_filter_bank = filter_para.Filter_bank_num;

	// Deactivation filter 
	CANx->FA1R &= ~(1 << number_filter_bank);
	
	// Select scale 16 bit
	if(filter_para.Filter_scale == Filter_scale_16bit)
	{
		CANx->FS1R &= ~(1 << number_filter_bank);
		
		CANx->sFilterRegister[number_filter_bank].FR1 = ((filter_para.filter_mask_low << 5) << 16) | (filter_para.filter_ID_low);
		
		CANx->sFilterRegister[number_filter_bank].FR2 = ((filter_para.filter_mask_high << 5) << 16) | (filter_para.Filter_ID_high);
	}
	
	// Select scale 32 bit	
	else if(filter_para.Filter_scale == Filter_scale_32bit)
	{
		CANx->FS1R |= 1 << number_filter_bank;
		
		CANx->sFilterRegister[number_filter_bank].FR1 = ((filter_para.Filter_ID_high << 5) << 16) | (filter_para.filter_ID_low);
		
		CANx->sFilterRegister[number_filter_bank].FR2 = ((filter_para.filter_mask_high << 5) << 16) | (filter_para.filter_mask_low);
	}
	
	//Select Identifier mode
	if(filter_para.Filter_mode == Filter_mode_ID_Mask)
	{
		CANx->FM1R &= ~(1 << number_filter_bank);
	}
	else if(filter_para.Filter_mode == Filter_mode_ID_List)
	{
		CANx->FM1R |= 1 << number_filter_bank;
	}
	
	// select FIFO number
	if(filter_para.Filter_FIFO_assign == Filter_FIFO_0)
	{
		CANx->FFA1R &= ~(1 << number_filter_bank);
	}
	else if(filter_para.Filter_FIFO_assign == Filter_FIFO_1)
	{
		CANx->FFA1R |= 1 << number_filter_bank;
	}
	
	// Active number filter
	CANx->FA1R |= 1 << number_filter_bank;		
	
	// Select normal mode
	CANx->FMR &= ~CAN_FMR_FINIT;	
}	

// TRANSMIT MESSAGE_______________________________________________________________
void can_transmit_message(CAN_TypeDef *CANx, can_Tx_message message_para_tx, uint8_t *data_tx, uint32_t *Tx_mailbox)
{
	uint32_t number_mailbox;
	uint32_t code_mailbox;
	
	// Select mailbox empty
	code_mailbox = CANx->TSR;
	
	if((code_mailbox & CAN_TSR_TME0) || (code_mailbox & CAN_TSR_TME1) || (code_mailbox & CAN_TSR_TME2))
	{
		number_mailbox = (code_mailbox & CAN_TSR_CODE) >> 24;
	}
	if(number_mailbox > 2U)
	{
		*Tx_mailbox = 1 << number_mailbox;
	}
	
	//Seclect type Identify
	if(message_para_tx.Identifer_type == Identifer_standard)
	{
		CANx->sTxMailBox[number_mailbox].TIR |= message_para_tx.Standard_ID << 21;
		CANx->sTxMailBox[number_mailbox].TIR |= Identifer_standard << 2;
	}
	else 	if(message_para_tx.Identifer_type == Identifer_extended)
	{
		CANx->sTxMailBox[number_mailbox].TIR |= message_para_tx.Extended_ID << 12;
		CANx->sTxMailBox[number_mailbox].TIR |= Identifer_extended << 2;
	}
	
	// Select request frame
	if(message_para_tx.request_frame == Data_frame)
	{
		CANx->sTxMailBox[number_mailbox].TIR |= Data_frame << 1;
	}
	else if(message_para_tx.request_frame == Remote_frame)
	{
		CANx->sTxMailBox[number_mailbox].TIR |= Remote_frame << 1;
	}
	
	// Set time stamp
	CANx->sTxMailBox[number_mailbox].TDTR |= message_para_tx.time_global << 8;
	
	// Set length data
	CANx->sTxMailBox[number_mailbox].TDTR |= message_para_tx.data_length << 0;
	
	// Write data
	CANx->sTxMailBox[number_mailbox].TDLR = ((uint32_t)data_tx[0] << 0)  | 
											((uint32_t)data_tx[1] << 8)  |
											((uint32_t)data_tx[2] << 16) | 
											((uint32_t)data_tx[3] << 24);
			
	CANx->sTxMailBox[number_mailbox].TDHR = ((uint32_t)data_tx[4] << 0)  | 
											((uint32_t)data_tx[5] << 8)  |
											((uint32_t)data_tx[6] << 16) | 
											((uint32_t)data_tx[7] << 24);
	
	// request transmit
	CANx->sTxMailBox[number_mailbox].TIR |= 1 << 0;	
}

// RECEIVE MESSAGE_______________________________________________________________
void can_receive_message(CAN_TypeDef *CANx, can_Rx_message *message_para_rx, uint8_t *data_rx, uint32_t rx_fifo)
{
	uint32_t num_message_pending;
	
	// Check FIFO
	if(rx_fifo == Rx_FIFO_0) num_message_pending = (CANx->RF0R & CAN_RF0R_FMP0);
	else 	if(rx_fifo == Rx_FIFO_1) num_message_pending = (CANx->RF0R & CAN_RF1R_FMP1);
	
	if(num_message_pending > 0U)
	{
		// Get data type Identifer
		message_para_rx->Identifer_type = (CANx->sFIFOMailBox[rx_fifo].RIR & CAN_RI0R_IDE) >> 2;
		
		//Get data type Identifer
		if(message_para_rx->Identifer_type == Identifer_standard)
		{
			message_para_rx->Standard_ID = (CANx->sFIFOMailBox[rx_fifo].RIR & CAN_RI0R_STID) >> 21;
		}
		else if(message_para_rx->Identifer_type == Identifer_extended)
		{
			message_para_rx->Extended_ID = (CANx->sFIFOMailBox[rx_fifo].RIR & CAN_RI0R_EXID) >> 12;			
		}	
		
		// Get data frame
		message_para_rx->request_frame = (CANx->sFIFOMailBox[rx_fifo].RIR  & CAN_RI0R_RTR) >> 1;
		
		// Check filter match
		message_para_rx->filter_match = (CANx->sFIFOMailBox[rx_fifo].RDTR & CAN_RDT0R_FMI) >> 8;		
		
		// Receive time stamp
		message_para_rx->time_stamp = (CANx->sFIFOMailBox[rx_fifo].RDTR & CAN_RDT0R_TIME) >> 16;
		
		// Read data length
		message_para_rx->data_length = (CANx->sFIFOMailBox[rx_fifo].RDTR & CAN_RDT0R_DLC) >> 0;

		// Get data
		data_rx[0] = (CANx->sFIFOMailBox[rx_fifo].RDLR & CAN_RDL0R_DATA0) >> 0;
		data_rx[1] = (CANx->sFIFOMailBox[rx_fifo].RDLR & CAN_RDL0R_DATA1) >> 8;
		data_rx[2] = (CANx->sFIFOMailBox[rx_fifo].RDLR & CAN_RDL0R_DATA2) >> 16;
		data_rx[3] = (CANx->sFIFOMailBox[rx_fifo].RDLR & CAN_RDL0R_DATA3) >> 24;		
		data_rx[4] = (CANx->sFIFOMailBox[rx_fifo].RDHR & CAN_RDH0R_DATA4) >> 0;
		data_rx[5] = (CANx->sFIFOMailBox[rx_fifo].RDHR & CAN_RDH0R_DATA5) >> 8;
		data_rx[6] = (CANx->sFIFOMailBox[rx_fifo].RDHR & CAN_RDH0R_DATA6) >> 16;
		data_rx[7] = (CANx->sFIFOMailBox[rx_fifo].RDHR & CAN_RDH0R_DATA7) >> 24;			

		// Release FIFO
		if(rx_fifo == Rx_FIFO_0)  CANx->RF0R |= CAN_RF0R_RFOM0;
		else if(rx_fifo == Rx_FIFO_1)  CANx->RF1R |= CAN_RF1R_RFOM1;
	}
	
	else
		return;
}

// ENABLE INTERRUPT_______________________________________________________________
void can_enable_irq(CAN_TypeDef *CANx, IRQn_Type line_irq, uint32_t message_irq, uint32_t level_priority)
{
	CANx->IER |= (1 << message_irq);
	
	NVIC_SetPriority(line_irq, level_priority);
	
	NVIC_EnableIRQ(line_irq);
}





