#include "cmsis_os.h"
#include "main.h"
#include "adc.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"

#include "app_pressure.h"

extern ADC_HandleTypeDef hadc1;
//Ajouter le mutex

void tsk_pressure(void*arg){
    
	osDelay(100);

	int error = 0;
	uint32_t value = 0;
	float current = 0;
	float resistance = 150;
	float pressure = 0.;


    while(1){

    	osDelay(200);

        //read adc
    	value = (pressureGet(&hadc1)*3)/4095;

        //calculate pressure values
    	pressure = 31.25*((value/resistance)-4);

        //save/send to sd card

        //wait 10ms
    	osDelay(10);
    }
}

uint32_t pressureGet(ADC_HandleTypeDef * adc){

	uint32_t ADCValue = 0;

	//Start to read ADC channel 1
	if(HAL_ADC_Start(adc) == HAL_OK){

		//Start
		if(HAL_ADC_PollForConversion(adc,1000) == HAL_OK){

			ADCValue = HAL_ADC_GetValue(adc);
			HAL_ADC_Stop(adc);

		}
	}

}
