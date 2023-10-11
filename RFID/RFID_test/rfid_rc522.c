#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "rfid_rc522.h"
#include "gpio.h"
#include "spi.h"

#define CS_Enable		     	gpio_output(GPIOA,PIN4,PIN_RESET)
#define CS_DisEnable 			gpio_output(GPIOA,PIN4,PIN_SET)

#define RST_Enable		     	gpio_output(GPIOB,PIN0,PIN_SET)
#define RST_DisEnable 			gpio_output(GPIOB,PIN0,PIN_RESET)

#define RFID_SPI				SPI1	
 
void RC522_ConfigPin(void)
{
	// SPI Config PIN
	spi_ConfigPin();
	
	// PIN Select slave
	gpio_config(GPIOA,PORTA, PIN4, PIN_OUTPUT);
	
	// PIN Reset
	gpio_config(GPIOB,PORTB, PIN0, PIN_OUTPUT);
}

uint8_t RC522_RW_Data(uint8_t w_data)
{
	uint8_t r_data;
	
	spi_readwrite(RFID_SPI, &w_data, &r_data, 1);
	
	return r_data;
}

// RFID RC522 write data_______________________________________________________________
void RC522_Write_Data(uint8_t addr_reg, uint8_t data_write)
{
	// Page 11 - RC522 Reference : address write is located 0XXXXXX0	
	uint8_t addr = (addr_reg << 1) & 0x7E;
	
	// Enable RC55
	CS_Enable;
	
	// Transmit command
	RC522_RW_Data(addr);
	
	// Transmit data
	RC522_RW_Data(data_write);
	
	// Disenable RC522
	CS_DisEnable;
	
}

// RFID RC522 read data_______________________________________________________________
uint8_t RC522_Read_Data(uint8_t addr_reg)
{
	uint8_t data_read;
	
	// Page 11 - RC522 Reference : address read is located 1XXXXXX0
	uint8_t addr = ((addr_reg << 1) & 0x7E) | 0x80;
	
	// Enable RC522
	CS_Enable;	

	// Transmit command
	RC522_RW_Data(addr);
	
	// Receive data
	data_read = RC522_RW_Data(0x00);
	
	// Disenable RC522
	CS_DisEnable;
	
	// Return data
	return data_read;
}

// RFID RC522 Set Bit_______________________________________________________________
void SetBitMask(uint8_t reg, uint8_t mask)  
{
    uint8_t tmp;
    tmp = RC522_Read_Data(reg);
    RC522_Write_Data(reg, tmp | mask);  // set bit mask
}

// RFID RC522 Reset Bit_______________________________________________________________
void ClearBitMask(uint8_t reg, uint8_t mask)  
{
    uint8_t tmp;
    tmp = RC522_Read_Data(reg);
    RC522_Write_Data(reg, tmp & (~mask));  // clear bit mask
} 

// RFID RC522 Antenna On_______________________________________________________________
void RC522_AntennaOn(void)
{
	RC522_Read_Data(TxControlReg);
	
	SetBitMask(TxControlReg, 0x03);
} 

// RFID RC522 Antenna Off_______________________________________________________________
void RC522_AntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
} 

// RFID RC522 soft reset_______________________________________________________________
void RC522_SoftReset(void)
{
	RC522_Write_Data(CommandReg, CMD_SOFT_RESET);
}

// RFID RC522 init_______________________________________________________________
void RC522_init(void)
{
	CS_DisEnable;
	RST_Enable;
	
	// Reset RC522
	RC522_SoftReset();
	
	/* Config register */
	// TModeReg : TAuto = 1, T_Gate = 00, T_AutoRestart = 0, TPrescaler_Hi = 0
	RC522_Write_Data(TModeReg, 0x8D);
	
	// TPrescalerReg : config 25us - TPrescaler_Hi(0x00) + TPrescaler_Lo(0xA9)
	RC522_Write_Data(TPrescalerReg, 0x3E); 
	
	// Reload time : 0x03E8 ~ 1000
	RC522_Write_Data(TReloadRegH, 0x30); 	
	RC522_Write_Data(TReloadRegL, 0x0);
	
	// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
	RC522_Write_Data(TxASKReg, 0x40);     

	// CRCPreset = 6363h
	RC522_Write_Data(ModeReg, 0x3D); 		
	
	// Turn on Antenna
	RC522_AntennaOn();
}

