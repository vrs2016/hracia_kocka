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

uint8_t I2C1_getDeviceAddress(void);

void I2C1_clearDeviceAddress(void);

uint8_t I2C1_getReadRegister(void);

void I2C1_clearReadRegister(void);

uint32_t I2C1_getRxBuffer(int index);

void I2C1_DMA_Read(uint8_t slaveAddr, uint8_t readAddr,uint8_t numberBytesReceive);

// na zaklade akcelerometra zistime stranu kocky
char diceSide(MPU6050_t* Sensor);
// funkcia na inicializaciu I2C
void initI2C1(void);
// inicializacia ADC prevodnika
void initADC(void);
// inicializacia tlacidla pre spustenie prevodu
void initButton(void);
// funkcia na inicializaciu USART
void initUSART2(void);
// funkcia na zapis na I2C
void I2C1_BytesWrite(uint8_t slaveAddr, uint8_t pBuffer[], uint8_t length,uint8_t writeAddr);
// funkcia na zmazanie displeja
void clearDisplay(uint16_t colour);
// funkcia na vykreslenie vyplneneho kruhu na displeji
void lcdFilledCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour);
// funkcia na vykreslenie mriezky 3x3 pre kocku na displeji
void lcdMriezka3x3(int16_t x0, int16_t y0, uint16_t cislo, uint16_t colour1, uint16_t colour2);
// posli string po seriovej zbernici USART2
void sendUSART2(char *s);

#endif /* ELKOCKA_H_ */
