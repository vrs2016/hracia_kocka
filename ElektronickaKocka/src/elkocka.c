/*
 * elkocka.c
 *
 *  Created on: Dec 18, 2016
 *      Author: Jany
 */
#include <string.h>

#include "ili9163.h"


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


