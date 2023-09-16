#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "usart.h"
#include "systick.h"
#include "gpio.h"
#include "Sim900A.h"

#define GSM_USART  USART2
Message_Typedef Message_Details={0};

int No_of_messages=0;
char Reply[200]= {0};

// CLEAR BUFFER_______________________________________________________________
void clear_buffer(void)
{
	memset(Reply,0,strlen(Reply));
}

// SIM 900A INIT_______________________________________________________________
void SIM900Init(void)
{
	clear_buffer();
	while(1)
	{
		usart_send_string(GSM_USART,GSMTest);
		usart_receiver_string(GSM_USART,Reply,10);
		
		delay_systick_ms(1000);
		if(Reply[2]=='O' && Reply[3]=='K')
		{
			delay_systick_ms(1000);
			break;
		}
	}	
	//Activate TEXT MODE FOR SMS
	usart_send_string(GSM_USART,Activate_SMS_Mode);
	delay_systick_ms(1000);
	//check and Save memory setting
	usart_send_string(GSM_USART,SaveMsgSetting);	
	delay_systick_ms(1000);
	//Get time
	Message_Interrupt();
}

// READ SMS INBOX_______________________________________________________________
void ReadSmsInbox(int SMS_NUMBER)
{
	int Sep_quotes=0,comma=0,Char_Position_in_Reply=0,Sender_Digit=0,Date_Digit=0,Body_Digit=0,Time_Digit=0;
	char MessageSender[14]={0},DateReceived[9]={0},TimeReceived[9]={0},MessageBody[30]={0};
	char msgnumber[30];
	clear_buffer();
	sprintf(msgnumber,"AT+CMGR=%d\r\n",SMS_NUMBER);
	usart_send_string(GSM_USART,msgnumber);	
	usart_receiver_string(GSM_USART,Reply,100);
	delay_systick_ms(1000);
	for(Char_Position_in_Reply=0;Char_Position_in_Reply<strlen(Reply);Char_Position_in_Reply++)
	{
		if(Reply[Char_Position_in_Reply]=='"')
		{
			Sep_quotes++;
			Char_Position_in_Reply++;
		}
		if(Reply[Char_Position_in_Reply]==',')
		{
			comma++;
		}
		if(Sep_quotes==3)
		{
			MessageSender[Sender_Digit]=Reply[Char_Position_in_Reply];
			Sender_Digit++;
		}
		if(Sep_quotes==6 && comma==3 && Date_Digit<8)
		{
			DateReceived[Date_Digit]=Reply[Char_Position_in_Reply];
			Date_Digit++;
		}
		if(Sep_quotes==6 && comma==4 &&Time_Digit<8)
		{
			TimeReceived[Time_Digit]=Reply[Char_Position_in_Reply+1];
			Time_Digit++;
		}
		if(Sep_quotes==7)
		{
			MessageBody[Body_Digit]=Reply[Char_Position_in_Reply];
			Body_Digit++;
		}
	}

	sprintf(Message_Details.Message_Sender,"%s\t",MessageSender);
	sprintf(Message_Details.Date_Received,"%s\t",DateReceived);
	sprintf(Message_Details.Time_Received,"%s\t",TimeReceived);
	sprintf(Message_Details.Message_Body,"%s\r\n",MessageBody);
}

// CHECK NO MESSAGE IN MEMORY_______________________________________________________________
int Check_No_of_message_In_Memory(int MEMORY)
{
	char unit[15]= {0};
	uint8_t count=0,n=0,command=0,Value_To_Return=0;
	clear_buffer();
	//Check sim memory
	usart_send_string(GSM_USART,SimMemory);
	usart_receiver_string(GSM_USART,Reply,100);
	delay_systick_ms(100);
	if(Find_in_Text("CPMS",Reply)==Text_OK)
	{
		while(n<=strlen(Reply))
		{
			if(isdigit(Reply[n])!=0)
			{
				unit[count]=Reply[n];
				count++;
			}
			if(Reply[n]==',')
			{
				if(command==0)
				{
					No_of_messages=atoi(unit);
					count=0;
					if(MEMORY==INBOX_MEM)
					{
						Value_To_Return= No_of_messages;
						break;
					}
				}
				if(command==1 &&MEMORY==CAPACITY_MEM)
				{
					No_of_messages=atoi(unit);
					Value_To_Return= No_of_messages;
					break;
				}
				else
				{
					No_of_messages=0;
				}
				command++;
			}
			n++;
		}
	}
	return Value_To_Return;
}

