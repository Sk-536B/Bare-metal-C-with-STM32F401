#include "main.h"

volatile uint32_t msTicks;

int main(void) {
	rcc_init();
	SysTick_Config(84000UL); /* Assuming Sysclock is 84MHZ */
	gpio_init();

	while(1) {
		// Read PA0 for Low Signal
		if((GPIOA->IDR & (1UL << 0)) == 0) {

			// Toggle PC13
			GPIOC->ODR ^= (1UL << 13);
			ms_delay(200);
		}
	}
}

void SysTick_Handler(void) {
	msTicks++;
}
