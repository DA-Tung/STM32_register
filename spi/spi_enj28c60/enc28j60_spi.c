#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "spi.h"
#include "gpio.h"
#include "enc28j60_spi.h"
#include "systick.h"


#define ENC28J60_start				gpio_output(GPIOA,PIN4,PIN_RESET)
#define ENC28J60_terminate			gpio_output(GPIOA,PIN4,PIN_SET)

// ENC28J60 Read/write_______________________________________________________________
uint8_t enc28j60_readwrite(uint8_t reg_rw)
{
	uint8_t value_rw;
	
	// Read and write data
	spi_readwrite(SPI1, (uint8_t *)&reg_rw, (uint8_t *)&value_rw, 1);
	
	return value_rw;
}

// ENC28J60 Clear data register ECON1_______________________________________________________________
void enc28j60_clear_reg(uint8_t addr_reg, uint8_t value_clear)
{
	uint8_t buffer_clear[2];
	
	// Start frame
	ENC28J60_start;
	
	// Bit field clear
	buffer_clear[0] = (ENC28J60_BFC << 5)|(addr_reg&0x1F);
	enc28j60_readwrite(buffer_clear[0]);
	
	// Send data to clear register
	buffer_clear[1] = value_clear;
	enc28j60_readwrite(buffer_clear[1]);
	
	// End frame	
	ENC28J60_terminate;	
}

// ENC28J60 Set register ECON1_______________________________________________________________
void enc28j60_set_reg(uint8_t addr_reg, uint8_t value_set)
{
	uint8_t buffer_set[2];
	
	// Start frame
	ENC28J60_start;
	
	// Bit field Set
	buffer_set[0] = (ENC28J60_BFS << 5) | addr_reg;
	enc28j60_readwrite(buffer_set[0]);
	
	// Send data to set register
	buffer_set[1] =  value_set;
	enc28j60_readwrite(buffer_set[1]);
	
	// End frame		
	ENC28J60_terminate;	
}

// ENC28J60 Set bank number_______________________________________________________________
void enc28j60_set_bank(uint8_t bank_num)
{
	// Clear value bank register ECON1
	enc28j60_clear_reg(BANK_ECON1, 0x03);
	
	// Select Bank
	enc28j60_set_reg(BANK_ECON1, bank_num);
}

// ENC28J60 write register common_______________________________________________________________
void enc28j60_write_ctrl_reg(uint8_t addr_reg, uint8_t value_reg)
{
	uint8_t buffer_write[2];
	
	// Start frame
	ENC28J60_start;
	
	// write control register
	buffer_write[0] = (ENC28J60_WCR << 5)|(addr_reg&0x1F);
	enc28j60_readwrite(buffer_write[0]);
	
	// Write data for register
	buffer_write[1] = value_reg;
	enc28j60_readwrite(buffer_write[1]);
	
	// End frame
	ENC28J60_terminate;
}

// ENC28J60 Read register common_______________________________________________________________
uint8_t enc28j60_read_ctrl_reg(uint8_t addr_reg)
{
	uint8_t buffer_read[2];
	
	// Start frame	
	ENC28J60_start;
	
	// Read control register
	buffer_read[0] = (ENC28J60_RCR << 5)|(addr_reg&0x1F);
	
	// write data to verify funcion
	enc28j60_readwrite(buffer_read[0]);
	
	// Verify sequence are MAC and MI resgister or NOT?
	if(addr_reg & 0x80)
	{
		enc28j60_readwrite(0); 						// read dummy byte when resgiter related to MAC,MI
	}
	
	// read data
	enc28j60_readwrite(buffer_read[1]);
	
	// End frame	
	ENC28J60_terminate;
	
	return buffer_read[1];
}

// ENC28J60 write register bank, value register_______________________________________________________________
void enc28j60_write_reg_bank(uint8_t bank_reg, uint8_t num_bank, uint8_t value_bank)
{
	// Set bank
	enc28j60_set_bank(num_bank);
	
	// write register bank
	enc28j60_write_ctrl_reg(bank_reg, value_bank);
}

// ENC28J60 Read register bank, value register_______________________________________________________________
uint8_t enc28j60_read_reg_bank(uint8_t bank_reg, uint8_t num_bank)
{
	uint8_t value_bank_reg = 0;
	
	// Set bank	
	enc28j60_set_bank(num_bank);
	
	// read register bank
	value_bank_reg = enc28j60_read_ctrl_reg(bank_reg);
	
	// Return value
	return value_bank_reg;
}

