#include "main.h"

volatile uint32_t msTicks;
uint8_t tx_buff[10] = "SARAVANA!";

int main(void) {
	rcc_init();
	SysTick_Config(84000UL); /* Assuming Sysclock is 84MHZ */
	gpio_init();
	usart_init();

	while(1) {
		// Read PA0 for Low Signal
		if((GPIOA->IDR & (1UL << 0)) == 0) {

			usart_transmit_polling(tx_buff, 100, 10);

			// Toggle PC13
			GPIOC->ODR ^= (1UL << 13);
			ms_delay(200);
		}
	}
}

void SysTick_Handler(void) {
	msTicks++;
}
