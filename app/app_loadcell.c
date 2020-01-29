#include "cmsis_os.h"
#include "gpio.h"

#include "main.h"
#include "app_loadcell.h"
#include "HX711.h"

uint32_t loadcellLSB1 = 0;
uint32_t loadcell_kg1 = 0;

void tsk_loadcell(void*arg)
{
	osDelay(100);
	HX711_setup();
	float valKG_1;

    while (1) {
    	osDelay(200);
    	loadcellLSB1 = HX711_read(0);

    	  valKG_1 = (((float)loadcellLSB1) * 0.0000814907F) - 0.65;
    	  loadcell_kg1 = (uint32_t)(1000.0*valKG_1);
    }
}
