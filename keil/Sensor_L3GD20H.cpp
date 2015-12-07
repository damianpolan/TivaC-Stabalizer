
#include "Sensor_L3GD20H.h"


void defaultCallback_gyro(void *pvCallbackData, uint_fast8_t ui8Status)
{	
	// See if an error occurred.
	if(ui8Status != I2CM_STATUS_SUCCESS) {
		UARTprintf("Init Gyro Error: %i\n", ui8Status);
		// An error occurred, so handle it here if required.
		while(1) {}
	}
	((Sensor_L3GD20H_Gyro*) pvCallbackData)->_defaultDone = true; // Indicate that the LSM303DLHCAccel transaction has completed.
}


Sensor_L3GD20H_Gyro::Sensor_L3GD20H_Gyro( tI2CMInstance* masterD ) {
	_sensorID = 0xD5;
	_masterDriver = masterD;
}



void Sensor_L3GD20H_Gyro::init(void) {
	UARTprintf("Init Gyro\n");
	// Initialize the L3GD20H. This code assumes that the I2C master instance
	// has already been initialized.
	_defaultDone = false;
	L3GD20HInit(&_gyroscope, _masterDriver, 0x6B, defaultCallback_gyro, this);
	while(!_defaultDone){}
		
	//switch to normal mode and enable all 3 chanels
	_defaultDone = false;
	L3GD20HReadModifyWrite(&_gyroscope, L3GD20H_O_CTRL1,
	~0x0F,
	0x0F, defaultCallback_gyro,
	this);
	while(!_defaultDone){}
		
	// Configure the L3GD20H for 500 deg/sec sensitivity
	_defaultDone = false;
	L3GD20HReadModifyWrite(&_gyroscope, L3GD20H_O_CTRL4,
	~L3GD20H_CTRL4_FS_M,
	L3GD20H_CTRL4_FS_500DPS, defaultCallback_gyro,
	this);
	while(!_defaultDone){}	
}


void Sensor_L3GD20H_Gyro::doRead(void) {
		_defaultDone = false;
		L3GD20HDataRead(&_gyroscope, defaultCallback_gyro, this);
		while(!_defaultDone){}
		// Get the new gyroscope readings.
		L3GD20HDataGyroGetFloat(&_gyroscope, &lastRead_gyro[0], &lastRead_gyro[1], &lastRead_gyro[2]);
}



bool Sensor_L3GD20H_Gyro::getEvent(sensors_event_t* event) {

	
	return true;
}


void Sensor_L3GD20H_Gyro::getSensor(sensor_t* sensor) {

	
}