// ENC28J60 write buffer memory_______________________________________________________________
void enc28j60_write_buffer_memory(uint8_t *data_buffer, uint16_t length)
{	
	// Start frame	
	ENC28J60_start;
	
	// write buffer memory
	enc28j60_readwrite(WBM_Byte_0);
	
	// write all data
	for(int i = 0; i < length - 1; i++)
	{
		enc28j60_readwrite(data_buffer[i]);
	}
	
	// End frame	
	ENC28J60_terminate;
}

// ENC28J60 Read buffer memory_______________________________________________________________
uint8_t enc28j60_read_buffer_memory(void)
{
	uint8_t read_buffer_data = 0;

	// Start frame		
	ENC28J60_start;
	
	// write data to verify funcion
	enc28j60_readwrite(RBM_Byte_0);	
	
	// read data;
	enc28j60_readwrite(read_buffer_data);
	
	// End frame		
	ENC28J60_terminate;
	
	// Return data
	return read_buffer_data;
}

// ENC28J60 Systerm soft reset_______________________________________________________________
void enc28j60_system_soft_reset(void)
{
	uint8_t reset_data;
	
	// System reset soft
	reset_data = (ENC28J60_SRC << 5) | 0x1F;
	
	// Start Frame
	ENC28J60_start;
	
	// write command
	enc28j60_readwrite(reset_data);
	
	// End Frame
	ENC28J60_terminate;
}

// ENC28J60 PHY write_______________________________________________________________
void enc28j60_write_phy(uint8_t value_reg, uint16_t data_phy)
{
	// write the address PHY
	enc28j60_write_reg_bank(BANK_2_MIREGADR, ENC28J60_BANK_2, value_reg);
	
	uint8_t phy_data_l = data_phy & 0x00FF;
	uint8_t phy_data_h = data_phy >> 8;
	
	// write data PHY
	enc28j60_write_reg_bank(BANK_2_MIWRL, ENC28J60_BANK_2, phy_data_l);
	enc28j60_write_reg_bank(BANK_2_MIWRH, ENC28J60_BANK_2, phy_data_h);
	
	while(enc28j60_read_reg_bank(BANK_3_MISTAT, ENC28J60_BANK_3) & 0x01);
}

// ENC28J60 PHY Read_______________________________________________________________
uint16_t enc28j60_read_phy(uint8_t value_reg)
{
	uint16_t data_phy;
	uint8_t read_phy[2];
	
	enc28j60_write_reg_bank(BANK_2_MIREGADR, ENC28J60_BANK_2, value_reg);
	
	enc28j60_write_reg_bank(BANK_2_MICMD,ENC28J60_BANK_2, 0x01);
	
	while(enc28j60_read_reg_bank(BANK_3_MISTAT, ENC28J60_BANK_3) & 0x01);
	
	enc28j60_write_reg_bank(BANK_2_MICMD,ENC28J60_BANK_2, 0);
	
	read_phy[0] = enc28j60_read_reg_bank(BANK_2_MIRDL, ENC28J60_BANK_2);
	read_phy[1] = enc28j60_read_reg_bank(BANK_2_MIRDH, ENC28J60_BANK_2);
	
	data_phy = read_phy[0] + (read_phy[1] << 8);
	
	return data_phy;
}

// ENC28J60 read MAC address_______________________________________________________________
void read_MAC_addr(uint8_t *addr_MAC)
{
	addr_MAC[0] = enc28j60_read_reg_bank(BANK_3_MAADR1, ENC28J60_BANK_3);
	addr_MAC[1] = enc28j60_read_reg_bank(BANK_3_MAADR2, ENC28J60_BANK_3);
	addr_MAC[2] = enc28j60_read_reg_bank(BANK_3_MAADR3, ENC28J60_BANK_3);
	addr_MAC[3] = enc28j60_read_reg_bank(BANK_3_MAADR4, ENC28J60_BANK_3);
	addr_MAC[4] = enc28j60_read_reg_bank(BANK_3_MAADR5, ENC28J60_BANK_3);
	addr_MAC[5] = enc28j60_read_reg_bank(BANK_3_MAADR6, ENC28J60_BANK_3);		
}

