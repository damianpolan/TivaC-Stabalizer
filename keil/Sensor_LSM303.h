
#ifndef _SENSOR_LSM303_H
#define _SENSOR_LSM303_H


extern "C" {
#include <stdarg.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "sensorlib/i2cm_drv.h"

//sensor provided
//#include "sensorlib/hw_lsm303dlhc.h"
//#include "sensorlib/lsm303dlhc_accel.h"
//#include "sensorlib/l3gd20h.h"
//#include "sensorlib/hw_l3gd20h.h"

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

#include <string.h>
//#include <time.h>
}
#include "adafruit/Adafruit_Sensor.h"




class Sensor_LSM303_Accel : public Adafruit_Sensor
{
  public:
    Sensor_LSM303_Accel(tI2CMInstance* masterD);
		volatile int _defaultDone;
		float lastRead_accel[3];
		float lastRead_mag[3];
    void init(void);
    virtual bool getEvent(sensors_event_t*);
    virtual void getSensor(sensor_t*);
		void doRead(void);
	
  private:
		tI2CMInstance* _masterDriver;
		tLSM303D _accelerometer;
		int _sensorID;
};


class Sensor_LSM303_Mag : public Adafruit_Sensor
{
  public:
    Sensor_LSM303_Mag(Sensor_LSM303_Accel* accel);
    void init(void);
    virtual bool getEvent(sensors_event_t*);
    virtual void getSensor(sensor_t*);
		Sensor_LSM303_Accel* getAccel(void);

  private:
		int _sensorID;
		Sensor_LSM303_Accel* _accel;
};



#endif

