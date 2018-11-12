#include "UART.h"

char readChar(void)
{
		char c;
		int i;
		//Enquanto tiver algo no receive register
		while((UART0->FR & (1<<4)) != 0 );
		//Delay pra n�o dar problema
		for(i=0;i<1000;i++);
		c = UART0->DR;
		for(i=0;i<1000;i++);
		return c;
}

void printChar(char c)
{
	int i;
	//Enquanto tiver algo no transmit register
	while((UART0->FR & (1<<5)) != 0);
	//Delay pra n�o dar problema
	for(i=0;i<1000;i++);
	UART0->DR = c;
	for(i=0;i<1000;i++);
	return;
}

void printString(char* string)
{
		while(*string)
		{
			printChar(*(string++));
		}
		return;
}





void UART_init(){
	int i=0;
	//Enable UART0 setting bit 0 on RCGUART
	SYSCTL_UART->RCGCUART|=(1<<0);
		for(i=0;i<1000;i++);
	;;;
	//Enable GPIO.
	SYSCTL_UART->RCGCGPIO|=(1<<0);
		for(i=0;i<1000;i++);
	;;;
	//Setting PA0 and PA1 for using alternate functions (AFSEL) and enabling digital logic (DEN).
	GPIOA_AHB->AFSEL	|= (1 << 0)|(1 << 1);
	;;;
	GPIOA_AHB->DEN	|= (1 << 0)|(1 << 1);
	;;;
	//The U0RX and UATX are connected to the PA0 and PA1, respectively. 
	//Both of then correspond to the bit 1 on their respective nibble on GPIOA_AHB.
	GPIOA_AHB->PCTL|= (1 << 0)|(1 << 4);
	;;;
	
	//Interrupt FIFO Level
	UART0->IFLS|=(1 << 4); //RX FIFO >=1/2 Full
	;;;
	//INTERRUPT Mask
	UART0->IM|=(1 << 4); //Interrupt on receive 	
	;;;

	//Disabling UART and making sure HSE mode is off
	UART0->CTL &=~ ((1 << 0)|(1 << 5));
	;;;

	//Setting Baud rate of 115200
	UART0->IBRD = 8;//58;
	;;;
	UART0->FBRD = 54;//38;
	;;;
	//BITS(7-SPS, 6:5 - WLEN, 4 - FEN, 3 - STP2, 2 - EPS,1 - PEN, 0 - BRK)
	//SETTING 0x0110000 for data length of 8 bits (WLEN-11), One stop bit (STP2-0), no parity(EPS-0), no fifo (FEN-0)
	UART0->LCRH = 0x60;//|(1 << 4);
	;;;
	//0x00 for system clock.
	UART0->CC  = 0x00;
	;;;
	//Enable UART, setting UARTEN bit
	UART0->CTL  |= (1 << 0);
	;;;
	//Enabling interrupt on NVIC
	NVIC_UART->ISER[0]|=(1 << 5);
	;;;
	//Setting priority
	NVIC_UART->IP[5]= 0x00;
	return;
}




