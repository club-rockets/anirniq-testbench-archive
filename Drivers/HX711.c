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
	HX711_CLOCK_HIGH2();
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
	uint32_t buffer;
	buffer = 0;

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

		buffer <<= 1 ;
		buffer += HX711_DATA_VALUE();

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

	return buffer;
}
