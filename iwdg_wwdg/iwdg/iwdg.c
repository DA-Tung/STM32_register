#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "stdint.h"
#include "iwdg.h"


void iwdg_init(IWDG_TypeDef *IWDG_def, IWDG_config c_iwdg)
{
	// start the watchdog
	IWDG_def->KR = iwdg_start;
	
	// Enable access the register
	IWDG_def->KR = iwdg_enable_access;
	
	// set parameter prescaler divider
	IWDG_def->PR = c_iwdg.pre_divider;
	
	// set parameter reload value
	IWDG_def->RLR = c_iwdg.reload_reg;	
}

void iwdg_reload_counter(IWDG_TypeDef *IWDG_def)
{
	// reload value counter
	IWDG_def->KR = iwdg_reload;
}










