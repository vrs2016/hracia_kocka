#include <string.h>
#include <stddef.h>
#include "stm32l1xx.h"
#include "mpu6050.h"
#include "ili9163.h"
#include "stm32l1xx.h"
#include "stm32l1xx_conf.h"
#include "stm32l1xx_i2c.h"


int32_t I2C_Rx_Buffer[10];
uint8_t deviceAddrUseI2c = 0;
uint8_t readReg = 0;

void I2C1_initDMA(void) {
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel7);

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) 0x40005410;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) I2C_Rx_Buffer;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


uint8_t I2C1_getDeviceAddress(void) {
	return deviceAddrUseI2c;
}


void I2C1_clearDeviceAddress(void) {
	deviceAddrUseI2c = 0;
}


int32_t I2C1_getRxBuffer(int index) {
	return I2C_Rx_Buffer[index];
}


uint8_t I2C1_getReadRegister(void) {
	return readReg;
}


void I2C1_clearReadRegister(void) {
	readReg = 0;
}


// vygeneruje nahodne cislo pomocou ADC prevodu a CRC
uint8_t getTrueRandomNumber(void) {
	// citame viackrat pre vyssi sum a nahodnost
	for (int i = 0; i < 10; i++) {
	    // zacni AD prevod vnutornej teploty procesora
	    ADC_SoftwareStartConv(ADC1);
	    // ak skoncil prevod
	    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {}
	    // vratime 10-tu hodnotu kontrolneho suctu
	    if (i>=9){
	    	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	    	// konvertujeme na cislo z rozsahu 1-6
	    	return (CRC_CalcCRC(ADC_GetConversionValue(ADC1))%6 + 1);
	    }
	    else {
	    	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	    	CRC_CalcCRC(ADC_GetConversionValue(ADC1));
	    }
	}
	return 0;
}


// inicializacia tlacidla pre spustenie prevodu
void initButton(void){
	// inicializacne struktura
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}


// inicializacia ADC prevodnika
void initADC(void){
	// inicializacne struktury
	ADC_InitTypeDef ADC_InitStructure;

	// zapneme RCC a PLL periferie
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	RCC_HSICmd(ENABLE);
	while (!RCC_GetFlagStatus(RCC_FLAG_HSIRDY));

	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	// povolime vnutorny kanal pre citanie teploty procesora
	ADC_TempSensorVrefintCmd(ENABLE);
	// zapneme AD prevodnik
	ADC_Cmd(ADC1, ENABLE);
	// prevadzame pri najvyssej rychlosti aby sme vygenerovali najvyssi sum
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16,1,ADC_SampleTime_4Cycles);
	// povolime hodiny pre RCC
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}


// inicializacia pre I2C zbernicu
void initI2C1(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;

	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);
	//I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C1_initDMA();
}


void I2C1_BytesWrite(uint8_t slaveAddr, uint8_t pBuffer[], uint8_t length,uint8_t writeAddr) {
	while (deviceAddrUseI2c != 0);
	deviceAddrUseI2c = slaveAddr;
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1, writeAddr);
	for (int i = 0; i < length; i++) {
		I2C_SendData(I2C1, pBuffer[i]);
		for (int x = 0; x < 1000; x++);
	}
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C1, ENABLE);
	I2C1_clearReadRegister();
	I2C1_clearDeviceAddress();
}


