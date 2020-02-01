#include "HX711.h"

#define _USING_FREERTOS_

#ifdef _USING_FREERTOS_
#include "cmsis_os.h"
#endif

void HX711_setup()
{
  // Enable the
	RCC->AHB1ENR |= HX711_AHB1ENR;

	HX711_GPIO->MODER |= (1 << (HX711_CLOCK_PIN << 1));

	HX711_GPIO->OSPEEDR |= (1 << (HX711_CLOCK_PIN << 1));


	// Enable Timer 4 for precise wait
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	TIM4->CR1 |= TIM_CR1_ARPE | TIM_CR1_DIR;
	// Prescale by 1 to get 10MHz clock
	TIM4->PSC = 3;


  // Set the clock pin high (IDLE)
	HX711_CLOCK_HIGH();
}

void HX711_wait100ns(uint32_t ns_100)
{
	TIM4->ARR = ns_100;
	TIM4->EGR |= TIM_EGR_UG;

	TIM4->SR &= ~TIM_SR_UIF;
	TIM4->CR1 |= TIM_CR1_CEN;

	while(!(TIM4->SR & TIM_SR_UIF));

	TIM4->CR1 &= ~TIM_CR1_CEN;
	TIM4->SR &= ~TIM_SR_UIF;
}


uint32_t HX711_read(uint32_t nb)
{
	union{
		uint32_t uint;
		signed val : 24;
	} buff;
	buff.uint = 0;

	HX711_CLOCK_LOW();

	while (HX711_DATA_VALUE() == 1){
#ifdef _USING_FREERTOS_
		osDelay(1);
#else
		:
#endif
	}

	HX711_wait100ns(3);

	for (uint8_t i = 0; i < 24; i++)
	{
#ifdef _USING_FREERTOS_
		taskENTER_CRITICAL();
#endif
		HX711_CLOCK_HIGH();
		HX711_wait100ns(3);

		buff.uint <<= 1 ;
		buff.uint += HX711_DATA_VALUE();

		HX711_CLOCK_LOW();
#ifdef _USING_FREERTOS_
		taskEXIT_CRITICAL();
#endif
		HX711_wait100ns(3);
	}

	for (uint8_t i = 0; i < 1; i++)
	{
#ifdef _USING_FREERTOS_
		taskENTER_CRITICAL();
#endif
		HX711_CLOCK_HIGH();
		HX711_wait100ns(3);
		HX711_CLOCK_LOW();
		HX711_wait100ns(3);
#ifdef _USING_FREERTOS_
		taskEXIT_CRITICAL();
#endif
	}
	//buffer = buffer ^ 0x800000;
	if(buff.val<0){
		buff.val = 0-buff.val;
	}

	return buff.uint;
}


uint32_t HX711_read_all(uint32_t* a, uint32_t* b, uint32_t* c){
	union{
		uint32_t uint;
		signed val : 24;
	} buff[3] = {0};

	uint32_t i = 0;

	HX711_CLOCK_LOW();

	while (HX711_all_ready() == 0){
#ifdef _USING_FREERTOS_
		osDelay(1);
#else
		:
#endif
	}

	HX711_wait100ns(3);

	for (uint8_t i = 0; i < 24; i++)
	{
#ifdef _USING_FREERTOS_
		taskENTER_CRITICAL();
#endif
		HX711_CLOCK_HIGH();
		HX711_wait100ns(3);

		buff[0].uint <<= 1 ;
		buff[0].uint += HX711_DATA_VALUE();

		buff[1].uint <<= 1 ;
		buff[1].uint += HX711_DATA_VALUE2();

		buff[2].uint <<= 1 ;
		buff[2].uint += HX711_DATA_VALUE3();

		HX711_CLOCK_LOW();
#ifdef _USING_FREERTOS_
		taskEXIT_CRITICAL();
#endif
		HX711_wait100ns(3);
	}

	for (uint8_t i = 0; i < 1; i++)
	{
#ifdef _USING_FREERTOS_
		taskENTER_CRITICAL();
#endif
		HX711_CLOCK_HIGH();
		HX711_wait100ns(3);
		HX711_CLOCK_LOW();
		HX711_wait100ns(3);
#ifdef _USING_FREERTOS_
		taskEXIT_CRITICAL();
#endif
	}

	for(i=0;i<3;i++){
		if(buff[i].val<0){
			buff[i].val = 0-buff[i].val;
		}
	}

	*a = buff[0].uint;
	*b = buff[1].uint;
	*c = buff[2].uint;

	return 1;
}
