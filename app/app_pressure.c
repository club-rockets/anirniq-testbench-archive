#include "cmsis_os.h"
#include "main.h"
#include "adc.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"
#include "bsp_can.h"
#include "bsp_can_regdef.h"

#include "app_pressure.h"
#include "app_sd.h"

extern ADC_HandleTypeDef hadc1;

float pressureConvert(uint32_t adcval){
    float temp = (float)adcval;
    temp -=745.0;
    temp /=5.956;
    return temp;
}

void tsk_pressure(void*arg){
    
    osDelay(20);

    const TickType_t readInterval = 50;
    TickType_t lastWake = 0;
    lastWake = xTaskGetTickCount();

    uint32_t adcVal1 = 0;
    uint32_t adcVal2 = 0;
    uint32_t adcVal3 = 0;
    float pressure1 = 0;
    float pressure2 = 0;
    float pressure3 = 0;

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    can_regData_u canData = {0};

    while(1){
        //wait 50ms
        vTaskDelayUntil(&lastWake,readInterval);

        //read adc
        sConfig.Channel = ADC_CHANNEL_5;
        HAL_ADC_ConfigChannel(&hadc1, &sConfig);
        HAL_ADC_Start(&hadc1);
        vTaskDelay(1);
        adcVal1 = HAL_ADC_GetValue(&hadc1);
        pressure1 = pressureConvert(adcVal1);
        sd_writeFloat("pressure1",pressure1);
        canData.FLOAT = pressure1;
        can_canSetRegisterData(CAN_TESTBENCH_PRESSURE_1,&canData);
        
        sConfig.Channel = ADC_CHANNEL_6;
        HAL_ADC_ConfigChannel(&hadc1, &sConfig);
        HAL_ADC_Start(&hadc1);
        vTaskDelay(1);
        adcVal2 = HAL_ADC_GetValue(&hadc1);
        pressure2 = pressureConvert(adcVal2);
        sd_writeFloat("pressure2",pressure2);
        canData.FLOAT = pressure2;
        can_canSetRegisterData(CAN_TESTBENCH_PRESSURE_2,&canData);

        sConfig.Channel = ADC_CHANNEL_7;
        HAL_ADC_ConfigChannel(&hadc1, &sConfig);
        HAL_ADC_Start(&hadc1);
        vTaskDelay(1);
        adcVal3 = HAL_ADC_GetValue(&hadc1);
        pressure3 = pressureConvert(adcVal3);
        sd_writeFloat("pressure3",pressure3);
        canData.FLOAT = pressure3;
        can_canSetRegisterData(CAN_TESTBENCH_PRESSURE_3,&canData);

        //save/send to sd card

    }
}