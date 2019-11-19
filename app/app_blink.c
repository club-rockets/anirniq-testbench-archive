#include "cmsis_os.h"
#include "gpio.h"

#include "main.h"
#include "app_blink.h"


void app_blink(void*arg)
{
//	osDelay(300000);
//	EjectDrogue();
    while (1) {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        osDelay(100);
    }
}
