#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f4xx.h"
#include "peripherals.h"

extern volatile uint32_t msTicks;

static inline void ms_delay(uint32_t delay) {
	uint32_t init_val = msTicks;
	while((msTicks - init_val) < delay);
}

#endif /* MAIN_H_ */