void I2C1_DMA_Read(uint8_t slaveAddr, uint8_t readAddr,uint8_t numberBytesReceive) {
	while (deviceAddrUseI2c != 0);
	deviceAddrUseI2c = slaveAddr;
	readReg = readAddr;
	// vypneme DMA kanal
	DMA_Cmd(DMA1_Channel7, DISABLE);
	// nastavime pocet bytov (mozne len po vypnuti kanala)
	DMA_SetCurrDataCounter(DMA1_Channel7, numberBytesReceive);
	// kym je zmernica zaneprazdnena
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	// povolime automaticke generovanie NACK
	I2C_DMALastTransferCmd(I2C1, ENABLE);
	// vygenerujeme zacatie komunikacie
	I2C_GenerateSTART(I2C1, ENABLE);
	// testujeme EV5 a vynulujeme
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	// odosleme adresu na zapis
	I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter);
	// testujeme EV6 a vynulujeme
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	// vynulujeme EV6 tym, ze nastavime PE bit
	I2C_Cmd(I2C1, ENABLE);
	// odosleme vnutornu adresu do ktorej budeme zapisovat
	I2C_SendData(I2C1, readAddr);
	// testujeme EV8 a vynulujeme
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	// vygenerujeme zacatie komunikacie po druhykrat
	I2C_GenerateSTART(I2C1, ENABLE);
	// testujeme EV5 a vynulujeme
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	// odosleme adresu na citanie
	I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Receiver);
	// testujeme EV6 a vynulujeme
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	// zapneme DMA pre zacatie komunikacie
	DMA_Cmd(DMA1_Channel7, ENABLE);
	I2C_DMACmd(I2C1, ENABLE);
}


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
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
}

// na zaklade akcelerometra zistime stranu kocky
char diceSide(MPU6050_t* Sensor){
	if ( (Sensor->Akcelerometer_X > 10000) && (Sensor->Akcelerometer_Y < 10000)
			&& (Sensor->Akcelerometer_Z < 10000) ){
		return 4;
	}
	else if( (Sensor->Akcelerometer_X < -10000) && (Sensor->Akcelerometer_Y > -10000)
			&& (Sensor->Akcelerometer_Z > -10000) ){
		return 3;
	}
	else if( (Sensor->Akcelerometer_Y > 10000) && (Sensor->Akcelerometer_X < 10000)
			&& (Sensor->Akcelerometer_Z < 10000) ){
		return 5;
	}
	else if( (Sensor->Akcelerometer_Y < -10000) && (Sensor->Akcelerometer_X > -10000)
			&& (Sensor->Akcelerometer_Z > -10000) ){
		return 2;
	}
	else if( (Sensor->Akcelerometer_Z > 10000) && (Sensor->Akcelerometer_X < 10000)
			&& (Sensor->Akcelerometer_Y < 10000) ){
		return 6;
	}
	else if( (Sensor->Akcelerometer_Z < -10000) && (Sensor->Akcelerometer_X > -10000)
			&& (Sensor->Akcelerometer_Y > -10000) ){
		return 1;
	}
	else{
		return -1;
	}
}


// posli string po seriovej zbernici USART2
void sendUSART2(char *s){
  // kym sme neposlali vsetky znaky
  while(*s){
	// pockaj kym mozes vysielat
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    // posli znak na zbernicu a posun smernik na dalsi znak
    USART_SendData(USART2, *s++);
  }
}


void clearDisplay(uint16_t colour){
	lcdFilledRectangle(0,0,129,130,colour);
}


void lcdFilledCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour){
	int16_t x = 0, y = radius;
	int16_t tmp_radius = radius;
	int16_t d = 0;

	for(int i = 0; i < radius; i++){
		d = 3 - (2 * tmp_radius);
		y = tmp_radius;
		x = 0;
		while(x <= y){
			lcdPlot(xCentre + x, yCentre + y, colour);
			lcdPlot(xCentre + y, yCentre + x, colour);
			lcdPlot(xCentre - x, yCentre + y, colour);
			lcdPlot(xCentre + y, yCentre - x, colour);
			lcdPlot(xCentre - x, yCentre - y, colour);
			lcdPlot(xCentre - y, yCentre - x, colour);
			lcdPlot(xCentre + x, yCentre - y, colour);
			lcdPlot(xCentre - y, yCentre + x, colour);

			if (d < 0) d += (4 * x) + 6;
			else{
				d += (4 * (x - y)) + 10;
				y -= 1;
			}
			x++;
		}
		tmp_radius--;
	}
}


void lcdMriezka3x3(int16_t x0, int16_t y0, uint16_t cislo, uint16_t colour1, uint16_t colour2){
	uint16_t i,j;
	uint16_t farba;

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


