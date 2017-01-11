#ifndef MPU6050_H_
#define MPU6050_H_

/* Standardna I2C addresa */
#define MPU6050_I2C_ADDR			0xD0

/* Who I am register */
#define MPU6050_I_AM				0x68

/* MPU6050 registre */
#define MPU6050_AUX_VDDIO			0x01
#define MPU6050_SMPLRT_DIV			0x19
#define MPU6050_CONFIG				0x1A
#define MPU6050_GYRO_CONFIG			0x1B
#define MPU6050_ACCEL_CONFIG		0x1C
#define MPU6050_MOTION_THRESH		0x1F
#define MPU6050_INT_PIN_CFG			0x37
#define MPU6050_INT_ENABLE			0x38
#define MPU6050_INT_STATUS			0x3A
#define MPU6050_ACCEL_XOUT_H		0x3B
#define MPU6050_ACCEL_XOUT_L		0x3C
#define MPU6050_ACCEL_YOUT_H		0x3D
#define MPU6050_ACCEL_YOUT_L		0x3E
#define MPU6050_ACCEL_ZOUT_H		0x3F
#define MPU6050_ACCEL_ZOUT_L		0x40
#define MPU6050_TEMP_OUT_H			0x41
#define MPU6050_TEMP_OUT_L			0x42
#define MPU6050_GYRO_XOUT_H			0x43
#define MPU6050_GYRO_XOUT_L			0x44
#define MPU6050_GYRO_YOUT_H			0x45
#define MPU6050_GYRO_YOUT_L			0x46
#define MPU6050_GYRO_ZOUT_H			0x47
#define MPU6050_GYRO_ZOUT_L			0x48
#define MPU6050_MOT_DETECT_STATUS	0x61
#define MPU6050_SIGNAL_PATH_RESET	0x68
#define MPU6050_MOT_DETECT_CTRL		0x69
#define MPU6050_USER_CTRL			0x6A
#define MPU6050_PWR_MGMT_1			0x6B
#define MPU6050_PWR_MGMT_2			0x6C
#define MPU6050_FIFO_COUNTH			0x72
#define MPU6050_FIFO_COUNTL			0x73
#define MPU6050_FIFO_R_W			0x74
#define MPU6050_WHO_AM_I			0x75

/* Citlivosti gyroskopu v °/s */
#define MPU6050_GYRO_SENS_250		((float) 131)
#define MPU6050_GYRO_SENS_500		((float) 65.5)
#define MPU6050_GYRO_SENS_1000		((float) 32.8)
#define MPU6050_GYRO_SENS_2000		((float) 16.4)

/* Citlivosti akcelerometra v g */
#define MPU6050_ACCE_SENS_2			((float) 16384)
#define MPU6050_ACCE_SENS_4			((float) 8192)
#define MPU6050_ACCE_SENS_8			((float) 4096)
#define MPU6050_ACCE_SENS_16		((float) 2048)


// MPU6050 moze mat dve slave adresy v zavislosti od AD0 pinu ( low - 0xD0 / high - 0xD2 )
typedef enum {
	MPU6050_Zariadenie_0 = 0,   // AD0 pin je low
	MPU6050_Zariadenie_1 = 0x02 // AD0 pin je high
} MPU6050_Zariadenie_t;


// parametre citlivost akcelerometra
typedef enum {
	MPU6050_Akcelerometer_2G = 0x00, // Rozsah je +- 2 [g]
	MPU6050_Akcelerometer_4G = 0x01, // Rozsah je +- 4 [g]
	MPU6050_Akcelerometer_8G = 0x02, // Rozsah je +- 8 [g]
	MPU6050_Akcelerometer_16G = 0x03 // Rozsah je +- 16 [g]
} MPU6050_Akcelerometer_t;


// parametre citlivosti gyroskopu
typedef enum {
	MPU6050_Gyroskop_250s = 0x00,  // Rozsah je +- 250 [°/s]
	MPU6050_Gyroskop_500s = 0x01,  // Rozsah je +- 500 [°/s]
	MPU6050_Gyroskop_1000s = 0x02, // Rozsah je +- 1000 [°/s]
	MPU6050_Gyroskop_2000s = 0x03  // Rozsah je +- 2000 [°/s]
} MPU6050_Gyroskop_t;


// hlavna datova struktura pre MPU6050
typedef struct {
	// hlavicka
	uint8_t Adresa;         // adresa zariadenia
	float Gyro_Mult;         // multiplikator pre gyroskop, sluzi na prevod jednotiek [°/s]
	float Akce_Mult;         // multiplikator pre akcelerometer, sluzi na prevod jednotiek [g]
	// datova cast
	int16_t Akcelerometer_X; // Hodnota z akcelerometra v X-ovej osi
	int16_t Akcelerometer_Y; // Hodnota z akcelerometra v Y-ovej osi
	int16_t Akcelerometer_Z; // Hodnota z akcelerometra v Z-ovej osi
	int16_t Gyroskop_X;     // Hodnota z gyroskopu v X-ovej osi
	int16_t Gyroskop_Y;     // Hodnota z gyroskopu v Y-ovej osi
	int16_t Gyroskop_Z;     // Hodnota z gyroskopu v Z-ovej osi
	float Teplota;       // Teplota [°C]
} MPU6050_t;

// inicializuje MPU6050 pred prvym pouzitimi a nastavi pozadovane citlivosti akcelerometra a gyroskopu
Status initMPU6050(MPU6050_t* Data, MPU6050_Zariadenie_t DeviceNumber, MPU6050_Akcelerometer_t citlivostA, MPU6050_Gyroskop_t citlivostG);

// nacita udaje z akcelerometra
/* VSTUPY */
// - *Data : pointer na MPU6050_t strukturu na ulozenie nacitanych udajov
/* NAVRATOVA HODNOTA */
// - MPU6050_Result_Ok : vsetko v poriadku
// - Ina : v pripade chyby
char nacitajMPU6050_Akcelerometer(MPU6050_t* Data);

// nacita udaje z gyroskopu
/* VSTUPY */
// - *Data : pointer na MPU6050_t strukturu na ulozenie nacitanych udajov
/* NAVRATOVA HODNOTA */
// - MPU6050_Result_Ok : vsetko v poriadku
// - Ina : v pripade chyby
char nacitajMPU6050_Gyroskop(MPU6050_t* Data);

// nacita udaje z teplotneho senzora
/* VSTUPY */
// - *Data : pointer na MPU6050_t strukturu na ulozenie nacitanych udajov
/* NAVRATOVA HODNOTA */
// - MPU6050_Result_Ok : vsetko v poriadku
// - Ina : v pripade chyby
char nacitajMPU6050_Teplotu(MPU6050_t* Data);

// nacita udaje z akcelerometra, gyroskopu a teplotneho senzora
/* VSTUPY */
// - *Data : pointer na MPU6050_t strukturu na ulozenie nacitanych udajov
/* NAVRATOVA HODNOTA */
// - MPU6050_Result_Ok : vsetko v poriadku
// - Ina : v pripade chyby
char nacitajMPU6050_Vsetko(MPU6050_t* Data);

#endif /* MPU6050_H_ */
