/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO/STM32
**                STMicroelectronics STM32Lxx Standard Peripherals Library
**
**  Distribution: The file is distributed “as is,” without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

#include <stddef.h>
#include <math.h>
#include <stdio.h>

#include "mpu6050.h"
#include "stm32l1xx.h"
#include "spi.h"
#include "ssd1306.h"
#include "ili9163.h"
#include "elkocka.h"


volatile int16_t data[14], test;

// prerusenie od MPU6050 pre DMA pristup
void DMA1_Channel7_IRQHandler(void) {
	// ak nastalo prerusenie
	if (DMA_GetFlagStatus(DMA1_FLAG_TC7)) {
		// vynulujeme vlajku prerusenia
		DMA_ClearFlag(DMA1_FLAG_TC7);
		// vypneme DMA prenos
		I2C_DMACmd(I2C1, DISABLE);
		// vygenerujeme koniec prenosu
		I2C_GenerateSTOP(I2C1, ENABLE);
		// vypneme DMA kanal
		DMA_Cmd(DMA1_Channel7, DISABLE);

		if(I2C1_getReadRegister() == MPU6050_WHO_AM_I) {
			test = (int16_t) I2C1_getRxBuffer(0);
		}
		// nacitame data do struktury
		if(I2C1_getReadRegister() == MPU6050_ACCEL_XOUT_H){
			data[0] = (int16_t) I2C1_getRxBuffer(0);
		}
		else if(I2C1_getReadRegister() == MPU6050_ACCEL_XOUT_L){
			data[1] = (int16_t) I2C1_getRxBuffer(0);
		}
		else if(I2C1_getReadRegister() == MPU6050_ACCEL_YOUT_H){
			data[2] = (int16_t) I2C1_getRxBuffer(0);
		}
		else if(I2C1_getReadRegister() == MPU6050_ACCEL_YOUT_L){
			data[3] = (int16_t) I2C1_getRxBuffer(0);
		}
		else if(I2C1_getReadRegister() == MPU6050_ACCEL_ZOUT_H){
			data[4] = (int16_t) I2C1_getRxBuffer(0);
		}
		else if(I2C1_getReadRegister() == MPU6050_ACCEL_ZOUT_L){
			data[5] = (int16_t) I2C1_getRxBuffer(0);
		}

		I2C1_clearReadRegister();
		I2C1_clearDeviceAddress();
	}
}


int main(void){
	uint16_t biela = decodeRgbValue(31, 31, 31);
	uint16_t cierna = decodeRgbValue(0, 0, 0);
	//uint16_t cervena = decodeRgbValue(31, 0, 0);
	uint16_t zelena = decodeRgbValue(0, 31, 0);
	//uint16_t modra = decodeRgbValue(0, 0, 31);
	char error = 0;
	MPU6050_t MPU6050_Data;

	// inicializacne funkcie
	initSPI2();
	initCD_Pin();
	initCS_Pin();
	initRES_Pin();
	initButton();
	initADC();
	//initUSART2();
	initI2C1();
	error = initMPU6050(&MPU6050_Data,MPU6050_Zariadenie_0,MPU6050_Akcelerometer_2G,MPU6050_Gyroskop_250s);
	if(error!=0){
		return error;
	}
	lcdInitialise(LCD_ORIENTATION0);
	clearDisplay(cierna);
	lcdMriezka3x3(54, 93, 6, biela, cierna);

	//hod kockou na zaklade akcelerometra
	lcdPutS("Nahodny generator", lcdTextX(1), lcdTextY(9), biela, cierna);
	lcdRectangle(45, 85, 83, 122, biela);
	////Mriezka pre stvorceky
	//lcdMriezka3x3(51, 90, 3, zelena, cierna);

	//Mriezka pre kruzky
	lcdMriezka3x3(54, 93, 6, zelena, cierna);

	//nahodny generator
	lcdPutS("Elektronicka hracia kocka", lcdTextX(1), lcdTextY(1), biela, cierna);
	lcdRectangle(45, 23, 83, 60, biela);
	uint32_t xxx = getTrueRandomNumber();
	xxx = getTrueRandomNumber();
	xxx = getTrueRandomNumber();

	while(1){
		MPU6050_readAcc((int16_t*)&data, &MPU6050_Data);
		////Mriezka pre stvorceky
		//lcdMriezka3x3(51, 28, i+1, biela, cierna);
		//Mriezka pre kruzky
		lcdMriezka3x3(54, 31, diceSide(&MPU6050_Data), biela, cierna);
	}
	return 0;
}

