/*
 * ads1100.c
 *
 *  Created on: Oct 23, 2012
 *      Author: Jozef Rodina
 */

#include "ads1100.h"
#include "i2c.h"

Status initADS1100(void)
{
	unsigned char data = 0x88;
	Status error = I2C_Master_BufferWriteWithoutRegisterAddress(&data, 1, ADS1100_ADDRESS_W);

	return error;
}

Status readDataADS1100(unsigned int*data)
{
	unsigned char buffer[3];

	Status error = I2C_Master_BufferReadWithoutRegisterAddress(buffer, 3, ADS1100_ADDRESS_R);

	*data = (buffer[0]<<8) + buffer[1];

	return error;
}
