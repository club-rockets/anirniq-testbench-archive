/*
 * APP_receiveReg.c
 *
 *  Created on: Jun 3, 2019
 *      Author: Marc-Andre Denis
 *
 *      Receive register update command via uart and transmit it to the can bus
 *
 */

#include "app_transmitReg.h"
#include "radio_packet.h"
#include "cmsis_os.h"
#include "uart2.h"
#include "bsp_can.h"
#include "string.h"
#include "APP_transmitReg.h"

#define SIGNAL_NEW_DATA (1<<0)

extern osThreadId app_receiveRegHandle;

extern osMessageQId rxRegsHandle;

void bytesReceived();


static union{
	uint8_t UINT8[sizeof(radio_packet_t)];
	radio_packet_t packet;
} packetBuff = {0};

static uint8_t packetBuffIndex = 0;

void tsk_receiveReg(void const * argument){
	union rxReg regConf = {0};
	osEvent evt;
	while(1){
		//wait for uart transmission
		uart2_registerRxCallback(bytesReceived);
		evt = osSignalWait(SIGNAL_NEW_DATA,osWaitForever);
		if(evt.status == osEventSignal && (evt.value.v & SIGNAL_NEW_DATA)){

			uart2_registerRxCallback(0);

			//get data from uart buffer
			while(uart2_rxBuffAvailable()){
				//fill the packet buffer
				packetBuff.UINT8[packetBuffIndex] = uart2_get();
				packetBuffIndex++;

				if(packetBuffIndex < (sizeof(packetBuff))){
					continue;
				}

				//compute the crc and compare
				if(radio_compute_crc(&(packetBuff.packet)) == packetBuff.packet.checksum){
					//the message is valid
					packetBuffIndex = 0;

					//save message

					//send packet on internal can bus call callback only if board is com
					if (can_canSetAnyRegisterData(packetBuff.packet.node,\
							packetBuff.packet.message_id,\
							(can_regData_u*)(&(packetBuff.packet.payload)),\
							(packetBuff.packet.node == BENCHTEST)) ){
						//send confirmation to the base station
						regConf.reg.board = packetBuff.packet.node;
						regConf.reg.id = packetBuff.packet.message_id;
						osMessagePut(rxRegsHandle,regConf.UINT,0);
					}

				}
				else{
					//packet is invalid left shift entire buffer to recover the start of next message
					uint8_t i = 0;
					for(i = 0;i < (sizeof(packetBuff) - 1); i++){
						packetBuff.UINT8[i] = packetBuff.UINT8[i+1];
					}
					--packetBuffIndex;
				}

			}
		}
	}
};

void bytesReceived(){
	osSignalSet(app_receiveRegHandle,SIGNAL_NEW_DATA);
}
