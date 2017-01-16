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
#define MPU6050_CLOCK_INTERNAL          0x00
#define MPU6050_CLOCK_PLL_XGYRO         0x01
#define MPU6050_CLOCK_PLL_YGYRO         0x02
#define MPU6050_CLOCK_PLL_ZGYRO         0x03
#define MPU6050_CLOCK_PLL_EXT32K        0x04
#define MPU6050_CLOCK_PLL_EXT19M        0x05
#define MPU6050_CLOCK_KEEP_RESET        0x07
#define MPU6050_GYRO_FS_250         0x00<<3
#define MPU6050_GYRO_FS_500         0x01<<3
#define MPU6050_GYRO_FS_1000        0x02<<3
#define MPU6050_GYRO_FS_2000        0x03<<3
#define MPU6050_ACCEL_FS_2          0x00<<3
#define MPU6050_ACCEL_FS_4          0x01<<3
#define MPU6050_ACCEL_FS_8          0x02<<3
#define MPU6050_ACCEL_FS_16         0x03<<3
#define MPU6050_DHPF_RESET          0x00
#define MPU6050_DHPF_5              0x01
#define MPU6050_DHPF_2P5            0x02
#define MPU6050_DHPF_1P25           0x03
#define MPU6050_DHPF_0P63           0x04
#define MPU6050_DHPF_HOLD           0x07
#define MPU6050_CLOCK_DIV_348       0x0
#define MPU6050_CLOCK_DIV_333       0x1
#define MPU6050_CLOCK_DIV_320       0x2
#define MPU6050_CLOCK_DIV_308       0x3
#define MPU6050_CLOCK_DIV_296       0x4
#define MPU6050_CLOCK_DIV_286       0x5
#define MPU6050_CLOCK_DIV_276       0x6
#define MPU6050_CLOCK_DIV_267       0x7
#define MPU6050_CLOCK_DIV_258       0x8
#define MPU6050_CLOCK_DIV_500       0x9
#define MPU6050_CLOCK_DIV_471       0xA
#define MPU6050_CLOCK_DIV_444       0xB
#define MPU6050_CLOCK_DIV_421       0xC
#define MPU6050_CLOCK_DIV_400       0xD
#define MPU6050_CLOCK_DIV_381       0xE
#define MPU6050_CLOCK_DIV_364       0xF

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
char initMPU6050(MPU6050_t* Data, MPU6050_Zariadenie_t DeviceNumber, MPU6050_Akcelerometer_t citlivostA, MPU6050_Gyroskop_t citlivostG);

// nacitame hodnoty akcelerometra
void MPU6050_readAcc(int16_t* data, MPU6050_t* Sensor);

#endif /* MPU6050_H_ */
