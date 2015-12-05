
#include "Sensor_LSM303.h"

void defaultCallback_accel(void *pvCallbackData, uint_fast8_t ui8Status)
{	
	// See if an error occurred.
	if(ui8Status != I2CM_STATUS_SUCCESS) {
		UARTprintf("Init Accel Error: %i\n", ui8Status);
		// An error occurred, so handle it here if required.
		while(1) {}
	}
	((Sensor_LSM303_Accel*) pvCallbackData)->_defaultDone = true; // Indicate that the LSM303DLHCAccel transaction has completed.
}


Sensor_LSM303_Accel::Sensor_LSM303_Accel( tI2CMInstance* masterD ) {
	_sensorID = 0xD4;
	_masterDriver = masterD;
}



void Sensor_LSM303_Accel::init(void) {
	UARTprintf("Init Accel\n");
	int success = 0;
	//initialize the accelerometer
	_defaultDone = false;
	success = LSM303DInit(&_accelerometer, _masterDriver, 0x19, defaultCallback_accel, this); //0x19 is the default I2C address of the LSM303DLHC device
	while(!_defaultDone) {}
	
	//enable the accelerometer for 100HZ
	_defaultDone = false;
	LSM303DReadModifyWrite(&_accelerometer, LSM303DLHC_O_CTRL1,
	~0xFF,
	0x07 | LSM303DLHC_CTRL1_ODR_100HZ, //0x57
	defaultCallback_accel,
	this);
	while(!_defaultDone) {}
		
	//configure no high pass filter
	_defaultDone = false;
	LSM303DReadModifyWrite(&_accelerometer, LSM303DLHC_O_CTRL2,
	~0xFF,
	0x00, 
	defaultCallback_accel,
	this);
	while(!_defaultDone) {}
	
	// disable pin interrupts
	_defaultDone = false;
	LSM303DReadModifyWrite(&_accelerometer, LSM303DLHC_O_CTRL3,
	~0xFF,
	0x00, 
	defaultCallback_accel,
	this);
	while(!_defaultDone) {}	
		
	// continuous update, +/- 2G, high-res disabled
	_defaultDone = false;
	LSM303DReadModifyWrite(&_accelerometer, LSM303DLHC_O_CTRL4,
	~0xFF,
	0x00, 
	defaultCallback_accel,
	this);
	while(!_defaultDone) {}	
		
	// normal mode, FIFO disable, no latch interrupt, 4D disable
	_defaultDone = false;
	LSM303DReadModifyWrite(&_accelerometer, LSM303DLHC_O_CTRL5,
	~0xFF,
	0x00 | LSM303DLHC_CTRL5_REBOOTCTL_REBOOT, 
	defaultCallback_accel,
	this);
	while(!_defaultDone) {}	
		
	// no interrupt, reboot on PAD2
	_defaultDone = false;
	LSM303DReadModifyWrite(&_accelerometer, LSM303DLHC_O_CTRL6,
	~0xFF,
	0x00 | LSM303DLHC_CTRL6_BOOT_I2_EN, 
	defaultCallback_accel,
	this);
	while(!_defaultDone) {}	
		
	
}

void Sensor_LSM303_Accel::doRead(void) {
	_defaultDone = false;
	LSM303DDataRead(&_accelerometer, defaultCallback_accel, this);
	while(!_defaultDone){}
		
	// Get the new accelerometer readings.
	LSM303DDataAccelGetFloat(&_accelerometer, &lastRead_accel[0], &lastRead_accel[1], &lastRead_accel[2]);
	LSM303DDataMagnetoGetFloat(&_accelerometer, &lastRead_mag[0], &lastRead_mag[1], &lastRead_mag[2]);
		
	UARTprintf("Read Accel: %f, %f, %f\n", lastRead_accel[0], lastRead_accel[1], lastRead_accel[2]);
	UARTprintf("Read Mag: %f, %f, %f\n", lastRead_mag[0], lastRead_mag[1], lastRead_mag[2]);
}


bool Sensor_LSM303_Accel::getEvent(sensors_event_t* event) {
  memset(event, 0, sizeof(sensors_event_t));
			
	event->version   = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->type      = SENSOR_TYPE_ACCELEROMETER;
  event->timestamp = 1;//clock();
  event->acceleration.x = lastRead_accel[0];
  event->acceleration.y = lastRead_accel[1];
  event->acceleration.z = lastRead_accel[2];
	
	return true;
}


void Sensor_LSM303_Accel::getSensor(sensor_t* sensor) {
	memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy (sensor->name, "LSM303", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version     = 1;
  sensor->sensor_id   = _sensorID;
  sensor->type        = SENSOR_TYPE_ACCELEROMETER;
  sensor->min_delay   = 0;
  sensor->max_value   = 0.0F; // TBD
  sensor->min_value   = 0.0F; // TBD
  sensor->resolution  = 0.0F; // TBD	
	
}





/****************************************************************************************************
MAG
*****************************************************************************************************/


Sensor_LSM303_Mag::Sensor_LSM303_Mag( Sensor_LSM303_Accel* accel ) {
	_sensorID = 0xD4;
	_accel = accel;
}

void Sensor_LSM303_Mag::init(void) {
	UARTprintf("Init Mag\n");	
}



bool Sensor_LSM303_Mag::getEvent(sensors_event_t* event) {
			
  memset(event, 0, sizeof(sensors_event_t));
	event->version   = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->type      = SENSOR_TYPE_MAGNETIC_FIELD;
  event->timestamp = 1;//clock();
  event->magnetic.x = _accel->lastRead_mag[0];
  event->magnetic.y = _accel->lastRead_mag[1];
  event->magnetic.z = _accel->lastRead_mag[2];
	
	return true;
}


void Sensor_LSM303_Mag::getSensor(sensor_t* sensor) {
	 /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy (sensor->name, "LSM303", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version     = 1;
  sensor->sensor_id   = _sensorID;
  sensor->type        = SENSOR_TYPE_MAGNETIC_FIELD;
  sensor->min_delay   = 0;
  sensor->max_value   = 0.0F; // TBD
  sensor->min_value   = 0.0F; // TBD
  sensor->resolution  = 0.0F; // TBD
	
}








