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