// ENC28J60 Set clock output_______________________________________________________________
void enc28j60_set_clk(uint8_t clk)
{
	// Setup clock output resgiter
	enc28j60_write_reg_bank(BANK_3_ECOCON,ENC28J60_BANK_3, clk&0x07);
}

// ENC28J60 Init_______________________________________________________________
void enc28j60_init(void)
{
	/*----------------------------------------------------------------*/
	
	// End frame	
	ENC28J60_terminate;	
	
	// System soft reset
	enc28j60_system_soft_reset();
	
	// Start read buffer
	enc28j60_write_reg_bank(BANK_0_ERXSTL, ENC28J60_BANK_0, ERX_Start & 0xFF);
	enc28j60_write_reg_bank(BANK_0_ERXSTH, ENC28J60_BANK_0, ERX_Start >> 8);
	
	// Set pointer Rx
	enc28j60_write_reg_bank(BANK_0_ERXRDPTL, ENC28J60_BANK_0, ERX_Start & 0xFF);
	enc28j60_write_reg_bank(BANK_0_ERXRDPTH, ENC28J60_BANK_0, ERX_Start >> 8);
	
	// End read buffer
	enc28j60_write_reg_bank(BANK_0_ERXNDL, ENC28J60_BANK_0, ERX_End & 0xFF);
	enc28j60_write_reg_bank(BANK_0_ERXNDH, ENC28J60_BANK_0, ERX_End >> 8);

	// Start write buffer
	enc28j60_write_reg_bank(BANK_0_ETXSTL, ENC28J60_BANK_0, ETX_Start & 0xFF);
	enc28j60_write_reg_bank(BANK_0_ETXSTH, ENC28J60_BANK_0, ETX_Start >> 8);
	
	// End write buffer
	enc28j60_write_reg_bank(BANK_0_ETXNDL, ENC28J60_BANK_0, ETX_End & 0xFF);
	enc28j60_write_reg_bank(BANK_0_ETXNDH, ENC28J60_BANK_0, ETX_End >> 8);
	
	// read filters : UCEN = 1 (matching local MAC), ANDOR = 1 (select filter), CRCEN = 1 (Enable CRC check)
	enc28j60_write_reg_bank(BANK_1_ERXFCON, ENC28J60_BANK_1, 0xE0);	
	
	// Check status clock ready
	while(!(enc28j60_read_ctrl_reg(BANK_ESTAT) & 0x01));
		
	/*----------------------------------------------------------------*/	
	// Setting MACON 1 : TXPAUS = RXPAUS = 1 (Permit Pause Control Frame TxRx), MARXEN = 1 (Packet recerived by MAC)
	enc28j60_write_reg_bank(BANK_2_MACON1,ENC28J60_BANK_2, 0x0D);
	
	// Setting MACON 3 : PADCFG0 = 1 (padded 60 bytes + CRC appended), TXCRCEN = 1 (write CRC Enable), FRMLNEN = 1 (Frame check length enable), FULDPX = 0 (Hafl Duplex)
	enc28j60_write_reg_bank(BANK_2_MACON3, ENC28J60_BANK_2, 0x32);
	
	// Setting MACON 4 : DEFER = 1 (Defer write)
	enc28j60_write_reg_bank(BANK_2_MACON4, ENC28J60_BANK_2, 0x40);
	
	// Maximum frame length to be permited
	enc28j60_write_reg_bank(BANK_2_MAMXFLL, ENC28J60_BANK_2, max_frame_length & 0xFF);
	enc28j60_write_reg_bank(BANK_2_MAMXFLH, ENC28J60_BANK_2, max_frame_length >> 8);
	
	// Configure Back to Back (Half duplex)
	enc28j60_write_reg_bank(BANK_2_MABBPIG, ENC28J60_BANK_2, 0x12);
	
	// Configure Non Back to Back (Half duplex)
	enc28j60_write_reg_bank(BANK_2_MAIPGL, ENC28J60_BANK_2, 0x12);
	enc28j60_write_reg_bank(BANK_2_MAIPGH, ENC28J60_BANK_2, 0xC0);
	
	// Retransmission 
	enc28j60_write_reg_bank(BANK_2_MACLCON1, ENC28J60_BANK_2, 0x03);
	
	// Collision Window ( max = 63)
	enc28j60_write_reg_bank(BANK_2_MACLCON2, ENC28J60_BANK_2, 0x3F);
	
	// Enable read bit : RXEN = 1
	enc28j60_write_ctrl_reg(BANK_ECON1, 0x04);
	
		// Set bit EIE.TXIE and EIE.INTIE
	enc28j60_set_reg(BANK_EIE, 0x88);
	
	/*----------------------------------------------------------------*/
	// Configure PHLCON : LEDA = 0x1000 (ON), LEDB = 0x0111 (Display write/read), Stretch LED = normal(40), Enable stretch Led
	enc28j60_write_phy(PHY_PHLCON, 0x0872);
	
	// Configure PHCON1 : Normal operation, hafl duplex
	enc28j60_write_phy(PHY_PHCON1, 0);
	
	// Configure PHCON2 : HDLDIS = 1 (Disable Loopback PHY Hafl-duplex)
	enc28j60_write_phy(PHY_PHCON2, 0x0100);
}

