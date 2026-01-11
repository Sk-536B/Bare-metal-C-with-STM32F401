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
			for(int i = 0; i < 2000000; i++);
		}
	}
}
