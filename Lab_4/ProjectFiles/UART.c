//#include "UART.h"
#include "TM4C1294NCPDT.h"
void UART_init(){
	SYSCTL->RCGCUART|=(1<<0);
	SYSCTL->RCGCGPIO|=(1<<0);
	
	//CHECK
	GPIOA_AHB->AFSEL	|= (1 << 0)|(1 << 1);
	GPIOA_AHB->PCTL		|= (1<<0)|(1<<4);
	
	//BAUDRATE 115200
	//BAUDRATED 65.1041667
	//IBRD 65
	//FBRD 0.1041667 -> integer(0.1041667 * 64 + 0.5) = integer(7.1666688)= 111b
	UART0->CTL  &= (0 << 0);
//	UART0->IBRD &= 	0xFF00;
//	UART0->IBRD |= 	(1 << 0)|(1 << 6);
//	UART0->FBRD &= 	(0 << 5)|(0 << 4)|(0 << 3);
//	UART0->FBRD |= 	(1 << 2)|(1 << 1)|(1 << 0);
	UART0->IBRD = 65;
	UART0->FBRD = 7;
	
	//BITS(7-SPS, 6:5 - WLEN, 4 - FEN, 3 - STP2, 2 - EPS,1 - PEN, 0 - BRK)
	//SETTING 0x0110000 for data length of 8 bits (WLEN-11), One stop bit (STP2-0), no parity(EPS-0), no fifo (FEN-0)
	// no interrupts (?)
	UART0->LCRH = 0x60;
	
	//0x00 for system clock.
	UART0->CC  = 0x00;
	
	
	//Enable UART, setting UARTEN bit
	UART0->CTL  |= (1 << 0);
}