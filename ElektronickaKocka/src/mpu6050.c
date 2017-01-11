#include "i2c.h"
#include "stm32l1xx.h"
#include "mpu6050.h"

Status initMPU6050(MPU6050_t* Data, MPU6050_Zariadenie_t DeviceNumber, MPU6050_Akcelerometer_t citlivostA, MPU6050_Gyroskop_t citlivostG){
	uint8_t temp;
	Status stat;

	// vypocitaj I2C adresu na zaklade cisla zariadenia
	Data->Adresa = MPU6050_I2C_ADDR | (uint8_t)DeviceNumber;

	// zobudime zariadenie MPU6050 zo sleep rezimu
	stat = writeByteI2C1(Data->Adresa,MPU6050_PWR_MGMT_1,0x00);
	if (stat!=Success){
		// ak nastala chyba, tak ukonci a vrat cislo chyby
		return stat;
	}

	// konfiguracia akcelerometra
	stat = readByteI2C1(Data->Adresa,MPU6050_ACCEL_CONFIG,&temp);
	if (stat!=Success){
		// ak nastala chyba, tak ukonci a vrat cislo chyby
		return stat;
	}
	// nastavime zvolenu citlivost akcelerometra
	temp = (temp & 0xE7) | (uint8_t)citlivostA << 3;
	stat = writeByteI2C1(Data->Adresa,MPU6050_ACCEL_CONFIG,temp);
	if (stat!=Success){
		// ak nastala chyba, tak ukonci a vrat cislo chyby
		return stat;
	}

	// konfiguracia gyroskopu
	stat = readByteI2C1(Data->Adresa,MPU6050_GYRO_CONFIG,&temp);
	if (stat!=Success){
		// ak nastala chyba, tak ukonci a vrat cislo chyby
		return stat;
	}
	// nastavime zvolenu citlivost gyroskopu
	temp = (temp & 0xE7) | (uint8_t)citlivostG << 3;
	stat = writeByteI2C1(Data->Adresa,MPU6050_GYRO_CONFIG,temp);
	if (stat!=Success){
		// ak nastala chyba, tak ukonci a vrat cislo chyby
		return stat;
	}

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
	return Success;
}

char nacitajMPU6050_Akcelerometer(MPU6050_t* Data) {
	uint8_t data[6];

	// nacitame aktualnu hodnotu z akcelerometra
	char error = readArrayI2C1(Data->Adresa,data,MPU6050_ACCEL_XOUT_H,6);

	// spojime horny a dolny byte do jedneho 16-bitoveho cisla a ulozime do struktury
	Data->Akcelerometer_X = (int16_t)(data[0] << 8 | data[1]);
	Data->Akcelerometer_Y = (int16_t)(data[2] << 8 | data[3]);
	Data->Akcelerometer_Z = (int16_t)(data[4] << 8 | data[5]);

	// navratova hodnota
	return error;
}

char nacitajMPU6050_Gyroskop(MPU6050_t* Data) {
	uint8_t data[6];

	// nacitame aktualnu hodnotu z gyroskopu
	char error = readArrayI2C1(Data->Adresa,data,MPU6050_GYRO_XOUT_H,6);

	// spojime horny a dolny byte do jedneho 16-bitoveho cisla a ulozime do struktury
	Data->Gyroskop_X = (int16_t)(data[0] << 8 | data[1]);
	Data->Gyroskop_Y = (int16_t)(data[2] << 8 | data[3]);
	Data->Gyroskop_Z = (int16_t)(data[4] << 8 | data[5]);

	// navratova hodnota
	return error;
}

char nacitajMPU6050_Teplotu(MPU6050_t* Data) {
	uint8_t data[2];
	int16_t temp;

	// nacitame aktualnu hodnotu z tepelneho senzora
	char error = readArrayI2C1(Data->Adresa,data,MPU6050_TEMP_OUT_H,2);

	// spojime horny a dolny byte do jedneho 16-bitoveho cisla a ulozime do struktury
	temp = (data[0] << 8 | data[1]);
	// prepocitame na stupne celzia podla datasheetu
	Data->Teplota = (float)((int16_t)temp / (float)340.0 + (float)36.53);

	// navratova hodnota
	return error;
}

char nacitajMPU6050_Vsetko(MPU6050_t* Data) {
	uint8_t data[14];
	int16_t temp;

	// nacitaj udaje zo vsetkych senzorov (14 bytov)
	char error = readArrayI2C1(Data->Adresa,data,MPU6050_ACCEL_XOUT_H,14);

	// spojime horny a dolny byte do jedneho 16-bitoveho cisla a ulozime do struktury
	Data->Akcelerometer_X = (int16_t)(data[0] << 8 | data[1]);
	Data->Akcelerometer_Y = (int16_t)(data[2] << 8 | data[3]);
	Data->Akcelerometer_Z = (int16_t)(data[4] << 8 | data[5]);

	temp = (data[6] << 8 | data[7]);
	// prepocitame na stupne celzia podla datasheetu
	Data->Teplota = (float)((float)((int16_t)temp) / (float)340.0 + (float)36.53);

	// spojime horny a dolny byte do jedneho 16-bitoveho cisla a ulozime do struktury
	Data->Gyroskop_X = (int16_t)(data[8] << 8 | data[9]);
	Data->Gyroskop_Y = (int16_t)(data[10] << 8 | data[11]);
	Data->Gyroskop_Z = (int16_t)(data[12] << 8 | data[13]);

	// navratova hodnota
	return error;
}