// RFID RC522 To Card_______________________________________________________________
uint8_t RC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint8_t *backLen)
{
    uint8_t status = MI_ERR;
    uint8_t irqEn = 0x00;
    uint8_t waitIRq = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint16_t i;

    switch (command)
    {
        case CMD_AUTHENT:		//Acknowledging the liver
		{
			irqEn = 0x12;
			waitIRq = 0x10;
			break;
		}
		case CMD_TRANSCEIVE:	// FIFO data collection
		{
			irqEn = 0x77;
			waitIRq = 0x30;
			break;
		}
		default:
			break;
    }
   
    RC522_Write_Data(CommIEnReg, irqEn|0x80);	//Request interrput
    ClearBitMask(CommIrqReg, 0x80);			//Clear all the bits
    SetBitMask(FIFOLevelReg, 0x80);			//FlushBuffer=1, FIFO
    
	RC522_Write_Data(CommandReg, CMD_IDLE);	//NO action; cancel command

	// Record in FIFO
    for (i=0; i<sendLen; i++)
    {   
		RC522_Write_Data(FIFODataReg, sendData[i]);    
	}

	//Run and check
	RC522_Write_Data(CommandReg, command);
    if (command == CMD_TRANSCEIVE)
    {    
		SetBitMask(BitFramingReg, 0x80);		//StartSend=1,transmission of data starts  
	}   
    
	//The team is allowed to be stored
	i = 2000;	//i depend on crystal, max time M1 is 25ms
    do 
    {
		//CommIrqReg[7..0]
		//Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
        n = RC522_Read_Data(CommIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitIRq));

    ClearBitMask(BitFramingReg, 0x80);			//StartSend=0
	
    if (i != 0)
    {    
        if(!(RC522_Read_Data(ErrorReg) & 0x1B))	//BufferOvfl Collerr CRCErr ProtecolErr
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   
				status = MI_NOTAGERR;			//??   
			}

            if (command == CMD_TRANSCEIVE)
            {
               	n = RC522_Read_Data(FIFOLevelReg);
              	lastBits = RC522_Read_Data(ControlReg) & 0x07;
                if (lastBits)
                {   
					*backLen = (n-1)*8 + lastBits;   
				}
                else
                {   
					*backLen = n*8;   
				}

                if (n == 0)
                {   
					n = 1;    
				}
                if (n > MAX_LEN)
                {   
					n = MAX_LEN;   
				}
				
				//FIFO read in the received data
                for (i=0; i<n; i++)
                {   
					backData[i] = RC522_Read_Data(FIFODataReg);    
				}
            }
        }
        else
        {   
			status = MI_ERR;  
		}
    }
    return status;
}

// RFID RC522 Request_____________________________________________________________
uint8_t RC522_Request(uint8_t reqMode, uint8_t *TagType)
{
	uint8_t status;  
	uint8_t backBits;			//The bits are manipulated

	RC522_Write_Data(BitFramingReg, 0x07);		//TxLastBists = BitFramingReg[2..0]	???
	
	TagType[0] = reqMode;
	status = RC522_ToCard(CMD_TRANSCEIVE, TagType, 1, TagType, &backBits);

	if ((status != MI_OK) || (backBits != 0x10))
	{    
		status = MI_ERR;
	}
   
	return status;
}

// RFID RC522 Anticoll_______________________________________________________________
uint8_t RC522_Anticoll(uint8_t *serNum)
{
    uint8_t status;
    uint8_t i;
	uint8_t serNumCheck=0;
    uint8_t unLen;
    

    //ClearBitMask(Status2Reg, 0x08);		//TempSensclear
    //ClearBitMask(CollReg,0x80);			//ValuesAfterColl
	RC522_Write_Data(BitFramingReg, 0x00);		//TxLastBists = BitFramingReg[2..0]
 
    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = RC522_ToCard(CMD_TRANSCEIVE, serNum, 2, serNum, &unLen);

    if (status == MI_OK)
	{
		//Check the serial number
		for (i=0; i<4; i++)
		{   
		 	serNumCheck ^= serNum[i];
		}
		if (serNumCheck != serNum[i])
		{   
			status = MI_ERR;    
		}
    }

    //SetBitMask(CollReg, 0x80);		//ValuesAfterColl=1

    return status;
} 

