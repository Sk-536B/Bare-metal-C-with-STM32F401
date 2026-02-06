#include "peripherals.h"

void rcc_init(void) {
	/* Turn HSE on */
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));

	/* Enable Power Peripheral Clock */
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	(void)RCC->APB1ENR;

	/* Select appropriate Voltage Scaling for 84MHZ (Scale 2) */
	PWR->CR &= ~(3UL << 14U);
	PWR->CR |= (2UL << 14);

	/* Configure Flash latency, enable prefetch, Instruction Cache, Data Cache */
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |=
			FLASH_ACR_LATENCY_2WS |
			FLASH_ACR_PRFTEN 			|
			FLASH_ACR_ICEN 				|
			FLASH_ACR_DCEN;

	/* Configure PLL Src - HSE, PLLM - 25, PLLN - 336, PLLP - 4 */
	RCC->PLLCFGR &=
			~(RCC_PLLCFGR_PLLM |
				RCC_PLLCFGR_PLLN |
				RCC_PLLCFGR_PLLP);
	RCC->PLLCFGR |=
			(25UL << RCC_PLLCFGR_PLLM_Pos)	|
			(336UL << RCC_PLLCFGR_PLLN_Pos)	|
			(1UL << RCC_PLLCFGR_PLLP_Pos)		|
			RCC_PLLCFGR_PLLSRC_HSE;

	/* Turn PLL on */
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));

	/* APB1 prescaler Sysclock/2 */
	RCC->CFGR &= ~RCC_CFGR_PPRE1;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

	/* Sysclock source - PLL */
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR	|= RCC_CFGR_SW_PLL;

	/* Wait for PLL switch status */
	while((RCC->CFGR & RCC_CFGR_SWS_1) != RCC_CFGR_SWS_PLL);

	/* Enable GPIOA and GPIOC Peripheral Clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
	(void)RCC->AHB1ENR;

	/* Enable Timer 2 Peripheral Clock */
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	(void)RCC->APB1ENR;

	/* Enable USART1, SYSCFG Peripheral Clock */
	RCC->APB2ENR |= (RCC_APB2ENR_USART1EN | RCC_APB2ENR_SYSCFGEN);
	(void)RCC->APB2ENR;
}

void gpio_init(void) {
	/* Set PA0 as Input, PA9 and PA10 as Alternate Function */
	GPIOA->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER9 | GPIO_MODER_MODER10);
	GPIOA->MODER |= (GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1);

	/* Pull-up PA0 */
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;

	/* Select AF7 for PA9 and PA10 - USART1 */
	GPIOA->AFR[1] &= ~((15UL << 4U) | (15UL << 8U));
	GPIOA->AFR[1] |= ((7UL << 4U) | (7UL << 8U));

	/* Set PC13 as Output */
	GPIOC->MODER &= ~GPIO_MODER_MODER13;
	GPIOC->MODER |= GPIO_MODER_MODE13_0;

	/* Map PA0 to EXTI0 */
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;

	/* Configure EXTI */
	EXTI->IMR |= EXTI_IMR_MR0;		// Unmask Interrupt for line 0
	EXTI->RTSR &= ~EXTI_RTSR_TR0; 	// Disable Rising trigger for line 0
	EXTI->FTSR |= EXTI_FTSR_TR0;	// Enable Falling trigger for line 0

	/* Enable NVIC */
	NVIC_SetPriority(EXTI0_IRQn, 0);
	NVIC_EnableIRQ(EXTI0_IRQn);
}

void usart_init(void) {
	/* Set Baudrate */
	USART1->BRR = 0x2D9;

	/* Enable UART, Receiver, Transmitter */
	USART1->CR1 |= (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE);
}

/*
 * Loop through the Data, Send it Byte by Byte
 * Wait until TXE, then write Data into Data Register
 * If timeout is exceeded exit the function
 */
void usart_transmit_polling(uint8_t *pData, uint32_t size, uint32_t timeout) {
	uint32_t init_val = msTicks;

	for(int i = 0; i < size; i++) {
		while(!(USART1->SR & (1UL << 7U))) {
			if((msTicks - init_val) > timeout) return;
		}

		USART1->DR = pData[i];
	}
}

/*
 * Loop to receive Data Byte by Byte
 * Wait until RXNE, then read from Data Register
 * If timeout is exceeded exit the function
 */
void usart_receive_polling(uint8_t *pData, uint32_t size, uint32_t timeout) {
	uint32_t init_val = msTicks;

	for(int i = 0; i < size; i++) {
		while(!(USART1->SR & (1UL << 5U))) {
			if((msTicks - init_val) > timeout) return;
		}

		pData[i] = USART1->DR;
	}
}
