#include "main.h"

int main(void) {
	rcc_init();
	gpio_init();

	while(1) {
		// Read PA0 for Low Signal
		if((GPIOA->IDR & (1UL << 0)) == 0) {

			// Toggle PC13
			GPIOC->ODR ^= (1UL << 13);

			// Debouncing
			for(int i = 0; i < 1000000; i++);
		}
	}
}

void rcc_init(void) {
	// Enable GPIOA Peripheral Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// Enable GPIOC Peripheral Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
}

void gpio_init(void) {
	// GPIOA Pin 0 mode to Input
	GPIOA->MODER &= ~GPIO_MODER_MODER0_Msk;

	// GPIOC Pin 13 mode to Output
	GPIOC->MODER &= ~GPIO_MODER_MODER13_Msk;
	GPIOC->MODER |= GPIO_MODER_MODE13_0;

	// Pull-up PA0
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0_Msk;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;
}
