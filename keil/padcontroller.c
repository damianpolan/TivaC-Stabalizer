
#include "padcontroller.h"


#define SERVO_PERIOD 					5000
#define SERVO_DUTY 						250

#define SERVO_X1_RETRACTED 		SERVO_DUTY * 2.1
#define SERVO_X1_EXTENDED 		SERVO_DUTY

#define SERVO_X2_RETRACTED 		SERVO_DUTY / 2.2
#define SERVO_X2_EXTENDED 		SERVO_DUTY * 1.6



//Servo X1 -> PF2
//Servo X2 -> PF3
void initPWMs() {
	
	//Set the clock. Care here as we are also setting the clock in the sensor initializer
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC |   SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	
	//Configure PWM Clock divide system clock by 64
  SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

	//Enable the peripherals used by this program.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);  //The Tiva Launchpad has two modules (0 and 1). Module 1 covers the LED pins
	
	//Configure PF2,PF3 Pins as PWM
  GPIOPinConfigure(GPIO_PF2_M1PWM6);
  GPIOPinConfigure(GPIO_PF3_M1PWM7);
  GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);
	
	//Configure PWM Options
  //PWM_GEN_2 Covers M1PWM4 and M1PWM5
  //PWM_GEN_3 Covers M1PWM6 and M1PWM7
  PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); 
  PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); 
	
	//Set the Period (expressed in clock ticks)
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, SERVO_PERIOD);
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SERVO_PERIOD);
	
	//Set PWM duty
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,SERVO_DUTY);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,SERVO_DUTY);

	// Enable the PWM generator
	PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);

	// Turn on the Output pins
	PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);
	 
}


//sets the target for servo x1.
// param percent: Value between 0 and 100. 0 -> arm retracted, 100 -> arm extended
float setTarget_X1(float percent) {
	if(percent > 100)
		percent = 100;
	if(percent < 0)
		percent = 0;
	
	float value = SERVO_X1_RETRACTED - (((SERVO_X1_RETRACTED) - (SERVO_X1_EXTENDED)) * (percent / 100.0f));	 
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, value);
	
	return percent;
}

//sets the target for servo x2.
// param percent: Value between 0 and 100. 0 -> arm retracted, 100 -> arm extended
float setTarget_X2(float percent) {
	if(percent > 100)
		percent = 100;
	if(percent < 0)
		percent = 0;
	
	float value = SERVO_X2_RETRACTED + (((SERVO_X2_EXTENDED) - (SERVO_X2_RETRACTED)) * (percent / 100.0f));
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, value);
	
	return percent;
}

void init_padController() {
	initPWMs();
	
//	while(1)
//	{
//		setTarget_X1(50);
//		
//		setTarget_X2(50);
//    //PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,SERVO_X1_RETRACTED);
//		
//    //PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,SERVO_X2_RETRACTED);
//		cycles(10000000);
//		
//		setTarget_X2(100);
//    //PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,SERVO_X1_MIN);
//    //PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,SERVO_X2_EXTENDED);
//    //PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,SERVO_X1_EXTENDED);
//		cycles(10000000);
//		
//	}
}