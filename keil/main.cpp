extern "C" {
	#include "utils/uartstdio.h"
	#include "padcontroller.h"
}
#include "adafruit/Adafruit_Simple_AHRS.h"
#include "adafruit/Adafruit_Sensor.h"
#include "Sensor_LSM303.h"
//#include "sensorinit.h"
#include "sensorlib/i2cm_drv.h"


int defaultDone = false;
tI2CMInstance* masterDriver;
Sensor_LSM303_Accel* accel;


//extern "C" int sendchar(int ch)
//{
//	// TODO
//	return 0;
//}

//extern "C" int getkey(void)
//{
//	// TODO
//	return 0;
//}

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
		
	accel = new Sensor_LSM303_Accel(masterDriver);
	accel->init();
	
	//Adafruit_Simple_AHRS ahrs(&accel, &mag);
	//initSensors();
	init_padController();
	
	int currPerc_X1 = 50;
	int currPerc_X2 = 50;
	
	int count = 0;
	while(count < 15) {
		accel->doRead();
		
		sensors_event_t s;
		accel->getEvent(&s);
		
		float x = accel->lastRead_accel[0];
		float y = accel->lastRead_accel[1];
		float z = accel->lastRead_accel[2];
		UARTprintf("Read Accel:  %f, %f, %f\n", x, y, z);
	
		currPerc_X1 += (int) -y;
		currPerc_X2 += (int) x;
		
		setTarget_X1(currPerc_X1);
		setTarget_X2(currPerc_X2);
	
		
		
		cycles(100000);
	}
	
	
	
	return 0;
}
