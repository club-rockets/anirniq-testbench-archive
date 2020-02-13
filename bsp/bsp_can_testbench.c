/*
 * BSP_CAN.c
 *
 *  Created on: 15 mars 2019
 *      Author: Marc-Andre
 * 
 * A sodomised version of bsp can for the testbench
 */

#include "bsp_can_regdef.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "cmsis_os.h"
#include "string.h"

void vMemcpy(volatile void* dest, const volatile void* src,uint32_t lengt){
	uint32_t i = 0;
	for(;i<lengt;i++){
		((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
	}
}

//return false if no mailbox is available
uint32_t can_canSetRegisterData(uint32_t index, can_regData_u *data){
	return can_canSetAnyRegisterData(CAN_BOARD,index,data,1);
}

//return false if no mailbox is available
//if data pointer is null, send register without changing data value
uint32_t can_canSetAnyRegisterData(uint32_t board, uint32_t index, can_regData_u *data,uint8_t callbackEn)
{
    if (board >= 7 || index >= can_registersSize[board]) {
        return 0;
    }

    if(data){
    	vMemcpy(&(can_registers[board][index].data),data,sizeof(can_regData_u));
    }
    can_registers[board][index].lastTick = HAL_GetTick();


    //call callback
    if (can_registers[board][index].changeCallback && callbackEn) {
        can_registers[board][index].changeCallback(board, index);
    }

    return 1;
}

uint32_t can_getRegisterData(enum can_board board, uint32_t index, can_regData_u *reg)
{
    *reg = can_registers[board][index].data;
    return 1;
}

uint32_t can_getRegisterTimestamp(enum can_board board, uint32_t index)
{
    return can_registers[board][index].lastTick;
}

uint32_t can_setRegisterCallback(enum can_board board, uint32_t index, void (*callback)(uint32_t,uint32_t))
{
    can_registers[board][index].changeCallback = callback;
    return 1;
}

