
#ifndef SENSOR_L3GD20H_H
#define SENSOR_L3GD20H_H


#include <stdarg.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "sensorlib/i2cm_drv.h"


#include "sensorlib/l3gd20h.h"
#include "sensorlib/hw_l3gd20h.h"

#include "adafruit/Adafruit_Sensor.h"
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


class Sensor_L3GD20H_Gyro : public Adafruit_Sensor
{
  public:
    Sensor_L3GD20H_Gyro(tI2CMInstance* masterD);
		volatile int _defaultDone;
		float lastRead_gyro[3];
    void init(void);
    virtual bool getEvent(sensors_event_t*);
    virtual void getSensor(sensor_t*);
		void doRead(void);
	
  private:
		tI2CMInstance* _masterDriver;
		tL3GD20H _gyroscope;
		int _sensorID;
};

#endif