// RFID RC522 CRC_______________________________________________________________
void RC522_CalulateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData)
{
    uint8_t i, n;

    ClearBitMask(DivIrqReg, 0x04);			//CRCIrq = 0
    SetBitMask(FIFOLevelReg, 0x80);			//Con tro FIFO
    //Write_MFRC522(CommandReg, PCD_IDLE);

	//Record in FIFO
    for (i=0; i<len; i++)
    {   
		RC522_Write_Data(FIFODataReg, *(pIndata+i));   
	}
    RC522_Write_Data(CommandReg, CMD_CALCCRC);

	// Let the CRC computer complete
    i = 0xFF;
    do 
    {
        n = RC522_Read_Data(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));			//CRCIrq = 1

	//Doc results in CRC calculation
    pOutData[0] = RC522_Read_Data(CRCResultRegL);
    pOutData[1] = RC522_Read_Data(CRCResultRegM);
}

// RFID RC522 Select Tag_______________________________________________________________
uint8_t MFRC522_SelectTag(uint8_t *serNum)
{
	uint8_t i;
	uint8_t status;
	uint8_t size;
	uint8_t recvBits;
	uint8_t buffer[9]; 

	//ClearBitMask(Status2Reg, 0x08);			//MFCrypto1On=0

    buffer[0] = PICC_SElECTTAG;
    buffer[1] = 0x70;
    for (i=0; i<5; i++)
    {
    	buffer[i+2] = *(serNum+i);
    }
	RC522_CalulateCRC(buffer, 7, &buffer[7]);		//??
    status = RC522_ToCard(CMD_TRANSCEIVE, buffer, 9, buffer, &recvBits);
    
    if ((status == MI_OK) && (recvBits == 0x18))
    {   
		size = buffer[0]; 
	}
    else
    {   
		size = 0;    
	}

    return size;
}

// RFID RC522 check password_______________________________________________________________
uint8_t RC522_Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t *Sectorkey, uint8_t *serNum)
{
    uint8_t status;
    uint8_t recvBits;
    uint8_t i;
	uint8_t buff[12]; 

	//Confirmation + Address + password + quick number
    buff[0] = authMode;
    buff[1] = BlockAddr;
    for (i=0; i<6; i++)
    {    
		buff[i+2] = *(Sectorkey+i);   
	}
    for (i=0; i<4; i++)
    {    
		buff[i+8] = *(serNum+i);   
	}
    status = RC522_ToCard(CMD_AUTHENT, buff, 12, buff, &recvBits);

    if ((status != MI_OK) || (!(RC522_Read_Data(Status2Reg) & 0x08)))
    {   
		status = MI_ERR;   
	}
    
    return status;
}

// RFID RC522 Read docoment_______________________________________________________________
uint8_t RC522_Read_Doc(uint8_t blockAddr, uint8_t *recvData)
{
    uint8_t status;
    uint8_t unLen;

    recvData[0] = PICC_READ;
    recvData[1] = blockAddr;
    RC522_CalulateCRC(recvData,2, &recvData[2]);
    status = RC522_ToCard(CMD_TRANSCEIVE, recvData, 4, recvData, &unLen);

    if ((status != MI_OK) || (unLen != 0x90))
    {
        status = MI_ERR;
    }
    
    return status;
}

// RFID RC522 Write docoment_______________________________________________________________
uint8_t RC522_Write_Doc(uint8_t blockAddr, uint8_t *writeData)
{
    uint8_t status;
    uint8_t recvBits;
    uint8_t i;
	uint8_t buff[18]; 
    
    buff[0] = PICC_WRITE;
    buff[1] = blockAddr;
    RC522_CalulateCRC(buff, 2, &buff[2]);
    status = RC522_ToCard(CMD_TRANSCEIVE, buff, 4, buff, &recvBits);

    if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
    {   
		status = MI_ERR;   
	}
        
    if (status == MI_OK)
    {
        for (i=0; i<16; i++)		//16 FIFO bytes recorded
        {    
        	buff[i] = *(writeData+i);   
        }
        RC522_CalulateCRC(buff, 16, &buff[16]);
        status = RC522_ToCard(CMD_TRANSCEIVE, buff, 18, buff, &recvBits);
        
		if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
        {   
			status = MI_ERR;   
		}
    }
    
    return status;
}

// RFID RC522 Sleep_______________________________________________________________
void RC522_Sleep(void)
{
	uint8_t unLen;
	uint8_t buff[4]; 

	buff[0] = PICC_HALT;
	buff[1] = 0;
	RC522_CalulateCRC(buff, 2, &buff[2]);
 
	RC522_ToCard(CMD_TRANSCEIVE, buff, 4, buff,&unLen);
}


