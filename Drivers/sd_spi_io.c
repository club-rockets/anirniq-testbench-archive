


#include "sd_spi_io.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "main.h"


extern SPI_HandleTypeDef hspi2;


extern osSemaphoreId sdSemaphoreHandle;

#define SD_SELECT()   HAL_GPIO_WritePin(SD_CS_GPIO_Port,SWDIO_Pin,GPIO_PIN_RESET);
#define SD_DESELECT() HAL_GPIO_WritePin(SD_CS_GPIO_Port,SWDIO_Pin,GPIO_PIN_SET);

enum{unknown,sdByte,sdBlock} sdCardtype = unknown;
enum{noCard,uninitialised,initialised} cardStatus = uninitialised;


//osSignalWait(ACC_TRANSFER_CPLT_SIGNAL,osWaitForever);

uint8_t sd_sendCommand(uint8_t cmd,uint32_t arg){
	uint8_t cmdBuff[6];
	cmdBuff[0] = cmd;
	cmdBuff[1] = arg>>24;
	cmdBuff[2] = arg>>16;
	cmdBuff[3] = arg>>8;
	cmdBuff[4] = arg;
	cmdBuff[5] = 0;

	if(cmd == CMD0){
		cmdBuff[5] = 0x95;
	}
	if(cmd == CMD8){
		cmdBuff[5] = 0x87;
	}

	HAL_SPI_Transmit_DMA(&hspi2,cmdBuff,6);
	osSignalWait(SD_TRANSFER_CPLT_SIGNAL,osWaitForever);
	uint32_t trycounter = 1500;
	do{
			HAL_SPI_Receive_DMA(&hspi2,cmdBuff,1);
			osSignalWait(SD_TRANSFER_CPLT_SIGNAL,osWaitForever);
			trycounter--;
		}while(trycounter && (cmdBuff[0] & 0x80));

	if(trycounter == 0) return 0xff;

return cmdBuff[0];
}

DSTATUS sd_powerOn(uint8_t diskNo){
	sdCardtype = unknown;

	//wait 1ms
	osDelay(1);

	//spiClock at 100 - 400 khz

	//cs and mosi high send 74 or more clk pulses
	SD_DESELECT();

	uint8_t ffBuffer[10] = {0xff};

	HAL_SPI_Transmit_DMA(&hspi2,ffBuffer,10);
	osSignalWait(SD_TRANSFER_CPLT_SIGNAL,osWaitForever);

	//select
	SD_SELECT();
	if(sd_sendCommand(CMD0,0) != 0xff){
		SD_DESELECT();
		return STA_NOINIT;
	}
	//deselect
	SD_DESELECT();

	//set the spi to full speed to reduce latency

	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	HAL_SPI_Init(&hspi2);

return 0;
}

DSTATUS sd_initialize(uint8_t diskNo){
	sdCardtype = unknown;
	cardStatus = uninitialised;

	sd_powerOn(diskNo);
	uint8_t ffBuffer[5] = {0};
	uint8_t returnVal = 0;

	uint32_t trycounter;

	SD_SELECT();

	sd_sendCommand(CMD8,0x1AA);

	HAL_SPI_Receive_DMA(&hspi2,ffBuffer,4);
	osSignalWait(SD_TRANSFER_CPLT_SIGNAL,osWaitForever);


	//check if 1AA is matched
	if(ffBuffer[2] == 0x01 && ffBuffer[3] == 0xAA){
		osDelay(100);
	}
	else return STA_NOINIT;

	trycounter = 10000;
	do{
		sd_sendCommand(CMD55,0x0);
		returnVal = sd_sendCommand(CMD41,0x40000000);
		trycounter--;
	}while(returnVal == 0x01 && trycounter > 0);

	if(trycounter == 0 || returnVal != 0){
		return STA_NOINIT;
	}

	//read ocr register

	if(sd_sendCommand(CMD58,0) == 0x00){
		HAL_SPI_Receive_DMA(&hspi2,ffBuffer,4);
		osSignalWait(SD_TRANSFER_CPLT_SIGNAL,osWaitForever);

		//read ccs bit
		if(ffBuffer[0] & (1<<6)){
			sdCardtype = sdBlock;
			//force lba size to 512 bytes
			if(sd_sendCommand(CMD16,0x200) == 0){
				cardStatus = initialised;
			}

		}
		else{
			sdCardtype = sdByte;
			cardStatus = initialised;
		}
	}
	else return STA_NOINIT;

	SD_DESELECT();
	return 0;
}

DSTATUS sd_status(uint8_t diskNo){
	//check if card in the slot
	if(HAL_GPIO_ReadPin(SD_DETECT_GPIO_Port,SD_DETECT_Pin) == 0){
		return STA_NODISK;
		cardStatus = noCard;
	}

	//check if initialised
	if(cardStatus != initialised){
		return STA_NOINIT;
	}

	return 0;
}

uint32_t sd_readLba(uint8_t* buff,uint32_t lbano){
	uint8_t cmdBuff[6] = {0};
	uint32_t trycounter = 0;
	//wait for valid data token 0xfe;
	trycounter = 10000;
	do{
		HAL_SPI_Receive_DMA(&hspi2,cmdBuff,1);
		osSignalWait(SD_TRANSFER_CPLT_SIGNAL,osWaitForever);
		trycounter--;
		}while(trycounter && (cmdBuff[0] == 0xff));

	if(cmdBuff[0] == 0xfe){
		osDelay(100);
	}

	return 1;
}

DRESULT sd_read(uint8_t diskno,uint8_t* buff,uint32_t sector,uint32_t lbano){

	if(sdCardtype == sdByte){
		sector*=512;
	}

	SD_SELECT();

	if(lbano == 1){
		//cmd15, single block read
		if(sd_sendCommand(CMD17,sector) == 0){

			sd_readLba(buff,lbano);
			//HAL_SPI_Receive_DMA(&hspi2,buff,512);
			//osSignalWait(SD_TRANSFER_CPLT_SIGNAL,osWaitForever);
		}
		else{
			SD_DESELECT();
			return RES_ERROR;
		}
	}
	else{
		//cmd 18, multiple block read
	}

	SD_DESELECT();

	return RES_OK;
}

DRESULT sd_write(uint8_t diskno,uint8_t* buff,uint32_t sector,uint32_t lbano){
	if(sdCardtype == sdByte){
		sector*=512;
	}

	if(lbano == 1){
		if(sd_sendCommand(CMD24,sector) == 0){
			HAL_SPI_Transmit_DMA(&hspi2,buff,512);
			osSignalWait(SD_TRANSFER_CPLT_SIGNAL,osWaitForever);
		}
	}
	else{
		SD_DESELECT();
		return RES_ERROR;
	}

return RES_OK;
}
