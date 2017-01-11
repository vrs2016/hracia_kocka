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

#include "stm32l1xx.h"
#include "spi.h"
#include "ssd1306.h"
#include "ili9163.h"
#include "elkocka.h"
#include "ads1100.h"
#include "i2c.h"


int main(void){
	uint16_t biela = decodeRgbValue(31, 31, 31);
	uint16_t cierna = decodeRgbValue(0, 0, 0);
	//uint16_t cervena = decodeRgbValue(31, 0, 0);
	uint16_t zelena = decodeRgbValue(0, 31, 0);
	//uint16_t modra = decodeRgbValue(0, 0, 31);
	unsigned int data;


	// inicializacne funkcie
	initSPI2();
	initCD_Pin();
	initCS_Pin();
	initRES_Pin();
	initUSART2();
	initI2C1();
	initADS1100();
	//ssd1306_init();

	lcdInitialise(LCD_ORIENTATION0);
	zmazDisplej(cierna);
	lcdMriezka3x3(54, 93, 6, biela, cierna);

	//hod kockou na zaklade akcelerometra
	lcdPutS("Hod kockou", lcdTextX(1), lcdTextY(9), biela, cierna);
	lcdRectangle(45, 85, 83, 122, biela);
	////Mriezka pre stvorceky
	//lcdMriezka3x3(51, 90, 3, zelena, cierna);

	//Mriezka pre kruzky
	lcdMriezka3x3(54, 93, 6, zelena, cierna);

	//nahodny generator
	lcdPutS("Nahodny generator", lcdTextX(1), lcdTextY(1), biela, cierna);
	lcdRectangle(45, 23, 83, 60, biela);

	while(1){
		// readDataADS1100(&data);
		for(int i=0;i<6;i++){
			////Mriezka pre stvorceky
			//lcdMriezka3x3(51, 28, i+1, biela, cierna);

			//Mriezka pre kruzky
			lcdMriezka3x3(54, 31, i+1, biela, cierna);
			for(int j=0;j<150;j++){
				Delay(1000);
				// pomocna funkcia na zobrazenie informacii odoslanim na seriovu linku
				SendUSART2("Test\n\r");
			}
		}
	}
	return 0;
}
