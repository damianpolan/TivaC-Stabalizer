#include "UART_log.h"


//------------UART_Init------------
// Initialize the UART for 115,200 baud rate (assuming 16 MHz UART clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
// Output: none
void UART_Init(void){
  SYSCTL -> RCGCUART |= SYSCTL_RCGCUART_R0; // activate UART0
  SYSCTL -> RCGCGPIO |= SYSCTL_RCGCGPIO_R0; // activate port A
  UART0 -> CTL &= ~UART_CTL_UARTEN;        // disable UART
  UART0 -> IBRD = 8;                       // IBRD = int(16,000,000 / (16 * 115,200)) = int(8.680555) = 8
  UART0 ->FBRD  = 44;                      // FBRD = int(0.680555 * 64 + 0.5) = 44
                                           // 8 bit word length (no parity bits, one stop bit, enable FIFOs)
  UART0 -> LCRH = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  GPIOA -> AMSEL &= ~0x03;                // disable analog functionality on PA
  GPIOA -> AFSEL |= 0x03;                 // enable alt funct on PA1-0
  GPIOA -> DEN |= 0x03;                    // enable digital I/O on PA1-0
                                           // configure PA1-0 as UART   (just in case)
  GPIOA -> PCTL = (GPIOA -> PCTL & 0xFFFFFF00)+0x00000011;
	UART0 -> CTL |= UART_CTL_UARTEN;         // enable UART
}

//------------UART_InChar------------
// Wait for serial port input
// Input: none
// Output: ASCII code for chatracter received 
unsigned char UART_InChar(void){
	// wait for data in Rx FIFO
  while((UART0 -> FR & UART_FR_RXFE) != 0);    // empty loop body -- just wait
	// read a char from Rx FIFO
  return((unsigned char)(UART0 -> DR & 0xFF));
}
//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: data is an 8-bit ASCII character to be transmitted
// Output: none
void UART_OutChar(unsigned char data){
  // wait for space in Tx FIFO
  while((UART0 -> FR & UART_FR_TXFF) != 0);
  // write char to Tx FIFO
  UART0 -> DR = data;
}


void UART_log(char* message, int length) {
	int i;
	for (i = 0; i < length; i++) {
		UART_OutChar(message[i]);
	}	
}



