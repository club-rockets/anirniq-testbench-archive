/*
 * uart2.c
 *
 *  Created on: 30 mai 2019
 *      Author: Marc-Andre Denis
 */

#include "uart2.h"

static void(*volatile uart2_rxCallback)(void) = 0;

volatile struct{
	uint8_t start,end;
	uint8_t Buff[UART2_BUFFER_SIZE];
} uart1_RxBuff = {0},uart1_TxBuff = {0};

void uart2_init() {

	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	//enable the peripheral clock

	//enable the peripheral
	USART2->CR1 |= USART_CR1_UE;

	//set baud rate settings 460800
	//settings for 42mhz APB1 clock

	//USART2->BRR = 11<<4 | 0x6;
	USART2->BRR = 5<<4 | 0x7;

	//enable receiver mode
	USART2->CR1 |= USART_CR1_RE;
	//ENABLE TRANSMIT MODE
	USART2->CR1 |= USART_CR1_TE;

	//enable rxne interrupts
	USART2->CR1 |= USART_CR1_RXNEIE;

	//enable USART2 in nvic
	NVIC_EnableIRQ(38);
	NVIC_SetPriority(38, 11);
}


//return positive is transfer possible without buffer overrun
uint32_t uart2_transmit(uint8_t*buff,uint8_t size){
	//check for space
	if(uart1_TxBuff.end<uart1_TxBuff.start){
		if(uart1_TxBuff.start-uart1_TxBuff.end -1 < size) return 0;
	}
	else if (uart1_TxBuff.end>uart1_TxBuff.start){
		if(UART2_BUFFER_SIZE -1 -uart1_TxBuff.end + uart1_TxBuff.start < size) return 0;
	}

	//copy the data in the buffer
	uint8_t i;
	for(i=0;i<size;i++){
		uart1_TxBuff.end++;
		uart1_TxBuff.end%=UART2_BUFFER_SIZE;
		uart1_TxBuff.Buff[uart1_TxBuff.end] = buff[i];
	}
	//enable transmit interrupt
	USART2->CR1 |= USART_CR1_TXEIE;
	return 1;
}

uint8_t uart2_rxBuffAvailable(){
	return !(uart1_RxBuff.end == uart1_RxBuff.start);
}

uint8_t uart2_get(){
	uart1_RxBuff.start++;
	uart1_RxBuff.start%=UART2_BUFFER_SIZE;
	return uart1_RxBuff.Buff[uart1_RxBuff.start];
}

void uart2_registerRxCallback(void (*callback)(void)){
	uart2_rxCallback = callback;
}

void USART2_IRQHandler(){
	if(USART2->SR & USART_SR_TXE){
		//if tx buffer not empty
		if(uart1_TxBuff.end-uart1_TxBuff.start){
			uart1_TxBuff.start++;
			uart1_TxBuff.start%=UART2_BUFFER_SIZE;
			USART2->DR = uart1_TxBuff.Buff[uart1_TxBuff.start];
		}
		else USART2->CR1 &=~USART_CR1_TXEIE;
	}
	if(USART2->SR & USART_SR_RXNE){
		uart1_RxBuff.end++;
		uart1_RxBuff.end%=UART2_BUFFER_SIZE;
		uart1_RxBuff.Buff[uart1_RxBuff.end] = USART2->DR;
		if(uart2_rxCallback){
			uart2_rxCallback();
		}
	}
}



