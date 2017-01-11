/*
 * elkocka.c
 *
 *  Created on: Dec 18, 2016
 *      Author: Jany
 */
#include <string.h>
#include <stddef.h>
#include "stm32l1xx.h"
#include "ili9163.h"


// inicializacia pre USART
void initUSART2(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// nakonfigurujeme piny pre USART
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	// inicializacna struktura pre RX
	GPIO_InitTypeDef RXInitStr;

	RXInitStr.GPIO_Pin = GPIO_Pin_3;
	RXInitStr.GPIO_Mode = GPIO_Mode_AF;
	RXInitStr.GPIO_OType = GPIO_OType_PP;
	RXInitStr.GPIO_PuPd = GPIO_PuPd_NOPULL;
	RXInitStr.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA,&RXInitStr);

	// inicializacna struktura pre TX
	GPIO_InitTypeDef TXInitStr;

	TXInitStr.GPIO_Pin = GPIO_Pin_2;
	TXInitStr.GPIO_Mode = GPIO_Mode_AF;
	TXInitStr.GPIO_OType = GPIO_OType_PP;
	TXInitStr.GPIO_PuPd = GPIO_PuPd_NOPULL;
	TXInitStr.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA,&TXInitStr);

	// inicializacna struktura pre USART
	USART_InitTypeDef USART_InitStructure;
	// konfiguracia USART
	USART_InitStructure.USART_BaudRate = 2*9600;	// 2x kvoli kniznici
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
}

// posli string po seriovej zbernici USART2
void SendUSART2(char *s){
  // kym sme neposlali vsetky znaky
  while(*s){
	// pockaj kym mozes vysielat
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    // posli znak na zbernicu a posun smernik na dalsi znak
    USART_SendData(USART2, *s++);
  }
}

void zmazDisplej(uint16_t colour){
	lcdFilledRectangle(0,0,129,130,colour);
}

void lcdFilledCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour)
{
	int16_t x = 0, y = radius;
	int16_t tmp_radius = radius;
	int16_t d = 0;

	for(int i = 0; i < radius; i++){
		d = 3 - (2 * tmp_radius);
		y = tmp_radius;
		x = 0;
		   while(x <= y)
			{
				lcdPlot(xCentre + x, yCentre + y, colour);
				lcdPlot(xCentre + y, yCentre + x, colour);
				lcdPlot(xCentre - x, yCentre + y, colour);
				lcdPlot(xCentre + y, yCentre - x, colour);
				lcdPlot(xCentre - x, yCentre - y, colour);
				lcdPlot(xCentre - y, yCentre - x, colour);
				lcdPlot(xCentre + x, yCentre - y, colour);
				lcdPlot(xCentre - y, yCentre + x, colour);

				if (d < 0) d += (4 * x) + 6;
				else
				{
					d += (4 * (x - y)) + 10;
					y -= 1;
				}

				x++;
			}
		   tmp_radius--;
	}
}

void lcdMriezka3x3(int16_t x0, int16_t y0, uint16_t cislo, uint16_t colour1, uint16_t colour2)
{
	uint16_t i,j;
	uint16_t farba;

	/* 00 01 02
	 * 10 11 12
	 * 20 21 22
	 */

	for(i = 0; i < 3; i++){
		for(j = 0; j<3; j++){
			switch(cislo) {

			   case 1:
			      if(i == 1 && j == 1) farba = colour1;
				  else farba = colour2;
			      break;
			   case 2:
				  if((i == 0 && j == 0)||(i == 2 && j == 2)) farba = colour1;
				  else farba = colour2;
			      break;
			   case 3:
				  if(i == j) farba = colour1;
				  else farba = colour2;
				  break;
			   case 4:
			      if((i == 0 && j != 1) || (i == 2 && j != 1)) farba = colour1;
				  else farba = colour2;
				  break;
			   case 5:
				  if((i == 0 && j != 1) || (i == 2 && j != 1) || (i == 1 && j == 1)) farba = colour1;
				  else farba = colour2;
				  break;
			   case 6:
				  if(j != 1) farba = colour1;
				  else farba = colour2;
				  break;
			}

			//lcdStvorcek(x0 + j*10, y0 +i*10, farba);
			lcdFilledCircle(x0 + j*10, y0 +i*10, 3, farba);
		}
	}
}


