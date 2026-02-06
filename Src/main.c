#include "main.h"

volatile uint32_t msTicks;
uint8_t rx_buff[10];

int main(void) {
	rcc_init();
	SysTick_Config(84000UL); /* Assuming Sysclock is 84MHZ */
	gpio_init();
	usart_init();

	while(1) {
		usart_receive_polling(rx_buff, ARR_LEN(rx_buff), 100);

		if(*rx_buff) {
			usart_transmit_polling(rx_buff, ARR_LEN(rx_buff), 100);

			memset(rx_buff, 0, ARR_LEN(rx_buff));
		}
	}
}

/* Interrupt Handlers */

void SysTick_Handler(void) {
	msTicks++;
}

void EXTI0_IRQHandler(void) {
	// Check if the Interrupt is triggered by EXTI0
	if(EXTI->PR & EXTI_PR_PR0) {

		// Bit is cleared by writing a 1 to it
		EXTI->PR |= EXTI_PR_PR0;

		// Toggle GPIO pin PC13
		GPIOC->ODR ^= GPIO_ODR_OD13;
	}
}
