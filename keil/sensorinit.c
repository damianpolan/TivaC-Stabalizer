
#include "sensorinit.h"


//i2c master driver
tI2CMInstance masterDriver;


//accelerometer
tLSM303DLHCAccel accelerometer;
volatile bool defaultDone;
void* user_accelCallback;


void InitI2C0(void)
{
	//enable I2C module 0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

	//reset module
	SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);
	//enable GPIO peripheral that contains I2C 0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	// Configure the pin muxing for I2C0 functions on port B2 and B3.
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	GPIOPinConfigure(GPIO_PB3_I2C0SDA);
	// Select the I2C function for these pins.
	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

		// Enable and initialize the I2C0 master module.  Use the system clock for
	// the I2C0 module.  The last parameter sets the I2C data transfer rate.
	// If false the data rate is set to 100kbps and if true the data rate will
	// be set to 400kbps.
	//I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

	//clear I2C FIFOs
	//HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}




void I2CMIntHandler_wrapper(void){
	I2CMIntHandler(&masterDriver);
}

void
defaultCallback(void *pvCallbackData, uint_fast8_t ui8Status)
{
	// See if an error occurred.
	if(ui8Status != I2CM_STATUS_SUCCESS) {
		UARTprintf("Init Accel Error: %i\n", ui8Status);
		// An error occurred, so handle it here if required.
		while(1) {
			
		}
	}
	
	defaultDone = true; // Indicate that the LSM303DLHCAccel transaction has completed.
}


void initAccel(void) {	
	float fAccel[3];
	int count = 0;
	volatile int success = 0;
	
  UARTprintf("Init Accel\n");
	//initialize the accelerometer
	defaultDone = false;
	success = LSM303DLHCAccelInit(&accelerometer, &masterDriver, 0x19, defaultCallback, 0); //0x19 is the default I2C address of the LSM303DLHC device
	while(!defaultDone) {}
	
	//enable the accelerometer for 100HZ
	defaultDone = false;
	LSM303DLHCAccelReadModifyWrite(&accelerometer, LSM303DLHC_O_CTRL1,
	~0xFF,
	0x07 | LSM303DLHC_CTRL1_ODR_100HZ, //0x57
	defaultCallback,
	0);
	while(!defaultDone) {}
		
		
	//configure no high pass filter
	defaultDone = false;
	LSM303DLHCAccelReadModifyWrite(&accelerometer, LSM303DLHC_O_CTRL2,
	~0xFF,
	0x00, 
	defaultCallback,
	0);
	while(!defaultDone) {}
	
	// disable pin interrupts
	defaultDone = false;
	LSM303DLHCAccelReadModifyWrite(&accelerometer, LSM303DLHC_O_CTRL3,
	~0xFF,
	0x00, 
	defaultCallback,
	0);
	while(!defaultDone) {}	
		
	// continuous update, +/- 2G, high-res disabled
	defaultDone = false;
	LSM303DLHCAccelReadModifyWrite(&accelerometer, LSM303DLHC_O_CTRL4,
	~0xFF,
	0x00, 
	defaultCallback,
	0);
	while(!defaultDone) {}	
		
	// normal mode, FIFO disable, no latch interrupt, 4D disable
	defaultDone = false;
	LSM303DLHCAccelReadModifyWrite(&accelerometer, LSM303DLHC_O_CTRL5,
	~0xFF,
	0x00 | LSM303DLHC_CTRL5_REBOOTCTL_REBOOT, 
	defaultCallback,
	0);
	while(!defaultDone) {}	
		
	// no interrupt, reboot on PAD2
	defaultDone = false;
	LSM303DLHCAccelReadModifyWrite(&accelerometer, LSM303DLHC_O_CTRL6,
	~0xFF,
	0x00 | LSM303DLHC_CTRL6_BOOT_I2_EN, 
	defaultCallback,
	0);
	while(!defaultDone) {}	
		
	while(count < 10)
	{
		count++;
		// Request another reading from the LSM303DLHCAccel.
		defaultDone = false;
		LSM303DLHCAccelDataRead(&accelerometer, defaultCallback, 0);
		while(!defaultDone){}
		
		// Get the new accelerometer readings.
		LSM303DLHCAccelDataAccelGetFloat(&accelerometer, &fAccel[0], &fAccel[1], &fAccel[2]);
		
			
		UARTprintf("Read Accel: %f, %f, %f\n", fAccel[0], fAccel[1], fAccel[2]);
			
			
		cycles(10000000);
	}
	
	
	UARTprintf("Done.\n");	
}


tLSM303D accelerometer2;


