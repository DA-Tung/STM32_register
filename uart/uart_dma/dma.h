
#include "stm32f4xx.h"
#include "stm32f407xx.h"

#define Peripheral_to_Memory   0U
#define Memory_to_Peripheral   1U
#define Memory_to_Memory       2U

#define dma_data_byte          0U
#define dma_data_half_word     1U
#define dma_data_word          2U

#define dma_channel_0          0U
#define dma_channel_1          1U
#define dma_channel_2          2U
#define dma_channel_3          3U
#define dma_channel_4          4U
#define dma_channel_5          5U
#define dma_channel_6          6U
#define dma_channel_7          7U

typedef struct
{
	uint32_t type_data;
	uint32_t size_data;
	uint32_t channel_stream;
	uint32_t priority_level;
	IRQn_Type type_irq;
}dma_para_config;

void dma_config(DMA_Stream_TypeDef * DMAx_Streamx, dma_para_config config_dma);
void dma_get_data(DMA_Stream_TypeDef * DMAx_Streamx, uint32_t reg_addr, uint32_t m_data, uint16_t length);




