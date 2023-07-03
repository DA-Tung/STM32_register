
#include "stm32f4xx.h"
#include "stm32f407xx.h"

//define command word
#define R_register						0x00
#define W_register						0x20
#define R_Rx_Payload      		0x61
#define W_Tx_Payload      		0xA0
#define Flush_Tx							0xE1
#define Flush_Rx          		0xE2
#define Reuse_Tx_PL	      		0xE3
#define Activate          		0x50
#define R_Rx_PL_WID       		0x60
#define W_Ack_Payload         0xA1
#define W_Tx_Payload_NoAck		0xB0
#define NOP										0xFF

// Define register
#define CONFIG								0x00
#define EN_AA									0x01
#define EN_RxAddr							0x02
#define Setup_AW							0x03
#define Setup_Retr						0x04
#define RF_CH                 0x05
#define RF_setup							0x06
#define Status								0x07
#define Observe_Tx						0x08
#define CD										0x09
#define Rx_Addr_P0						0x0A
#define Rx_addr_P1						0x0B
#define Rx_Addr_P2						0x0C
#define Rx_addr_P3						0x0D
#define Rx_Addr_P4						0x0E
#define Rx_addr_P5						0x0F
#define Tx_Addr								0x10
#define Rx_PW_P0							0x11
#define Rx_PW_P1							0x12
#define Rx_PW_P2							0x13
#define Rx_PW_P3							0x14
#define Rx_PW_P4							0x15
#define Rx_PW_P5							0x16
#define FIFO_Status						0x17
#define DYNPD									0x1C
#define FEATURE								0x1D




static uint8_t NRF24L01_readwrite(uint8_t rw_addr);
uint8_t NRF24L01_read(uint8_t reg);
void NRF24L01_write(uint8_t reg, uint8_t data);
void NRF24L01_Init(void);
void NRF24L01_Flush_Tx(void);
void NRF24L01_Flush_Rx(void);
void NRF24L01_Rx_mode(void);
void NRF24L01_Tx_mode(void);




