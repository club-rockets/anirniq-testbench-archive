#include "cmsis_os.h"
#include "gpio.h"

#include "main.h"
#include "app_heartbeat.h"


void app_blink(void*arg)
{
//	osDelay(300000);
//	EjectDrogue();
    while (1) {
        HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        osDelay(100);
    }
}
