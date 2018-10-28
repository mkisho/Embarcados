//#include "UART.h"
#include "TM4C1294NCPDT.h"
void UART_init1(){
	
	//Enable UART0 setting bit 0 on RCGUART
	SYSCTL->RCGCUART|=(1<<0);
	//Enable GPIO.
	SYSCTL->RCGCGPIO|=(1<<0);
	
	//Setting PA0 and PA1 for using alternate functions (AFSEL) and enabling digital logic (DEN).
	GPIOA_AHB->AFSEL	|= (1 << 0)|(1 << 1);
	GPIOA_AHB->DEN	|= (1 << 0)|(1 << 1);
	//The U0RX and UATX are connected to the PA0 and PA1, respectively. 
	//Both of then correspond to the bit 1 on their respective nibble on GPIOA_AHB.
	GPIOA_AHB->PCTL|= (1 << 0)|(1 << 4);
	
	
	//BAUDRATE BRD = 256000 
	//Baud BRD = BRDI + BRDF = UARTSysClk / (ClkDiv * Baud Rate)
	//BAUDRATED 58.59375
	//IBRD 58
	//FBRD 0.59375 -> integer(0.59375 * 64 + 0.5) = integer(38.5)= 38
	UART0->CTL &= ~(1 << 0);
	UART0->IBRD = 58;
	UART0->FBRD = 38;
	
	//BITS(7-SPS, 6:5 - WLEN, 4 - FEN, 3 - STP2, 2 - EPS,1 - PEN, 0 - BRK)
	//SETTING 0x0110000 for data length of 8 bits (WLEN-11), One stop bit (STP2-0), no parity(EPS-0), no fifo (FEN-0)
	// no interrupts (?)
	UART0->LCRH = 0x60;
	
	//0x00 for system clock.
	UART0->CC  = 0x00;
	
	//Enable UART, setting UARTEN bit
	UART0->CTL  |= (1 << 0);
}