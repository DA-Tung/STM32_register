
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "systick.h"

// channel dac
#define dac_channel_1          1
#define dac_channel_2          2

// Alignment
#define dac_align_8b_R1        1
#define dac_align_8b_R2        2

#define dac_align_12b_L1       3
#define dac_align_12b_R1       4
#define dac_align_12b_L2       5
#define dac_align_12b_R2       6

#define dac_align_8b_RD        7
#define dac_align_12b_LD       8
#define dac_align_12b_RD       9

#define trigger_disen          0U
#define trigger_en             1U

#define timer_6_trig_ev				 0U
#define timer_8_trig_ev				 1U
#define timer_7_trig_ev				 2U
#define timer_5_trig_ev				 3U
#define timer_2_trig_ev				 4U
#define timer_4_trig_ev				 5U
#define ext_line_9_trig 			 6U
#define software_trigger			 7U
#define not_trigger_dac        0U

void dac_config(DAC_TypeDef *DACx, uint8_t channel, uint8_t trigger, uint32_t trigger_para);
void dac_set_value(DAC_TypeDef *DACx, uint8_t channel, uint8_t alignment, uint32_t value_data);
void dac_set_multi_value(DAC_TypeDef *DACx, uint8_t channel, uint8_t alignment, uint32_t *value_data, uint32_t length);

void dac_dma_start(DAC_TypeDef *DACx, uint8_t channel, uint8_t alignment, uint32_t *value_data, uint32_t length);