// SEND MESSAGE_______________________________________________________________
int Send_Message(char Phone_Number[13],char *TextMsg)
{
	int Ctrl_z=26;
	char ch[1];
	char Phone[30];
	ch[0]=Ctrl_z;
	strcpy(Phone,"AT+CMGS=\"");
	strcat(Phone,Phone_Number);
	strcat(Phone,"\"\r\n");
	usart_send_string(GSM_USART,Phone);
	delay_systick_ms(1000);
	usart_send_string(GSM_USART,TextMsg);
	delay_systick_ms(1000);
	usart_send_string(GSM_USART,ch);
	delay_systick_ms(5000);
	return SMS_SENT;
}

// CLEAR MESSAGE DETAILS_______________________________________________________________
void clear_MessageDetails(Message_Typedef *Message_Storage)
{
	char* Clear_msg = NULL;
	
	memcpy(Message_Storage->Date_Received,Clear_msg,strlen(Message_Storage->Date_Received));
	memcpy(Message_Storage->Message_Body,Clear_msg,strlen(Message_Storage->Message_Body));
	memcpy(Message_Storage->Message_Sender,Clear_msg,strlen(Message_Storage->Message_Sender));
	memcpy(Message_Storage->Time_Received,Clear_msg,strlen(Message_Storage->Time_Received));
}

// DELETE SINGLE MESSAGE_______________________________________________________________
void DeleteSingleMessage(int SMS_NUMBER)
{
	char msgnumber[30];
	usart_send_string(GSM_USART,Activate_SMS_Mode);
	delay_systick_ms(2000);
	sprintf(msgnumber,"AT+CMGD=%d\r\n",SMS_NUMBER);
	usart_send_string(GSM_USART, msgnumber);
	delay_systick_ms(1000);
}

// DELETE ALL MESSAGE_______________________________________________________________
void DeleteAllMessages(void)
{
	clear_buffer();
	usart_send_string(GSM_USART,Activate_SMS_Mode);
	delay_systick_ms(1000);
	usart_send_string(GSM_USART, DeleteAllSms);
	delay_systick_ms(3000);
}

// READ DELETE LAST MESSAGE_______________________________________________________________
int Read_Delete_LastMsg(int READ_DELETE)
{
	int No_Sms_Inbox=0;
	No_Sms_Inbox=Check_No_of_message_In_Memory(INBOX_MEM);
	if(No_Sms_Inbox>=20)
	{
		DeleteAllMessages();
	}
	switch(No_Sms_Inbox)
	{
	case 0:
		clear_MessageDetails(&Message_Details);
		return 0;
		break;
	case 1:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(1);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(1);
		}
		break;
	case 2:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(2);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(2);
		}
		break;
	case 3:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(3);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(3);
		}
		break;
	case 4:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(4);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(4);
		}
		break;
	case 5:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(5);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(5);
		}
		break;
	case 6:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(6);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(6);
		}
		break;
	case 7:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(7);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(7);
		}
		break;

	case 8:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(8);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(8);
		}
		break;

	case 9:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(9);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(9);
		}
		break;

	case 10:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(10);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(10);
		}
		break;

	case 11:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(11);
		}

		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(11);
		}
		break;
	case 12:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(12);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(12);
		}
		break;
	case 13:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(13);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(13);
		}

		break;
	case 14:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(14);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(14);
		}

		break;
	case 15:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(15);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(15);
		}
		break;
	case 16:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(16);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(16);
		}
		break;
	case 17:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(17);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(17);
		}
		break;
	case 18:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(18);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(18);
		}
		break;
	case 19:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(19);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(19);
		}
		break;
	case 20:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(20);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(20);
		}
		break;
	case 21:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(21);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(21);
		}
		break;
	case 22:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(22);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(22);
		}
		break;
	case 23:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(23);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(23);
		}
		break;
	case 24:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(24);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(24);
		}
		break;
	case 25:
		if(READ_DELETE==READ)
		{
			ReadSmsInbox(25);
		}
		if(READ_DELETE==DELETE)
		{
			DeleteSingleMessage(25);
		}
		break;
	}
	return 1;
}

// FIND IN TEXT_______________________________________________________________
int Find_in_Text(char Search_in_name[20],char *string)
{
	int n=0,q=0,match=0;
	char test[20]={0};
	char Search_name[20]={0};
	strcpy(Search_name,Search_in_name);
	for(n=0; n<=strlen(string); n++)
	{
		if(string[n]==Search_name[0])
		{
			q=n;
			match=0;
			while(match<strlen(Search_name))
			{
				test[match]=string[q];
				match++;
				q++;
			}
			if(memcmp(Search_name,test,strlen(Search_name))==0)
			{
				return Text_OK;
			}
			else
			{
				return Text_NOT_OK;
			}
		}
	}
	return Search_OK;
}

// MESSAGE INTERRUPT_______________________________________________________________
void Message_Interrupt(void)
{
	usart_send_string(GSM_USART, ReciveMsg);
	delay_systick_ms(1000);
}




