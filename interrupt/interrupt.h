
#include "stm32f4xx.h"
#include "stm32f407xx.h"

// Level interrupt
#define irq_level_0           0U
#define irq_level_1           1U
#define irq_level_2           2U
#define irq_level_3           3U
#define irq_level_4           4U

// Define Rise/fall irq
#define rising_trigger        0U
#define falling_trigger       1U

typedef struct
{
	uint16_t exti_port;
	uint8_t trigger_level;
	IRQn_Type irq_type;
	uint32_t priority_level_irq;
}irq_config;

//void interrupt_config(GPIO_TypeDef* GPIOx, uint16_t PINx, uint32_t exti_port, uint32_t trigger_level);
void interrupt_config(GPIO_TypeDef* GPIOx, uint16_t PINx, irq_config para_irq);

// Handler EXTI
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);

// Handler DMA
void DMA1_Stream0_IRQHandler(void);
void DMA1_Stream1_IRQHandler(void);
void DMA1_Stream2_IRQHandler(void);
void DMA1_Stream3_IRQHandler(void);
void DMA1_Stream4_IRQHandler(void);
void DMA1_Stream5_IRQHandler(void);
void DMA1_Stream6_IRQHandler(void);
void DMA1_Stream7_IRQHandler(void);

void DMA2_Stream0_IRQHandler(void);
void DMA2_Stream1_IRQHandler(void);
void DMA2_Stream2_IRQHandler(void);
void DMA2_Stream3_IRQHandler(void);
void DMA2_Stream4_IRQHandler(void);
void DMA2_Stream5_IRQHandler(void);
void DMA2_Stream6_IRQHandler(void);
void DMA2_Stream7_IRQHandler(void);

// Handler ADC
void ADC_IRQHandler(void);

// Handler CAN
void CAN1_TX_IRQHandler(void);
void CAN1_RX0_IRQHandler(void);
void CAN1_RX1_IRQHandler(void);
void CAN1_SCE_IRQHandler(void);

//Handler TIM
void TIM1_CC_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void TIM5_IRQHandler(void);

// Handler I2C
void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);
void I2C2_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);
void I2C3_EV_IRQHandler(void);
void I2C3_ER_IRQHandler(void);

// Handler SPI
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);

// Hanlder USART
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void USART4_IRQHandler(void);
void USART5_IRQHandler(void);
void USART6_IRQHandler(void);




