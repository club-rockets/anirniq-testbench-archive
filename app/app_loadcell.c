#include "cmsis_os.h"
#include "gpio.h"

#include "main.h"
#include "app_loadcell.h"
#include "HX711.h"

static float valKG_1 = 0;
static float valKG_2 = 0;
static float valKG_3 = 0;

extern osMutexId loadcellMutexHandle;

void tsk_loadcell(void*arg)
{
	osDelay(100);
	HX711_setup();

	uint32_t loadcellLSB1 = 0;
	uint32_t loadcellLSB2 = 0;
	uint32_t loadcellLSB3 = 0;

	float zero1 = 0;
	float zero2 = 0;
	float zero3 = 0;

    while (1) {
		osDelay(200);

		//read loadcells
		HX711_read_all(&loadcellLSB1,&loadcellLSB2,&loadcellLSB3);

		//convert/store values
		osMutexWait(loadcellMutexHandle,10);



		valKG_1 = (((float)loadcellLSB1) * 0.0000814907F);
		valKG_2 = (((float)loadcellLSB2) * 0.0000093132F);
		valKG_3 = (((float)loadcellLSB3) * 0.0000093132F);

		//if tare button pressed
		if(HAL_GPIO_ReadPin(TARE_1_GPIO_Port,TARE_1_Pin) == 0){
			zero1 = valKG_1;
		}
		if(HAL_GPIO_ReadPin(TARE_2_GPIO_Port,TARE_2_Pin) == 0){
					zero2 = valKG_2;
		}
		if(HAL_GPIO_ReadPin(TARE_3_GPIO_Port,TARE_3_Pin) == 0){
					zero3 = valKG_3;
		}

		valKG_1 -= zero1;
		valKG_2 -= zero2;
		valKG_3 -= zero3;

		osMutexRelease(loadcellMutexHandle);
		//repeat

		osDelay(10);

    }
}

void loadcellGet(float* a, float* b, float* c){
	osMutexWait(loadcellMutexHandle,10);
	*a = valKG_1;
	*b = valKG_2;
	*c = valKG_3;
	osMutexRelease(loadcellMutexHandle);
}
