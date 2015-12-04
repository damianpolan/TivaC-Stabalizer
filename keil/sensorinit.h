#ifndef SENSOR_INIT_H
#define SENSOR_INIT_H

#include <stdarg.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "sensorlib/i2cm_drv.h"

//sensor provided
#include "sensorlib/hw_lsm303dlhc.h"
#include "sensorlib/lsm303dlhc_accel.h"
#include "sensorlib/l3gd20h.h"
#include "sensorlib/hw_l3gd20h.h"

#include "sensorlib/lsm303d.h"
#include "sensorlib/hw_lsm303d.h"

#include "stdbool.h"
#include "delays.h"

#include "inc/hw_i2c.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"

#include "driverlib/fpu.h"


void init_I2C(void);
void initSensors(void);

#endif
