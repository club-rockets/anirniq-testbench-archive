#ifndef HX711_H
#define HX711_H

#include "stm32f4xx.h"

#include <stddef.h>


// Config
#define HX711_GPIO GPIOE
#define HX711_AHB1ENR RCC_AHB1ENR_GPIOFEN

#define HX711_DATA_PIN 4
#define HX711_CLOCK_PIN 6
#define HX711_DATA_PIN2 0
#define HX711_CLOCK_PIN2 2


// Helper macros
#define HX711_DATA_VALUE() ((uint32_t) (HX711_GPIO->IDR & (1 << HX711_DATA_PIN)) >> HX711_DATA_PIN)
#define HX711_CLOCK_HIGH() HX711_GPIO->BSRR |= (1 << HX711_CLOCK_PIN)
#define HX711_CLOCK_LOW() HX711_GPIO->BSRR |= (1 << (HX711_CLOCK_PIN+16))

#define HX711_DATA_VALUE2() ((uint32_t) (HX711_GPIO->IDR & (1 << HX711_DATA_PIN2)) >> HX711_DATA_PIN2)
#define HX711_CLOCK_HIGH2() HX711_GPIO->BSRR |= (1 << HX711_CLOCK_PIN2)
#define HX711_CLOCK_LOW2() HX711_GPIO->BSRR |= (1 << (HX711_CLOCK_PIN2+16))



void HX711_setup();

void HX711_wait100ns(uint32_t ns_100);

uint32_t HX711_read(uint32_t nb);

#endif
