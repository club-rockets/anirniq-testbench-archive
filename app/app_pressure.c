#include "cmsis_os.h"
#include "main.h"
#include "adc.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"

#include "app_pressure.h"

extern ADC_HandleTypeDef hadc1;

void tsk_pressure(void*arg){
    
    osDelay(20);

    while(1){

        //read adc

        //calculate pressure values

        //save/send to sd card

        //wait 20ms
    }
}