#include "peripherals.h"

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
