#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "stdint.h"

#define divider_4				  (uint32_t)0U
#define divider_8				  (uint32_t)1U
#define divider_16				(uint32_t)2U
#define divider_32				(uint32_t)3U
#define divider_64				(uint32_t)4U
#define divider_128				(uint32_t)5U
#define divider_256				(uint32_t)6U

#define iwdg_start								(uint32_t)0xCCCCU
#define iwdg_reload								(uint32_t)0xAAAAU
#define iwdg_enable_access      	(uint32_t)0x5555U
#define iwdg_disenable_access			(uint32_t)0x0000U

typedef struct
{
	uint32_t pre_divider;
	uint32_t reload_reg;
}IWDG_config;


void iwdg_init(IWDG_TypeDef *IWDG_def, IWDG_config c_iwdg);
void iwdg_reload_counter(IWDG_TypeDef *IWDG_def);



