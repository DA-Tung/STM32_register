
// System***********************************************************
#include "math.h"
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "cmsis_os.h"

// Library***********************************************************
#include "main.h"
//#include "systick.h"
#include "gpio.h"
#include "timer_delay.h"
#include "osObjects.h"                      	// RTOS object definitions

// Declare variable***********************************************************
//#define osObjectsPublic                     	// define objects in main module
osThreadId ID_Task_1;
osThreadId ID_Task_2;

// Function***********************************************************
void GPIO_ConfigPin(void);
void Task_01 (void const *argument);
void Task_02 (void const *argument);

// GPIO Config
void GPIO_ConfigPin(void)
{
	gpio_config(GPIOD, PORTD, PIN12, PIN_OUTPUT);
	gpio_config(GPIOD, PORTD, PIN13, PIN_OUTPUT);
	gpio_config(GPIOD, PORTD, PIN14, PIN_OUTPUT);
	gpio_config(GPIOD, PORTD, PIN15, PIN_OUTPUT);
}

// Thread 1
void Task_01 (void const *argument) 
{
	(void)argument;
	
	while (1) 
	{
		// Insert thread code here...
		//osThreadYield ();                                           // suspend thread
		gpio_output(GPIOD, PIN12, PIN_TOGGLE);
		osDelay(1000);
	}
}

// Thread 2
void Task_02 (void const *argument) 
{
	(void)argument;
	
	while (1) 
	{
		// Insert thread code here...
		//osThreadYield ();                                           // suspend thread
		gpio_output(GPIOD, PIN14, PIN_TOGGLE);
		osDelay(2000);
	}
}

// Program main***********************************************************
int main()
{
	// initialize peripherals here
	GPIO_ConfigPin();	
	
	osKernelInitialize();                     // initialize CMSIS-RTOS
	
	// create 'thread' functions that start executing,
	osThreadDef(Task_01,osPriorityHigh,1,0);
	ID_Task_1 = osThreadCreate(osThread(Task_01), NULL);

	osThreadDef(Task_02,osPriorityNormal,1,0);
	ID_Task_2 = osThreadCreate(osThread(Task_02), NULL);
	
	osKernelStart ();                         // start thread execution 
	
	while(1)
	{
		
	}
}




