#include "stm32f4xx.h"
#include "stm32f407xx.h"

// Define SJW
#define CAN_SJW_1_QUANTA             (uint32_t)0U
#define CAN_SJW_2_QUANTA             (uint32_t)1U 
#define CAN_SJW_3_QUANTA             (uint32_t)2U
#define CAN_SJW_4_QUANTA             (uint32_t)4U

// Define Time Segment 2
#define CAN_TS2_1_QUANTA             (uint32_t)0U
#define CAN_TS2_2_QUANTA             (uint32_t)1U 
#define CAN_TS2_3_QUANTA             (uint32_t)2U
#define CAN_TS2_4_QUANTA             (uint32_t)3U
#define CAN_TS2_5_QUANTA             (uint32_t)4U
#define CAN_TS2_6_QUANTA             (uint32_t)5U 
#define CAN_TS2_7_QUANTA             (uint32_t)6U
#define CAN_TS2_8_QUANTA             (uint32_t)7U

// Define Time Segment 1
#define CAN_TS1_1_QUANTA             (uint32_t)0U
#define CAN_TS1_2_QUANTA             (uint32_t)1U
#define CAN_TS1_3_QUANTA             (uint32_t)2U
#define CAN_TS1_4_QUANTA             (uint32_t)3U
#define CAN_TS1_5_QUANTA             (uint32_t)4U
#define CAN_TS1_6_QUANTA             (uint32_t)5U
#define CAN_TS1_7_QUANTA             (uint32_t)6U
#define CAN_TS1_8_QUANTA             (uint32_t)7U
#define CAN_TS1_9_QUANTA             (uint32_t)8U
#define CAN_TS1_10_QUANTA            (uint32_t)9U
#define CAN_TS1_11_QUANTA            (uint32_t)10U
#define CAN_TS1_12_QUANTA            (uint32_t)11U
#define CAN_TS1_13_QUANTA            (uint32_t)12U
#define CAN_TS1_14_QUANTA            (uint32_t)13U
#define CAN_TS1_15_QUANTA            (uint32_t)14U
#define CAN_TS1_16_QUANTA            (uint32_t)15U

// Define filter mode
#define Filter_mode_ID_Mask          (uint32_t)0U
#define Filter_mode_ID_List          (uint32_t)1U

// Define filter scale
#define Filter_scale_16bit           (uint32_t)0U
#define Filter_scale_32bit           (uint32_t)1U

// Define FIFO assignment
#define Filter_FIFO_0                (uint32_t)0U
#define Filter_FIFO_1                (uint32_t)1U

// Define identifer mode
#define Identifer_standard           (uint32_t)0U
#define Identifer_extended           (uint32_t)1U

// Define identifer mode
#define Data_frame                   (uint32_t)0U
#define Remote_frame                 (uint32_t)1U

// Define FIFO
#define Rx_FIFO_0										 (uint32_t)0U
#define Rx_FIFO_1										 (uint32_t)1U	

// Define Interrupt
#define can_irq_tx_mailbox_empty     (uint32_t)0U

#define can_irq_Rx0_fifo_msg_pending (uint32_t)1U
#define can_irq_Rx0_fifo_full        (uint32_t)2U
#define can_irq_Rx0_fifo_over        (uint32_t)3U

#define can_irq_Rx1_fifo_msg_pending (uint32_t)4U
#define can_irq_Rx1_fifo_full        (uint32_t)5U
#define can_irq_Rx1_fifo_over        (uint32_t)6U

typedef struct
{
	uint32_t CAN_SJW_NUM;
	uint32_t CAN_TS2_NUM;
	uint32_t CAN_TS1_NUM;
	uint32_t CAN_BR_NUM;
}timing_para;

typedef struct
{
	uint32_t Filter_bank_num;
	uint32_t Filter_FIFO_assign;
//	uint32_t Filter_activation;
	uint32_t Filter_mode;
	uint32_t Filter_scale;
  uint32_t Filter_ID_high;
	uint32_t filter_ID_low;
	uint32_t filter_mask_high;
	uint32_t filter_mask_low;	
}filter_config_para;

typedef struct
{
	uint32_t Standard_ID;
	uint32_t Extended_ID;
	uint32_t Identifer_type;
	uint32_t request_frame;
	uint32_t data_length;
	uint32_t filter_match;
	uint32_t time_global;
	uint32_t time_stamp;
}can_Tx_message;

typedef struct
{
	uint32_t Standard_ID;
	uint32_t Extended_ID;
	uint32_t Identifer_type;
	uint32_t request_frame;
	uint32_t data_length;
	uint32_t filter_match;
	uint32_t time_global;
	uint32_t time_stamp;
}can_Rx_message;

void can_config_reg(CAN_TypeDef *CANx, timing_para para_timing);
void can_config_filter(CAN_TypeDef *CANx, filter_config_para filter_para);
void can_transmit_message(CAN_TypeDef *CANx, can_Tx_message message_para_tx,uint8_t *data_tx, uint32_t *Tx_mailbox);
void can_receive_message(CAN_TypeDef *CANx, can_Rx_message *message_para_rx,uint8_t *data_rx, uint32_t rx_fifo);
void can_enable_irq(CAN_TypeDef *CANx, IRQn_Type line_irq, uint32_t message_irq, uint32_t level_priority);



