

#include "app_disc.h"

#include "fatfs.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

#include <stdio.h>
#include <string.h>

#include"sd_spi_io.h"


uint8_t lbaBuff[512] = {0xaa};

void tsk_disk(void const * argument){


	sd_initialize(0);

	sd_write(0,lbaBuff,0,1);

	sd_read(0,lbaBuff,0,1);

	while(1){
		osDelay(100);
	}


}

