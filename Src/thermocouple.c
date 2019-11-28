/*
 * thermocouple.c
 *
 *  Created on: 14 nov. 2019
 *      Author: Rouss & Duv
 */

#include "thermocouple.h"

int InitDefaultMAXRegisters(t_register* deviceRegister, int size){

	//Set Component register lenght
	size = 16;

	//From MAX datasheet
	uint8_t registre[16] = {0x00, 0x03, 0xff, 0x7f, 0xc0, 0x07f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t registrePermission[16] = {0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x2,  0x2, 0x2, 0x2};
	uint8_t address[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c,  0x0d, 0x0e, 0x0f};

	//Init registers
	for (size_t i = 0; i < size; ++i) {

		deviceRegister->currentValue = 0;
		deviceRegister->defaultValue = registre[i];
		deviceRegister->permission = registrePermission[i];
		deviceRegister->address = address[i];

	}
	return 1;
}

int InitMAX(uint8_t* data, int size){

data[0] = 0x80|0x00;
data[1] = CR0_AUTOMATIC_CONVERSION | CR0_OPEN_CIRCUIT_FAULT_TYPE_K;	// CR0
data[2] = CR1_AVERAGE_1_SAMPLE | CR1_THERMOCOUPLE_TYPE_K;  // CR1
data[3] = MASK_VOLTAGE_UNDER_OVER_FAULT | MASK_THERMOCOUPLE_OPEN_FAULT;  // MASK

}

void spiTx(SPI_HandleTypeDef * hspi1, uint8_t* data,uint32_t size, uint32_t Timeout,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){

	HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_RESET);

	if (HAL_SPI_Transmit(hspi1,data,size, Timeout) != HAL_OK){

		// a voir
	}

	HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_SET);

}

void refreshRegister(t_register* deviceRegister, const uint8_t* data, uint8_t address, int totalRegisters){

	for (int var = 0; var < totalRegisters; ++var) {

		deviceRegister[address +var].currentValue = data[var+1];
	}
}

void spiRx(SPI_HandleTypeDef * hspi1, uint8_t* dataTx, uint8_t* dataRx,uint32_t size, uint32_t Timeout,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){

	HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_RESET);

	HAL_SPI_Transmit(hspi1,dataTx,1, Timeout);
	HAL_SPI_Receive(hspi1,dataRx,size, Timeout);

	HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_SET);
}

int Temperature(const uint8_t * rxBuffer, long * data, double * temperature){

	*data = (rxBuffer[0] << 24 | rxBuffer[1] << 16 | rxBuffer[2] << 8 | rxBuffer[3]); //Concat�nation des valeurs des 4 registres

	if(*data == 0xFFFFFFFF){
		return -1; //pas de communication de la chip slave

	}else{
		*data = *data >> 13;
		//Celsius
		*temperature = (double) *data * 0.0078125;
	}

	return 1;
}

int JunctionTemperature(const uint8_t * rxBuffer, double* temperature, long* data){

	long temperatureOffset = 0;

	*data = (rxBuffer[0] << 16 | rxBuffer[1] << 8 | rxBuffer[2]); //Concat�nation des valeurs des 4 registres

	if (*data == 0xFFFFFFFF){
		return -1;
	}else{
		temperatureOffset = (*data & 0xFF0000) >> 16;
	}

	//G�rer la cas du negatif A faire
    *data = *data >> 2;
    *temperature = (double) *data + temperatureOffset; //Ajout de la temperature offset
    *temperature *= 0.015625; //Comme un shift vers dizaines

    return 1;
}
