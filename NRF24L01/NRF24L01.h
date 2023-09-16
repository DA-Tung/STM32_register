#ifndef _NRF24L01_H_
#define _NRF24L01_H_

#include "stm32f4xx.h"
#include "stm32f407xx.h"

//define command word
#define R_register						0x00
#define W_register						0x20
#define R_Rx_Payload      				0x61
#define W_Tx_Payload      				0xA0
#define Flush_Tx						0xE1
#define Flush_Rx          				0xE2
#define Reuse_Tx_PL	      				0xE3
#define Activate          				0x50
#define R_Rx_PL_WID       				0x60
#define W_Ack_Payload         			0xA1
#define W_Tx_Payload_NoAck				0xB0
#define NOP								0xFF

// Define register
#define CONFIG							0x00
#define EN_AA							0x01
#define EN_RXADDR						0x02
#define SETUP_AW						0x03
#define SETUP_RETR						0x04
#define RF_CH                 			0x05
#define RF_SETUP						0x06
#define STATUS							0x07
#define OBSERVE_TX						0x08
#define CD								0x09
#define RX_ADDR_P0						0x0A
#define RX_ADDR_P1						0x0B
#define RX_ADDR_P2						0x0C
#define RX_ADDR_P3						0x0D
#define RX_ADDR_P4						0x0E
#define RX_ADDR_P5						0x0F
#define TX_ADDR							0x10
#define RX_PW_P0						0x11
#define RX_PW_P1						0x12
#define RX_PW_P2						0x13
#define RX_PW_P3						0x14
#define RX_PW_P4						0x15
#define RX_PW_P5						0x16
#define FIFO_STATUS						0x17
#define DYNPD							0x1C
#define FEATURE							0x1D

typedef struct
{
	uint8_t ERX_Pipe;
	uint8_t ERX_Addr;
	uint8_t Rx_PW_Pipe;
}Rx_Mode_Data;

static uint8_t NRF24L01_readwrite(uint8_t rw_addr);

uint8_t NRF24L01_read_data(uint8_t cmd);
void NRF24L01_read_multi_data(uint8_t cmd, uint8_t *buffer_data, uint16_t length);

void NRF24L01_write_data(uint8_t cmd, uint8_t data);
void NRF24L01_write_multi_data(uint8_t cmd, uint8_t *buffer_data, uint16_t length);

void NRF24L01_Flush_Tx(void);
void NRF24L01_Flush_Rx(void);

void NRF24L01_Init(void);

void NRF24L01_Rx_mode(uint8_t *addr, uint8_t channel, Rx_Mode_Data Rx_Mode);
uint8_t NRF24L01_Check_PipeNum(uint8_t PipeNum);
void NRF24L01_Receive(uint8_t *buffer_data);

void NRF24L01_Tx_mode(uint8_t *addr, uint8_t channel);
uint8_t NRF24L01_Transmit(uint8_t *buffer_data);

#endif // _NRF24L01_H_


