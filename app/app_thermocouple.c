#include "cmsis_os.h"
#include "gpio.h"

#include "main.h"
#include "app_thermocouple.h"


extern osMutexId loadcellMutexHandle;

void tsk_thermocouple(void*arg)
{

	//suspend Task
	vTaskSuspend(0);

	//initialise thermocouples

	const TickType_t readInterval = 100;
	TickType_t lastWake = xTaskGetTickCount();
    while (1) {
		vTaskDelayUntil(&lastWake,readInterval);

		//read

    }
}

