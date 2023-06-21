
#include "stm32f4xx.h"
#include "stm32f407xx.h"

// channel adc
#define adc_channel_0			      0U
#define adc_channel_1			      1U
#define adc_channel_2						2U
#define adc_channel_3						3U
#define adc_channel_4						4U
#define adc_channel_5						5U
#define adc_channel_6						6U
#define adc_channel_7						7U
#define adc_channel_8						8U
#define adc_channel_9						9U
#define adc_channel_10					10U
#define adc_channel_11					11U
#define adc_channel_12					12U
#define adc_channel_13					13U
#define adc_channel_14					14U
#define adc_channel_15   				15U

#define sample_time_3cyc        0U
#define sample_time_15cyc       1U
#define sample_time_28cyc       2U
#define sample_time_56cyc       3U
#define sample_time_84cyc       4U
#define sample_time_112cyc      5U
#define sample_time_144cyc      6U
#define sample_time_480cyc      7U

void adc_init(ADC_TypeDef* ADCx, uint32_t num_channel);
void adc_channel_conv(ADC_TypeDef* ADCx, char channel);
uint16_t adc_get_value(ADC_TypeDef* ADCx);
void adc_channel_config(ADC_TypeDef* ADCx, uint32_t channel_dma, uint32_t pos, uint32_t sample_time);
void adc_start(ADC_TypeDef* ADCx);

