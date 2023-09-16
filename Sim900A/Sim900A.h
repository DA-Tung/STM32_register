#ifndef _SIM900_H_
#define _SIM900_H_

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f407xx.h"

// Define commands
#define GSMTest                "AT\r\n"
#define CheckMSGMode           "AT+CMGS=?\r\n"
#define checkMem               "AT+CPMS=?\r\n"
#define Activate_SMS_Mode      "AT+CMGF=1\r\n"
#define TestSmsRead            "AT+CMGR=?\r\n"
#define ReadALLSms             "AT+CMGL=\"ALL\"\r\n"
#define ReadOneSms             "AT+CMGR=1\r\n"
#define SimMemory              "AT+CPMS=\"SM\"\r\n"
#define SaveMsgSetting         "AT+CSAS\r\n"
#define ReciveMsg              "AT+CNMI=1,1,0,0,0\r\n"
#define DeleteAllSms           "AT+CMGDA=\"DEL ALL\"\r\n"
#define SetClock               "AT+CCLK?\r\n"

typedef enum
{
	Text_OK=0,
	Text_NOT_OK,
	Sender_OK=0,
	TokenCode_OK,
	TokenUnits_OK,
	Search_OK,
	SMS_SENT,
	COMMAND_OK,
	SMS_READ_OK,
}Message_Status;

typedef enum
{
	Text=0,
	Sender,
	TokenCode,
	TokenUnits,
	COMMAND,
}SIM900_FIND;

typedef enum
{
	READ=0,
	DELETE,
	INBOX_MEM,
	CAPACITY_MEM,
}MESSAGE_CHOICE;

typedef struct
{
	char Message_Sender[15];
	char Message_Body[30];
	char Time_Received[11];
	char Date_Received[11];
}Message_Typedef;

void clear_buffer(void);
void SIM900Init(void);
void ReadSmsInbox(int SMS_NUMBER);
void ReadAllSmsInbox(void);
int Send_Message(char Phone_Number[13],char *TextMsg);
void DeleteSingleMessage(int SMS_NUMBER);
void DeleteAllMessages(void);
int Find_in_Text(char Search_in_name[20],char *string);
int Check_No_of_message_In_Memory(int MEMORY);
void Message_Interrupt(void);
int Read_Delete_LastMsg(int READ_DELETE);
void clear_MessageDetails(Message_Typedef *Message_Storage);

#endif // _SIM900_H_



