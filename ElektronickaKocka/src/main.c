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

/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"

#include "spi.h"
#include "ssd1306.h"
#include "ili9163.h"
#include "elkocka.h"


int main(void)
{
  int i = 0;
  uint16_t biela = decodeRgbValue(31, 31, 31);
  uint16_t cierna = decodeRgbValue(0, 0, 0);
  uint16_t cervena = decodeRgbValue(31, 0, 0);
  uint16_t zelena = decodeRgbValue(0, 31, 0);
  uint16_t modra = decodeRgbValue(0, 0, 31);

  initSPI2();
  initCD_Pin();
  initCS_Pin();
  initRES_Pin();
  //ssd1306_init();

  	lcdInitialise(LCD_ORIENTATION0);

  	//lcdClearDisplay(cierna);
  	zmazDisplej(cierna);
  	lcdMriezka3x3(54, 93, 6, biela, cierna);

  	/*lcdLine(0, 0, 127, 127, decodeRgbValue(31, 31, 31));
  	lcdLine(0, 127, 127, 0, decodeRgbValue(31, 31, 31));
  	lcdCircle(64, 64, 32, decodeRgbValue(31, 0, 0));
  	lcdCircle(64, 64, 40, decodeRgbValue(0, 31, 0));
  	lcdCircle(64, 64, 48, decodeRgbValue(0, 0, 31));*/

  	//lcdPutS("Hello World!", lcdTextX(4), lcdTextY(0), decodeRgbValue(0, 0, 0), decodeRgbValue(31, 31, 31));

  	lcdPutS("The quick brown fox jumped over the lazy dog 0123456789", lcdTextX(1), lcdTextY(2), decodeRgbValue(255, 255, 255), decodeRgbValue(0, 0, 0));

  		//lcdPutS("Test", 23, 17, 0xFFFF, 0);


  	//lcdFilledRectangle(1, 64, 128, 128, decodeRgbValue(0, 0, 0));
  		//lcdRectangle(2, 65, 127, 127, decodeRgbValue(31, 31, 31));

	// Run the LCD test
	uint8_t ballX = 70, ballY = 70;
	int8_t ballSpeed = 1;
	int8_t xDir = ballSpeed, yDir = ballSpeed;

  /* Infinite loop */
  while (1)
  {
	  // Delete the ball
	  		//lcdFilledRectangle(ballX-2, ballY-1, ballX+2, ballY+2, decodeRgbValue(0, 0, 0));
	  		//lcdLine(1, 1, 128, 128, decodeRgbValue(0, 0, 25));
	  		//lcdPutS("o", ballX, ballY, 0, 0);

	  		// Delete the bat
	  		//lcdFilledRectangle(ballX-4, 124, ballX+4, 126, decodeRgbValue(0, 0, 0));

	  		// Move the ball
	  		ballX += xDir;
	  		ballY += yDir;

	  		// Range check
	  		if (ballX > 120) xDir = -ballSpeed;
	  		if (ballX < 8) xDir = ballSpeed;

	  		if (ballY > 120) yDir = -ballSpeed;
	  		if (ballY < 70) yDir = ballSpeed;

	  		// Plot the ball
	  		//lcdFilledRectangle(ballX-2, ballY-1, ballX+2, ballY+2, decodeRgbValue(31, 31, 31));

	  		//lcdPutS("o", ballX, ballY, 0xFFFF, 0);

	  		// Plot the bat
	  		//lcdFilledRectangle(ballX-4, 124, ballX+4, 126, decodeRgbValue(31, 0, 31));

	  		//lcdPutS("Hello World!", lcdTextX(4), lcdTextY(0), decodeRgbValue(0, 0, 0), decodeRgbValue(31, 31, 31));
	  		Delay(10);
  }
  return 0;
}
