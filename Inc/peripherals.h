#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#include <string.h>
#include "stm32f4xx.h"

/* Macro to calculate length of an Array */
#define ARR_LEN(arr) (sizeof(arr)/sizeof(arr[0]))

extern volatile uint32_t msTicks;

void rcc_init(void);
void gpio_init(void);
void usart_init(void);
void usart_transmit_polling(uint8_t *pData, uint32_t size, uint32_t timeout);
void usart_receive_polling(uint8_t *pData, uint32_t size, uint32_t timeout);

static inline void ms_delay(uint32_t delay) {
	uint32_t init_val = msTicks;
	while((msTicks - init_val) < delay);
}

#endif /* PERIPHERALS_H_ */
