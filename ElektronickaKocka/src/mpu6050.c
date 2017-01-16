#include "stm32l1xx.h"
#include "mpu6050.h"
#include "ssd1306.h"
#include "elkocka.h"


// nacitame hodnoty akcelerometra
void MPU6050_readAcc(int16_t* data, MPU6050_t* Sensor){

	// nacitame registre
	I2C1_DMA_Read(MPU6050_I2C_ADDR, MPU6050_ACCEL_XOUT_H, 1);
	I2C1_DMA_Read(MPU6050_I2C_ADDR, MPU6050_ACCEL_XOUT_L, 1);
	I2C1_DMA_Read(MPU6050_I2C_ADDR, MPU6050_ACCEL_YOUT_H, 1);
	I2C1_DMA_Read(MPU6050_I2C_ADDR, MPU6050_ACCEL_YOUT_L, 1);
	I2C1_DMA_Read(MPU6050_I2C_ADDR, MPU6050_ACCEL_ZOUT_H, 1);
	I2C1_DMA_Read(MPU6050_I2C_ADDR, MPU6050_ACCEL_ZOUT_L, 1);

	// prepocet z registrov
	Sensor->Akcelerometer_X = (int16_t) (data[0] << 8 | data[1]);
	Sensor->Akcelerometer_Y = (int16_t) (data[2] << 8 | data[3]);
	Sensor->Akcelerometer_Z = (int16_t) (data[4] << 8 | data[5]);

}


char initMPU6050(MPU6050_t* Data, MPU6050_Zariadenie_t DeviceNumber, MPU6050_Akcelerometer_t citlivostA, MPU6050_Gyroskop_t citlivostG){
	uint8_t temp, buff = 0x00;
	char error = 0;

	// vypocitaj I2C adresu na zaklade cisla zariadenia
	Data->Adresa = MPU6050_I2C_ADDR | (uint8_t)DeviceNumber;

	// skontrolujeme, ci komunikujeme spravne
	I2C1_DMA_Read(Data->Adresa, MPU6050_WHO_AM_I, 1);
	Delay(50);
	temp = (uint8_t)I2C1_getRxBuffer(0);
	if(temp!=(uint8_t)MPU6050_I_AM){
		return 1;
	}

	// zobudime zo sleep rezimu
	I2C1_BytesWrite(Data->Adresa, &buff, 1, MPU6050_PWR_MGMT_1);

	// konfiguracia akcelerometra
	I2C1_DMA_Read(Data->Adresa, MPU6050_ACCEL_CONFIG, 1);
	Delay(50);
	temp = (uint8_t)I2C1_getRxBuffer(0);
	// nastavime zvolenu citlivost akcelerometra
	temp = (temp & 0xE7) | (uint8_t)citlivostA << 3;
	I2C1_BytesWrite(Data->Adresa, &temp, 1, MPU6050_ACCEL_CONFIG);

	// konfiguracia gyroskopu
	I2C1_DMA_Read(Data->Adresa, MPU6050_GYRO_CONFIG, 1);
	Delay(50);
	temp = (uint8_t)I2C1_getRxBuffer(0);
	// nastavime zvolenu citlivost gyroskopu
	temp = (temp & 0xE7) | (uint8_t)citlivostG << 3;
	I2C1_BytesWrite(Data->Adresa, &temp, 1, MPU6050_GYRO_CONFIG);

	// nastavime multiplikatory na prepocet jednotiek na zaklade zvolenej citlivosti pre akcelerometer
	switch (citlivostA) {
		case MPU6050_Akcelerometer_2G:
			Data->Akce_Mult = (float)1 / MPU6050_ACCE_SENS_2;
			break;
		case MPU6050_Akcelerometer_4G:
			Data->Akce_Mult = (float)1 / MPU6050_ACCE_SENS_4;
			break;
		case MPU6050_Akcelerometer_8G:
			Data->Akce_Mult = (float)1 / MPU6050_ACCE_SENS_8;
			break;
		case MPU6050_Akcelerometer_16G:
			Data->Akce_Mult = (float)1 / MPU6050_ACCE_SENS_16;
		default:
			break;
	}

	// nastavime multiplikatory na prepocet jednotiek na zaklade zvolenej citlivosti pre gyroskop
	switch (citlivostG) {
		case MPU6050_Gyroskop_250s:
			Data->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_250;
			break;
		case MPU6050_Gyroskop_500s:
			Data->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_500;
			break;
		case MPU6050_Gyroskop_1000s:
			Data->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_1000;
			break;
		case MPU6050_Gyroskop_2000s:
			Data->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_2000;
		default:
			break;
	}

	// ak je vsetko v poriadku
	return error;
}


