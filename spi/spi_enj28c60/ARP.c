#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "string.h"
#include "enc28j60_spi.h"
#include "spi.h"
#include "systick.h"
#include "gpio.h"
#include "NET.h"
#include "ARP.h"

extern const uint8_t MAC_Addr[6];
extern const uint8_t Ip_Addr[4];

uint8_t Default_Dest_MAC[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t Default_Target_MAC[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Create struct to create frame of ARP
typedef struct
{
	uint8_t  	Dest_MAC[6];          			// MAC target address
	uint8_t  	Source_MAC[6];          		// MAC source address
	uint16_t 	E_type;                 		// ethernet type
	uint16_t 	H_type;                 		// hardwave type
	uint16_t 	P_type;                 		// protocol type (ARP)
	uint16_t 	Size;                         	// size
	uint16_t 	Opcode;                       	// opcode
	uint8_t   	MAC_Sender[6];                  // sender MAC
	uint8_t   	IP_Sender[4];                   // sender IP
	uint8_t   	MAC_Target[6];                  // Target MAC
	uint8_t   	IP_Target[4];                   // Target IP
}FRAME_ARP;	  

// ARP read request_______________________________________________________________
void ARP_read_request(uint8_t *ARP_buffer, uint16_t length)
{
	// Create frame ARP
	FRAME_ARP ARP_frame;
	
	// Create array to save address MAC and IP
	uint8_t MAC_source[6];
	uint8_t IP_source[4];
	uint8_t IP_target[4];
 
	memcpy(MAC_source,ARP_buffer+6,6);  		// Get MAC address of source
	memcpy(IP_source ,ARP_buffer+28,4); 		// Get IP address of source
	memcpy(IP_target ,ARP_buffer+38,4); 		// Get IP address of target
 	 
	// Check it is ARP or not
	if(ARP_buffer[20] == 0x00 && ARP_buffer[21] == 0x01)
	{
		// Check IP address
		if(IP_target[0] != Ip_Addr[0] ||  IP_target[1] != Ip_Addr[1] || IP_target[2] != Ip_Addr[2] || IP_target[3] != Ip_Addr[3]) return;
		
		// Get information of ARP
		memcpy(ARP_frame.Dest_MAC, MAC_source,6);							// 6 byte default 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
		memcpy(ARP_frame.Source_MAC, MAC_Addr,6);							// 6 byte MAC address source (Address send request)
		ARP_frame.E_type = 0x0608;											// 2 byte to verify ARP frame
		ARP_frame.H_type = 0x1000;											// 2 byte to verify is Htype(Ethernet)
		ARP_frame.P_type = 0x0008;											// 2 byte to verify is IPv4
		ARP_frame.Size = 0x0604;											// 2 byte Length
		ARP_frame.Opcode = 0x0200;											// 2 byte to inform Opcode is ARP Request
		memcpy(ARP_frame.MAC_Sender,MAC_Addr,6);							// 6 byte MAC address source (Address send request) 
		memcpy(ARP_frame.IP_Sender, Ip_Addr,4);								// 4 byte IP address source (Address send request) 
		memcpy(ARP_frame.MAC_Target, MAC_source,6);							// 6 byte default of Target MAC : 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 when undefined MAC of machine receive request 
		memcpy(ARP_frame.IP_Target, IP_target,4);							// 4 byte IP address of machine receive request
		
		// Send data frame of ARP
		NET_SendFrame((uint8_t *)&ARP_frame,42);
	}
}

// ARP send request_______________________________________________________________
void ARP_send_request(uint8_t *ip_dest)
{
	// Create frame ARP	
	FRAME_ARP ARP_frame;
	
	memcpy(ARP_frame.Dest_MAC, Default_Dest_MAC,6);							// 6 byte default 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
	memcpy(ARP_frame.Source_MAC, MAC_Addr,6);								// 6 byte MAC address source (Address send request)
	ARP_frame.E_type = 0x0608;												// 2 byte to verify ARP frame
	ARP_frame.H_type = 0x1000;												// 2 byte to verify is Htype(Ethernet)
	ARP_frame.P_type = 0x0008;												// 2 byte to verify is IPv4
	ARP_frame.Size = 0x0604;												// 2 byte Length
	ARP_frame.Opcode = 0x0100;												// 2 byte to inform Opcode is ARP Request
	memcpy(ARP_frame.MAC_Sender,MAC_Addr,6);								// 6 byte MAC address source (Address send request) 
	memcpy(ARP_frame.IP_Sender, Ip_Addr,4);									// 4 byte IP address source (Address send request) 
	memcpy(ARP_frame.MAC_Target, Default_Target_MAC,6);						// 6 byte default of Target MAC : 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 when undefined MAC of machine receive request 
	memcpy(ARP_frame.IP_Target, ip_dest,4);									// 4 byte IP address of machine receive request
	
	// Send data frame of ARP
	NET_SendFrame((uint8_t *)&ARP_frame,42);	
}	



