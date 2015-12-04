
#include "sensorinit.h"


//i2c master driver
tI2CMInstance masterDriver;


//accelerometer
tLSM303DLHCAccel accelerometer;
volatile bool g_bLSM303DLHCAccelDone;
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
	I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

	//clear I2C FIFOs
	HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}




void I2CMIntHandler_wrapper(void){
	I2CMIntHandler(&masterDriver);
}

void
accelerometerCallback(void *pvCallbackData, uint_fast8_t ui8Status)
{
	// See if an error occurred.
	if(ui8Status != I2CM_STATUS_SUCCESS) {
		UARTprintf("Init Accel Error: %i\n", ui8Status);
		// An error occurred, so handle it here if required.
		while(1) {
			
		}
	}
	
	g_bLSM303DLHCAccelDone = true; // Indicate that the LSM303DLHCAccel transaction has completed.
}

int 
initSensors(void) {
	float fAccel[3];
	float rAccel[3];
	volatile int success = 0;
	InitI2C0();
		
	// Register the interrupt handler
  I2CIntRegister(I2C0_BASE, I2CMIntHandler_wrapper);
	
	
	success = SysCtlClockGet();
	//initialize the I2C master driver
	I2CMInit(&masterDriver, I2C0_BASE, INT_I2C0, 0xff, 0xff,  SysCtlClockGet()); //SysCtlClockGet()
	
	
  UARTprintf("Init Accel\n");
	//initialize the accelerometer
	g_bLSM303DLHCAccelDone = false;
	success = LSM303DLHCAccelInit(&accelerometer, &masterDriver, 0x19, accelerometerCallback, 0); //0x19 is the default I2C address of the LSM303DLHC device
	while(!g_bLSM303DLHCAccelDone) {}
		
	//single read mode
	g_bLSM303DLHCAccelDone = false;
	success = LSM303DLHCAccelReadModifyWrite(&accelerometer, 
			LSM303DLHC_O_MAG_MR, 
			~LSM303DLHC_MAG_MR_MODE_M,
			LSM303DLHC_MAG_MR_MODE_SLEEP, 
			accelerometerCallback, 0);
	while(!g_bLSM303DLHCAccelDone){}

		
		
	//enable x, y, z
	g_bLSM303DLHCAccelDone = false;
	success = LSM303DLHCAccelReadModifyWrite(&accelerometer, 
		LSM303DLHC_O_CTRL1,
		~LSM303DLHC_CTRL1_AXIS_M,
		LSM303DLHC_CTRL1_AXIS_Y_EN | LSM303DLHC_CTRL1_AXIS_X_EN | LSM303DLHC_CTRL1_AXIS_Z_EN, 
		accelerometerCallback, 0);
	while(!g_bLSM303DLHCAccelDone){}
	
	//reboot memory content
	g_bLSM303DLHCAccelDone = false;
	success = LSM303DLHCAccelReadModifyWrite(&accelerometer, 
		LSM303DLHC_O_CTRL5,
		~LSM303DLHC_CTRL5_REBOOTCTL_M,
		LSM303DLHC_CTRL5_REBOOTCTL_REBOOT, 
		accelerometerCallback, 0);
	while(!g_bLSM303DLHCAccelDone){}
		

		
	while(1)
	{
		// Request another reading from the LSM303DLHCAccel.
		g_bLSM303DLHCAccelDone = false;
		LSM303DLHCAccelDataRead(&accelerometer, accelerometerCallback, 0);
		while(!g_bLSM303DLHCAccelDone){}
		
		// Get the new accelerometer readings.
		LSM303DLHCAccelDataAccelGetFloat(&accelerometer, &fAccel[0], &fAccel[1], &fAccel[2]);
		
			
		UARTprintf("Read Accel: %f, %f, %f\n", fAccel[0], fAccel[1], fAccel[2]);
			
		//LSM303DLHCAccelDataAccelGetFloat(&accelerometer, &rAccel[0], &rAccel[1], &rAccel[2]);
		//UARTprintf("Read Accel: %f, %f, %f\n", rAccel[0], rAccel[1], rAccel[2]);
			
		cycles(10000000);
	}

}

