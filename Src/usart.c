#include "peripherals.h"

void usart_init(void) {
	/* Set Baudrate */
	USART1->BRR = 0x2D9;

	/* Enable UART, Receiver, Transmitter */
	USART1->CR1 |= (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE);
}

/*
 * Loop through the Data, Send it Byte by Byte
 * Wait for TXE flag
 * Write Data into Data Register
 * If timeout is exceeded exit the function
 */
void usart_transmit_polling(uint8_t *pData, uint32_t size, uint32_t timeout) {
	uint32_t init_val = msTicks;

	for(int i = 0; i < size; i++) {
		while(!(USART1->SR & USART_SR_TXE)) {
			if((msTicks - init_val) > timeout) return;
		}

		USART1->DR = pData[i];
	}
}

/*
 * Loop to receive Data Byte by Byte
 * Wait for RXNE flag
 * Read Data from Data Register
 * If timeout is exceeded exit the function
 */
void usart_receive_polling(uint8_t *pData, uint32_t size, uint32_t timeout) {
	uint32_t init_val = msTicks;

	for(int i = 0; i < size; i++) {
		while(!(USART1->SR & USART_SR_RXNE)) {
			if((msTicks - init_val) > timeout) return;
		}

		pData[i] = USART1->DR;
	}
}