// ENC28J60 write packet_______________________________________________________________
void enc28j60_write_packet(uint16_t start_addr, uint16_t length, uint8_t *data_packet_write)
{
	uint16_t end_addr = start_addr + length;
	
	// Start write buffer
	enc28j60_write_reg_bank(BANK_0_ETXSTL, ENC28J60_BANK_0, start_addr & 0xFF);
	enc28j60_write_reg_bank(BANK_0_ETXSTH, ENC28J60_BANK_0, start_addr >> 8);

	// End write buffer
	enc28j60_write_reg_bank(BANK_0_ETXNDL, ENC28J60_BANK_0, end_addr & 0xFF);
	enc28j60_write_reg_bank(BANK_0_ETXNDH, ENC28J60_BANK_0, end_addr >> 8);

	
	// write data 0
	enc28j60_write_buffer_memory(0, 1);
	
	// write data
	enc28j60_write_buffer_memory(data_packet_write, length);
	
	// Clear bit EIR.TXIF
	enc28j60_clear_reg(BANK_EIR, 0);
	
	// Start write
	enc28j60_write_ctrl_reg(BANK_ECON1, 0x08);
}

// ENC28J60 read packet_______________________________________________________________
uint16_t enc28j60_read_packet(uint8_t *data_packet_read)
{
	uint16_t length_read;
	uint16_t status_vector;
	uint16_t length_data = 0; 
	static uint16_t next_packet_pointer;	
	
	// Setup address start packer pointer
	enc28j60_write_reg_bank(BANK_0_ERDPTL, ENC28J60_BANK_0, ERX_Start & 0xFF);
	enc28j60_write_reg_bank(BANK_0_ERDPTH, ENC28J60_BANK_0, ERX_Start >> 8);
	
	// Read next packet pointer
	next_packet_pointer = enc28j60_read_buffer_memory();
	next_packet_pointer |= enc28j60_read_buffer_memory() << 8;
	
	// Read read byte conunt
	length_read = enc28j60_read_buffer_memory();
	length_read |= enc28j60_read_buffer_memory() << 8;	
	
	// Remove CRC count
	length_read -= 4;
	
	// Read status vector
	status_vector = enc28j60_read_buffer_memory();
	status_vector |= enc28j60_read_buffer_memory() << 8;		
	
	// read data form packet
	if(status_vector & 0x80)
	{
		length_data = length_read;
		
		while(length_data)
		{
			*data_packet_read++ = enc28j60_read_buffer_memory();
			length_data--;
		}				
	}

	// Setup next address start packer pointer
	enc28j60_write_reg_bank(BANK_0_ERXRDPTL, ENC28J60_BANK_0, next_packet_pointer & 0xFF);
	enc28j60_write_reg_bank(BANK_0_ERXRDPTH, ENC28J60_BANK_0, next_packet_pointer >> 8);	

	// Packet Decrement bit
	enc28j60_write_ctrl_reg(BANK_ECON2, 0x40);
	
	// return length of data
	return length_read;
}

// ENC28J60 Get Revision_______________________________________________________________
uint8_t enc28j60_getrev(void)
{
	return(enc28j60_read_reg_bank(BANK_3_EREVID,ENC28J60_BANK_3));
}


