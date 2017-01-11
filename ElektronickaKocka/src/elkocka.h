/*
 * elkocka.h
 *
 *  Created on: Dec 18, 2016
 *      Author: Jany
 */

#ifndef ELKOCKA_H_
#define ELKOCKA_H_
//----------------------------
/*** Definicie funkcii ***/
//----------------------------
// funkcia na inicializaciu USART
void initUSART2(void);
// funkcia na zmazanie displeja
void zmazDisplej(uint16_t colour);
// funkcia na vykreslenie vyplneneho kruhu na displeji
void lcdFilledCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour);
// funkcia na vykreslenie mriezky 3x3 pre kocku na displeji
void lcdMriezka3x3(int16_t x0, int16_t y0, uint16_t cislo, uint16_t colour1, uint16_t colour2);
// posli string po seriovej zbernici USART2
void SendUSART2(char *s);

#endif /* ELKOCKA_H_ */
