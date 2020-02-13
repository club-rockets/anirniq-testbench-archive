/*
 * heartBeat.h
 *
 *  Created on: 14 mars 2019
 *      Author: Marc-Andre
 *
 *      This thread blinks a led to show that the board is alive
 *
 */


#include "app_transmitReg.h"

#include "uart2.h"     
#include "stm32f4xx_hal.h"
#include "main.h"
#include "cmsis_os.h"
#include "bsp_can.h"
#include "enums.h"
#include "id.h"
#include "cmsis_os.h"
#include "bsp_can_regdef.h"
#include "radio_packet.h"
#include "string.h"



uint8_t txBuff[50] = {0};

extern osMessageQId rxRegsHandle;

static can_regData_u regData = {0};

static enum {transmitPad,transmitLaunch} transmitMode = transmitLaunch;

void tsk_transmitReg(void const * argument){

	osEvent rxEvent;
	union rxReg reg;
	radio_packet_t packet = {0};

	//variables for register scan
	uint32_t lastRegisterTick1 = 0,lastRegisterTick2 = 0;
	uint32_t boardIndex = 0;
	uint32_t registerIndex = 0;
	uint32_t lastBoardIndex = 0;
	uint32_t lastRegisterIndex = 0;


	while(1){
		//if a callback enabled register is updated
		//wait and send an other register
		if(transmitMode == transmitLaunch){
			rxEvent = osMessageGet(rxRegsHandle,30);
		}
		else{
			rxEvent = osMessageGet(rxRegsHandle,osWaitForever);
		}

		if(rxEvent.status == osEventMessage){
			reg.UINT = rxEvent.value.v;

			memset((void*)(&packet),0,sizeof(radio_packet_t));
			packet.node = reg.reg.board;
			packet.message_id = reg.reg.id;
			memcpy(&(packet.payload),(void*)(&(can_registers[packet.node][packet.message_id].data)),sizeof(radio_packet_payload_t));
			packet.checksum = radio_compute_crc(&packet);

			uart2_transmit((uint8_t*)(&packet),sizeof(packet));
		}
		else if(transmitMode == transmitLaunch){
			//verify that the register is not already handled by a change callback or sent recently
			
			registerIndex = (registerIndex + 1)%(can_registersSize[boardIndex]);
			if(!registerIndex){
				if(boardIndex == BENCHTEST) boardIndex = BENCHTESTSTATION;
				else boardIndex = BENCHTEST;
			}
			if(boardIndex == lastBoardIndex && registerIndex == lastRegisterIndex){
				break;
			}

			if(!(boardIndex == lastBoardIndex && registerIndex == lastRegisterIndex)){
				lastBoardIndex= boardIndex;
				lastRegisterIndex = registerIndex;
				//transmit the register over uart
				packet.node = boardIndex;
				packet.message_id = registerIndex;
				memcpy(&(packet.payload),(void*)(&(can_registers[packet.node][packet.message_id].data)),sizeof(radio_packet_payload_t));
				packet.checksum = radio_compute_crc(&packet);
				uart2_transmit((uint8_t*)(&packet),sizeof(packet));
			}
		}
	}
}
