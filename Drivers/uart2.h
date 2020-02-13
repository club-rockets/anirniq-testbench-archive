/*
 * uart2.h
 *
 *  Created on: 30 mai 2019
 *      Author: Marc-Andre Denis
 */

#ifndef DRIVERS_UART1_H_
#define DRIVERS_UART1_H_

#include "stm32f407xx.h"

#define UART2_BUFFER_SIZE 100

void uart2_init();

uint32_t uart2_transmit(uint8_t*buff,uint8_t size);

uint8_t uart2_rxBuffAvailable();

uint8_t uart2_get();

void uart2_registerRxCallback(void (*callback)(void));

#endif /* DRIVERS_UART1_H_ */
