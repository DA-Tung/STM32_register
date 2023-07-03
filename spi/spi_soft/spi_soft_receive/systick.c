#include "port.h"
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "systick.h"

// CONFIGURE CLOCK_______________________________________________________________
void SysClock_configure(void)
{	
//	#define PLLM 		4
//	#define PLLN 		192
//	#define PLLP 		4	
//	#define PLLQ		8
//	
	// HSE ON
	RCC->CR |= RCC_CR_HSEON;	
	
	// Wait HSE ready
	while(!(RCC->CR & RCC_CR_HSERDY));
	
	// Enable power interface clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	
	// Disable PLL
	RCC->CR &= ~(1 << 24);
	
	// Power regulator voltage scale
	PWR->CR |= PWR_CR_VOS;
	
	// Instruction cache enable
	FLASH->ACR |= FLASH_ACR_ICEN;

	// Data cache enable
	FLASH->ACR |= FLASH_ACR_DCEN;
	
	// Enable prefetch
	FLASH->ACR |= FLASH_ACR_PRFTEN;
	
	// Configure FLASH LATENCY (3WS)
	FLASH->ACR |= FLASH_ACR_LATENCY_3WS;
	
	// Prescaler AHB cycle
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	
	// Prescaler APB1 cycle
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
	
	// Prescaler APB2 cycle
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
	
	// Configure PLL	
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_2; // divide 4
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_6 | RCC_PLLCFGR_PLLN_7; // multi 192
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLP_0; // divide 4
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLQ_3;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM_4;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLQ_2;
	
	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;
	
	// Wait PLL ready
	while (!(RCC->CR & RCC_CR_PLLRDY));
	
	// Select the clock source
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	
	// Wait it to be set
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);		
	
	NVIC_SetPriority(SysTick_IRQn,1);
}

// Delay 1 us_______________________________________________________________
void delay_systick_us(uint32_t time)
{
	while(time)
	{
		// Configure parameter for 1 us	
		SysTick->LOAD = 71;
		// Current value of Systick counter
		SysTick->VAL = 0;
			
		// Processor clock
		SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
		
		// Enable	counter
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

		// Wait Flag Counter turn on
		while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
		
		// Count down
		time--;		
	}
}

// Delay 1 ms_______________________________________________________________
void delay_systick_ms(uint32_t time)
{
	while(time)
	{
		delay_systick_us(1000);
		time--;
	}
}





