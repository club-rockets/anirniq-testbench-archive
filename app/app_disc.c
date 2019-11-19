

#include "app_disc.h"

#include "fatfs.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

#include <stdio.h>
#include <string.h>

#include"sd_spi_io.h"


uint8_t lbaBuff[512] = {0};

void tsk_disk(void const * argument){


	sd_initialize(0);

	sd_read(0,lbaBuff,1<<9,1);

	while(1){
		osDelay(100);
	}


}

