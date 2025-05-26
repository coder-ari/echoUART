#ifndef STM32F401_H
#define STM32F401_H

#include <stdint.h>

//
// Base addresses
//
#define PERIPH_BASE         0x40000000UL
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000UL)
#define APB1PERIPH_BASE     (PERIPH_BASE + 0x00000000UL)
#define APB2PERIPH_BASE     (PERIPH_BASE + 0x00010000UL)

#define GPIOA_BASE          (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOC_BASE          (AHB1PERIPH_BASE + 0x0800UL)
#define RCC_BASE            (AHB1PERIPH_BASE + 0x3800UL)
#define TIM2_BASE           (APB1PERIPH_BASE + 0x0000UL)
#define USART1_BASE         (APB2PERIPH_BASE + 0x1000UL)

//
// RCC registers
//
#define RCC_AHB1ENR         (*(volatile uint32_t *)(RCC_BASE + 0x30UL))
#define RCC_APB1ENR         (*(volatile uint32_t *)(RCC_BASE + 0x40UL))
#define RCC_APB2ENR         (*(volatile uint32_t *)(RCC_BASE + 0x44UL))

//
// GPIOA registers
//
#define GPIOA_MODER         (*(volatile uint32_t *)(GPIOA_BASE + 0x00UL))
#define GPIOA_PUPDR         (*(volatile uint32_t *)(GPIOA_BASE + 0x0CUL))
#define GPIOA_AFRL          (*(volatile uint32_t *)(GPIOA_BASE + 0x20UL))
#define GPIOA_AFRH          (*(volatile uint32_t *)(GPIOA_BASE + 0x24UL))

//
// GPIOC registers
//
#define GPIOC_MODER         (*(volatile uint32_t *)(GPIOC_BASE + 0x00UL))
#define GPIOC_ODR           (*(volatile uint32_t *)(GPIOC_BASE + 0x14UL))

//
// TIM2 registers
//
#define TIM2_CR1            (*(volatile uint32_t *)(TIM2_BASE + 0x00UL))
#define TIM2_DIER           (*(volatile uint32_t *)(TIM2_BASE + 0x0CUL))
#define TIM2_SR             (*(volatile uint32_t *)(TIM2_BASE + 0x10UL))
#define TIM2_CNT            (*(volatile uint32_t *)(TIM2_BASE + 0x24UL))
#define TIM2_PSC            (*(volatile uint32_t *)(TIM2_BASE + 0x28UL))
#define TIM2_ARR            (*(volatile uint32_t *)(TIM2_BASE + 0x2CUL))

//
// USART1 registers
//
#define USART1_SR           (*(volatile uint32_t *)(USART1_BASE + 0x00UL))
#define USART1_DR           (*(volatile uint32_t *)(USART1_BASE + 0x04UL))
#define USART1_BRR          (*(volatile uint32_t *)(USART1_BASE + 0x08UL))
#define USART1_CR1          (*(volatile uint32_t *)(USART1_BASE + 0x0CUL))
#define USART1_CR2          (*(volatile uint32_t *)(USART1_BASE + 0x10UL))
#define USART1_CR3          (*(volatile uint32_t *)(USART1_BASE + 0x14UL))

//
// NVIC
//
#define NVIC_ISER0          (*(volatile uint32_t *)0xE000E100UL)
#define NVIC_ISER1          (*(volatile uint32_t *)0xE000E104UL)

//
// IRQ Numbers
//
#define TIM2_IRQ_NUMBER     28
#define USART1_IRQ_NUMBER   37

#endif // STM32F401_H
