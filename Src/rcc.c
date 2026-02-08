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
