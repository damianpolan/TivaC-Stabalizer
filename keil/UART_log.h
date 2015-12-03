#ifndef LOGGING_H
#define LOGGING_H


#include "CU_TM4C123.h"

void UART_Init(void);
unsigned char UART_InChar(void);
void UART_OutChar(unsigned char data);
void UART_log(char* message, int length);

#endif
