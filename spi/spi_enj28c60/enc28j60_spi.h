#include "stm32f4xx.h"
#include "stm32f407xx.h"

#define ADDR_MASK        				0x1F

// Opcode
#define ENC28J60_RCR					0U				// Read control register 
#define ENC28J60_RBM					1U 				// Read buffer memory
#define ENC28J60_WCR					2U				// write control register 
#define ENC28J60_WBM					3U 				// write buffer memory
#define ENC28J60_BFS					4U				// Bit field set
#define ENC28J60_BFC					5U 				// Bit field clear
#define ENC28J60_SRC					7U				// System reset command 

#define RBM_Byte_0						0x3A			// Byte 0 read buffer memory
#define WBM_Byte_0						0x7A			// Byte 0 write buffer memory

// Bank number
#define ENC28J60_BANK_0					0U				// Bank 0
#define ENC28J60_BANK_1					1U				// Bank 1
#define ENC28J60_BANK_2					2U				// Bank 2
#define ENC28J60_BANK_3					3U				// Bank 3

// Register bank common
#define BANK_EIE						0x1B			
#define BANK_EIR						0x1C
#define BANK_ESTAT						0x1D
#define BANK_ECON2						0x1E
#define BANK_ECON1						0x1F

// Register bank 0
#define BANK_0_ERDPTL					0x00
#define BANK_0_ERDPTH					0x01
#define BANK_0_EWRPTL					0x02
#define BANK_0_EWRPTH					0x03
#define BANK_0_ETXSTL					0x04
#define BANK_0_ETXSTH					0x05
#define BANK_0_ETXNDL					0x06
#define BANK_0_ETXNDH					0x07
#define BANK_0_ERXSTL					0x08
#define BANK_0_ERXSTH					0x09
#define BANK_0_ERXNDL					0x0A
#define BANK_0_ERXNDH					0x0B
#define BANK_0_ERXRDPTL					0x0C
#define BANK_0_ERXRDPTH					0x0D
#define BANK_0_ERXWRPTL					0x0E
#define BANK_0_ERXWRPTH					0x0F
#define BANK_0_EDMASTL					0x10
#define BANK_0_EDMASTH					0x11
#define BANK_0_EDMANDL					0x12
#define BANK_0_EDMANDH					0x13
#define BANK_0_EDMADSTL					0x14
#define BANK_0_EDMADSTH					0x15
#define BANK_0_EDMACSL					0x16
#define BANK_0_EDMACSH					0x17

// Register bank 1
#define BANK_1_EHT0 					0x00 
#define BANK_1_EHT1 					0x01 
#define BANK_1_EHT2 					0x02
#define BANK_1_EHT3 					0x03
#define BANK_1_EHT4 					0x04
#define BANK_1_EHT5 					0x05
#define BANK_1_EHT6 					0x06
#define BANK_1_EHT7 					0x07
#define BANK_1_EPMM0 					0x08
#define BANK_1_EPMM1 					0x09
#define BANK_1_EPMM2 					0x0A
#define BANK_1_EPMM3 					0x0B
#define BANK_1_EPMM4 					0x0C
#define BANK_1_EPMM5 					0x0D
#define BANK_1_EPMM6 					0x0E
#define BANK_1_EPMM7 					0x0F
#define BANK_1_EPMCSL					0x10
#define BANK_1_EPMCSH					0x11
#define BANK_1_EPMOL 					0x14
#define BANK_1_EPMOH 					0x15
#define BANK_1_ERXFCON					0x18
#define BANK_1_EPKTCNT 					0x19

// Register bank 2
#define BANK_2_MACON1 					0x00 | 0x80
#define BANK_2_MACON3 					0x02 | 0x80
#define BANK_2_MACON4 					0x03 | 0x80
#define BANK_2_MABBPIG 					0x04 | 0x80
#define BANK_2_MAIPGL 					0x06 | 0x80
#define BANK_2_MAIPGH 					0x07 | 0x80
#define BANK_2_MACLCON1					0x08 | 0x80
#define BANK_2_MACLCON2 				0x09 | 0x80
#define BANK_2_MAMXFLL 					0x0A | 0x80
#define BANK_2_MAMXFLH 					0x0B | 0x80
#define BANK_2_MICMD  					0x12 | 0x80
#define BANK_2_MIREGADR					0x14 | 0x80
#define BANK_2_MIWRL  					0x16 | 0x80
#define BANK_2_MIWRH  					0x17 | 0x80
#define BANK_2_MIRDL  					0x18 | 0x80
#define BANK_2_MIRDH  					0x19 | 0x80

// Register bank 3
#define BANK_3_MAADR5 					0x00 | 0x80
#define BANK_3_MAADR6 					0x01 | 0x80
#define BANK_3_MAADR3 					0x02 | 0x80
#define BANK_3_MAADR4 					0x03 | 0x80
#define BANK_3_MAADR1 					0x04 | 0x80
#define BANK_3_MAADR2 					0x05 | 0x80
#define BANK_3_MISTAT					0x0A | 0x80
#define BANK_3_EREVID					0x12
#define BANK_3_ECOCON					0x15

// *** Note : BANK2,BANK3 or 0x80 to verify MAC,MI 

// Register PHY
#define PHY_PHCON1						0
#define PHY_PHSTAT1						0x01
#define PHY_PHCID1						0x02
#define PHY_PHCID2						0x03
#define PHY_PHCON2						0x10
#define PHY_PHSTAT2						0x11
#define PHY_PHIE			  			0x12
#define PHY_PHIR	   					0x13
#define PHY_PHLCON						0x14


#define ERX_Start						0U
#define ERX_End							0x0FFFU
#define ETX_Start						0x1000U
#define ETX_End							0x1FFFU

#define max_frame_length				0x05DC

#define BtB_packet_haft   	    		0x12
#define BtB_packet_full					0x15

typedef struct
{
	uint16_t P_Rx_start;
	uint16_t P_Rx_end;
	uint16_t P_Tx_start;
	uint16_t P_Tx_end;
} pointer_Rx_Tx;

uint8_t enc28j60_readwrite(uint8_t reg_rw);

void enc28j60_set_bank(uint8_t bank_addr);

void enc28j60_clear_reg(uint8_t addr_reg, uint8_t value_clear);
void enc28j60_set_reg(uint8_t addr_reg, uint8_t value_set);

void enc28j60_write_ctrl_reg(uint8_t addr_reg, uint8_t value_reg);
uint8_t enc28j60_read_ctrl_reg(uint8_t addr_reg);

void enc28j60_write_reg_bank(uint8_t addr_reg, uint8_t num_bank, uint8_t value_buffer);
uint8_t enc28j60_read_reg_bank(uint8_t addr_reg, uint8_t num_bank);

void enc28j60_write_buffer_memory(uint8_t *data_buffer, uint16_t length);
uint8_t enc28j60_read_buffer_memory(void);

void enc28j60_system_soft_reset(void);

void enc28j60_write_phy(uint8_t value_reg, uint16_t data_phy);
uint16_t enc28j60_read_phy(uint8_t value_reg);

void enc28j60_set_clk(uint8_t clk);

void enc28j60_init(void);
void read_MAC_addr(uint8_t *addr_MAC);

void enc28j60_write_packet(uint16_t start_addr, uint16_t length, uint8_t *data_packet_write);
uint16_t enc28j60_read_packet(uint8_t *data_packet_read);

uint8_t enc28j60_getrev(void);
