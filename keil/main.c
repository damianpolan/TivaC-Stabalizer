#include "sensorinit.h"
//#include "utils/uartstdio.c"
//#include "UART_log.h"

//void
//InitConsole(void)
//{
//    UARTprintf("Init Console\n");
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
//    GPIOPinConfigure(GPIO_PA0_U0RX);
//    GPIOPinConfigure(GPIO_PA1_U0TX);
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
//    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
//    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//    UARTStdioConfig(0, 115200, 16000000);
//}


int main(void) {
	//UART_Init();
	
	initSensors();
	
	return 0;
}