void initAccel2(void) {	
	float fAccel[3];
	int count = 0;
	volatile int success = 0;
	
  UARTprintf("Init Accel\n");
	//initialize the accelerometer
	defaultDone = false;
	success = LSM303DInit(&accelerometer2, &masterDriver, 0x19, defaultCallback, 0); //0x19 is the default I2C address of the LSM303DLHC device
	while(!defaultDone) {}
	
	//enable the accelerometer for 100HZ
	defaultDone = false;
	LSM303DReadModifyWrite(&accelerometer2, LSM303DLHC_O_CTRL1,
	~0xFF,
	0x07 | LSM303DLHC_CTRL1_ODR_100HZ, //0x57
	defaultCallback,
	0);
	while(!defaultDone) {}
		
	//configure no high pass filter
	defaultDone = false;
	LSM303DReadModifyWrite(&accelerometer2, LSM303DLHC_O_CTRL2,
	~0xFF,
	0x00, 
	defaultCallback,
	0);
	while(!defaultDone) {}
	
	// disable pin interrupts
	defaultDone = false;
	LSM303DReadModifyWrite(&accelerometer2, LSM303DLHC_O_CTRL3,
	~0xFF,
	0x00, 
	defaultCallback,
	0);
	while(!defaultDone) {}	
		
	// continuous update, +/- 2G, high-res disabled
	defaultDone = false;
	LSM303DReadModifyWrite(&accelerometer2, LSM303DLHC_O_CTRL4,
	~0xFF,
	0x00, 
	defaultCallback,
	0);
	while(!defaultDone) {}	
		
	// normal mode, FIFO disable, no latch interrupt, 4D disable
	defaultDone = false;
	LSM303DReadModifyWrite(&accelerometer2, LSM303DLHC_O_CTRL5,
	~0xFF,
	0x00 | LSM303DLHC_CTRL5_REBOOTCTL_REBOOT, 
	defaultCallback,
	0);
	while(!defaultDone) {}	
		
	// no interrupt, reboot on PAD2
	defaultDone = false;
	LSM303DReadModifyWrite(&accelerometer2, LSM303DLHC_O_CTRL6,
	~0xFF,
	0x00 | LSM303DLHC_CTRL6_BOOT_I2_EN, 
	defaultCallback,
	0);
	while(!defaultDone) {}	
		
	while(count < 10)
	{
		count++;
		// Request another reading from the LSM303DLHCAccel.
		defaultDone = false;
		LSM303DDataRead(&accelerometer2, defaultCallback, 0);
		while(!defaultDone){}
		
		// Get the new accelerometer readings.
		LSM303DDataAccelGetFloat(&accelerometer2, &fAccel[0], &fAccel[1], &fAccel[2]);
		
			
		UARTprintf("Read Accel2: %f, %f, %f\n", fAccel[0], fAccel[1], fAccel[2]);
			
			
		cycles(10000000);
	}
	
	
	UARTprintf("Done.\n");	
}




tL3GD20H gyroscope;


void initGyro(void) {
	float fGyro[3];
	int count = 0;
	UARTprintf("Init Gyro\n");
	// Initialize the L3GD20H. This code assumes that the I2C master instance
	// has already been initialized.
	defaultDone = false;
	L3GD20HInit(&gyroscope, &masterDriver, 0x6B, defaultCallback, 0);
	while(!defaultDone){}
		
	//switch to normal mode and enable all 3 chanels
	defaultDone = false;
	L3GD20HReadModifyWrite(&gyroscope, L3GD20H_O_CTRL1,
	~0x0F,
	0x0F, defaultCallback,
	0);
	while(!defaultDone){}
		
	// Configure the L3GD20H for 500 deg/sec sensitivity
	defaultDone = false;
	L3GD20HReadModifyWrite(&gyroscope, L3GD20H_O_CTRL4,
	~L3GD20H_CTRL4_FS_M,
	L3GD20H_CTRL4_FS_500DPS, defaultCallback,
	0);
	while(!defaultDone){}
		
		
	while(count < 150)
	{
		count++;
		
		// Request another reading from the L3GD20H.
		defaultDone = false;
		L3GD20HDataRead(&gyroscope, defaultCallback, 0);
		while(!defaultDone){}
			
		// Get the new gyroscope readings.
		L3GD20HDataGyroGetFloat(&gyroscope, &fGyro[0], &fGyro[1], &fGyro[2]);
		UARTprintf("Read Gyro: %f, %f, %f\n", fGyro[0], fGyro[1], fGyro[2]);	
		
		cycles(1000000);
	}
	UARTprintf("Done.");	
}



void initSensors(void) {
	volatile int success = 0;
	
	
  FPULazyStackingEnable();
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);
	//initialze I2C and GPIO ports
	InitI2C0();
	
	// Register the interrupt handler
  I2CIntRegister(I2C0_BASE, I2CMIntHandler_wrapper);
	
	//initialize the I2C master driver
	I2CMInit(&masterDriver, I2C0_BASE, INT_I2C0, 0xff, 0xff, SysCtlClockGet()); //SysCtlClockGet()
	
	initAccel2();
	//initGyro();

}

