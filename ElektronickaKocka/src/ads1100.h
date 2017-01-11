/**
  ******************************************************************************
  * @file    hw/i2c_devices/ads1100/inc/ads1100.h
  * @author  Jozef Rodina
  * @version V1.0.0
  * @date    23-October-2012
  * @brief   ADS1100 basic driver.
  ******************************************************************************
  *
  * <h2><center>&copy; COPYRIGHT 2012 Jozef Rodina</center></h2>
  ******************************************************************************
  */
#ifndef __ADS1100_H
#define __ADS1100_H

#include "i2c.h"

#define AD0

//ADDRESS IF AD0
#ifdef AD0
	#define ADS1100_ADDRESS_W 0x90
	#define ADS1100_ADDRESS_R 0x91
#endif

//ADDRESS IF AD1
#ifdef AD1
	#define ADS1100_ADDRESS_W 0x92
	#define ADS1100_ADDRESS_R 0x93
#endif

Status initADS1100(void);
Status readDataADS1100(unsigned int*data);

#endif
