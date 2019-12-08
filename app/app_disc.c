

#include "app_disc.h"
#include "fatfs_sd.h"

#include "fatfs.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

#include <stdio.h>
#include <string.h>


uint8_t lbaBuff1[1024];
uint8_t lbaBuff2[1024];


void tsk_disk(void const * argument){

	while(1){
		osDelay(100);
	}

	memset(lbaBuff1,0x22,512);
	memset(lbaBuff2,0x0,512);

	osDelay(100);

	SD_disk_initialize(0);

	SD_disk_write(0,lbaBuff1,0,1);

	SD_disk_read(0,lbaBuff2,0,1);
	int i = 0;
	for(i=0;i<512;i++){
		if(lbaBuff1[i] != lbaBuff2[i]) break;
	}

	memset(lbaBuff1,0xCC,1024);
	memset(lbaBuff2,0x0,1024);


	SD_disk_write(0,lbaBuff1,1,2);

	SD_disk_read(0,lbaBuff2,1,2);

	for(i=0;i<1024;i++){
		if(lbaBuff1[i] != lbaBuff2[i]) break;
	}

	while(1){
		osDelay(100);
	}


}

