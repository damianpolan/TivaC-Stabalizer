extern "C" {
	#include <math.h>
	#include "utils/uartstdio.h"
	#include "padcontroller.h"
}
#include "adafruit/Adafruit_Simple_AHRS.h"
#include "adafruit/Adafruit_Sensor.h"
#include "Sensor_LSM303.h"
#include "Sensor_L3GD20H.h"
//#include "sensorinit.h"
#include "sensorlib/i2cm_drv.h"

int defaultDone = false;
tI2CMInstance* masterDriver;
Sensor_LSM303_Accel* accel;
Sensor_L3GD20H_Gyro* gyro;


//GYRO SPECS:


#define GYRO_STAT_G0 0.055
#define GYRO_STAT_G1 -0.30
#define GYRO_STAT_G2 0.134

#define GYRO_MIN_G0 -8.413447
#define GYRO_MAX_G0 10.007195

#define GYRO_MIN_G1 -10.003530
#define GYRO_MAX_G1 9.884716

#define GYRO_MIN_G2 -7.459581
#define GYRO_MAX_G2 8.352666
/*
	g0 min/max:   -8.413447,  10.007195
	g1 min/max:   -10.003530, 9.884716
	g2 min/max:   -7.459581,  8.352666
	
	stationary:
	g0 =  0.055
	g1 = -0.30
	g2 =  0.134
*/



void
InitConsole(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, 16000000);
}




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
	I2CMIntHandler(masterDriver);
}

void InitI2CMaster(void) {	
	
	masterDriver = new tI2CMInstance();
	
  FPULazyStackingEnable();
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);
	//initialze I2C and GPIO ports
	InitI2C0();
	
	// Register the interrupt handler
  I2CIntRegister(I2C0_BASE, I2CMIntHandler_wrapper);
	
	//initialize the I2C master driver
	I2CMInit(masterDriver, I2C0_BASE, INT_I2C0, 0xff, 0xff, SysCtlClockGet()); //SysCtlClockGet()
	
}


int main(void) {
	InitConsole();
	InitI2CMaster();
	
	init_padController();
		
	accel = new Sensor_LSM303_Accel(masterDriver);
	gyro = new Sensor_L3GD20H_Gyro(masterDriver);
	
	accel->init();	
	gyro->init();
	
	
	float currPerc_X1 = 50;
	float currPerc_X2 = 50;
		
	
	
	while(1) {
		accel->doRead();
		gyro->doRead();
				
		
		//with the accelerometer we want to minimiz x and y values
		float x = accel->lastRead_accel[0]; //x2 axis
		float y = accel->lastRead_accel[1]; //x1 axis
		float z = accel->lastRead_accel[2];
		//UARTprintf("Read Accel:  %f, %f, %f\n", x, y, z);
		
		
		
		// if the acceleration changes but the gyroscope does not, the increment change should not be significant
		float g0 = gyro->lastRead_gyro[0] - GYRO_STAT_G0; //x1 axis
		float g1 = gyro->lastRead_gyro[1] - GYRO_STAT_G1; //x2 axis
		//float g2 = gyro->lastRead_gyro[2] - GYRO_STAT_G2;
		//UARTprintf("Read Gyro:   %f, %f, %f\n", g0, g1, g2);

			
		//the total amount to shift the motor
		float x1Shift = 0;
		float x2Shift = 0;
		
		//currently not used
		//linear uniform distribution of the gx value between its max and min. Gives a value between -1.0 and 1.0
		//float g0_lin_percent = 2.0f * (1.0f - (GYRO_MAX_G0 - g0) / (GYRO_MAX_G0 - GYRO_MIN_G0)) - 1.0f; //x1 
		//float g1_lin_percent = 2.0f * (1.0f - (GYRO_MAX_G1 - g1) / (GYRO_MAX_G1 - GYRO_MIN_G1)) - 1.0f; //x2
		//UARTprintf("G0, G1:   %f, %f,\n", g0_lin_percent, g1_lin_percent);
				
				
		if(abs(y) < 2)
			x1Shift += (y / 2.7f); //2.7 good
		else 
			x1Shift += (y / 2.0f); //2.7 good
		
		if(abs(x) < 2)
			x2Shift -= (x / 2.7f);
		else 
			x2Shift -= (x / 2.0f);
	
		//set servo positions
		currPerc_X1 = setTarget_X1(currPerc_X1 + x1Shift);
		currPerc_X2 = setTarget_X2(currPerc_X2 + x2Shift);
		
		
//		float maxShift = x1Shift;
//		if(x2Shift > maxShift)
//			maxShift = x2Shift;
		//short delay to allow the servos to catch up
		cycles(10000);
	}
		
	return 0;
}
