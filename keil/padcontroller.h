#ifndef PAD_CONTROLLER_H
#define PAD_CONTROLLER_H

#include "driverlib/pin_map.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "delays.h"
#include <math.h>

void init_padController(void);
void setTarget_X1(int percent);
void setTarget_X2(int percent);

#endif

