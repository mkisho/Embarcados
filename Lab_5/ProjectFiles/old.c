
/*

char readChar(void)
{
		char c;
		while((UART0->FR & (1<<4)) != 0 );
		c = UART0->DR;
		return c;
}

void printChar(char c)
{
	while((UART0->FR & (1<<5)));
	UART0->DR = c;
}

void printString(char* string)
{
		while(*string)
		{
			printChar(*(string++));
		}
}






void PWM_init(){
	//Enable and provide a clock to PWM module 0 in Run mode
	SYSCTL->RCGCPWM |= (1 << 0);
	
	//Habilitando Port F
	SYSCTL->RCGCGPIO |= (1 << 5);
	
	//Setting PF0 for using alternate functions (AFSEL) and enabling digital logic (DEN).
	GPIOF_AHB->AFSEL|= (1 << 0);
	GPIOF_AHB->DEN	|= (1 << 0);
	GPIOF_AHB->AFSEL|= (1 << 2);
	GPIOF_AHB->DEN	|= (1 << 2);
	GPIOF_AHB->AFSEL|= (1 << 1);
	GPIOF_AHB->DEN	|= (1 << 1);
	//The M0PWM0 is connected to the PF0. 
	//It corresponds to the value on his respective nibble on GPIOF_AHB.
	GPIOF_AHB->PCTL	|= (1 << 2)|(1 << 1);
	GPIOF_AHB->PCTL	|= (1 << 10)|(1 << 9);
	GPIOF_AHB->PCTL	|= (1 << 5)|(1 << 6);
	
	//bit 8 (USEPWM = 1, use pwm clock divisor), birt 2:0 (PWMDIV=0, for /2) 
	PWM0->CC &=~ ((1 << 0)|(1 << 1)|(1 << 2));	
	PWM0->CC |= (1 << 8);
		
	PWM0->_0_CTL &= 0x0;
	PWM0->_1_CTL &= 0x0;
	//Setting actions taken by pwm for each event.
	//11:10  ACTCMPBD
	//9:8 ACTCMPBU
	//7:6 ACTCMPAD
	//5:4 ACTCMPAU 
	//3:2 ACTLOAD
	//1:0 ACTZERO
	//0-nothing; 1-invert; 2-drive low; 3-drive high
	//0x0000.008C-> 1000 1100 -> Drive high on load, drive low on comp A Down. For everything else, do nothing
	PWM0->_0_GENA = 0x8C;
	PWM0->_0_GENB = 0x8C;
//	PWM0->_1_GENA = 0x8C;
		PWM0->_1_GENA = 0xC8;
	//pwm frequency
	//0x0000.018F for 60KHz. Load is a 16 bit value
	//Clock source is 120MHz/2=60MHz.
	//For 200Hz of PWM cycle, 300.000 clock ticks
	//For 1Hz cycle, 60.000.000 clock ticks
	//Start with 200Hz
	PWM0->_0_LOAD = 3001;
	PWM0->_1_LOAD = 3000;	
//	PWM0->_2_LOAD = 3000000;
	
	//A and B values. This sets the duty cicle
	//A=150000 for 50% duty cicle.
	PWM0->_0_CMPA=1500;
	PWM0->_0_CMPB=2900;
	PWM0->_1_CMPA=1500;
	//Enable timers
	PWM0->_0_CTL |= (1<<0);
	PWM0->_1_CTL |= (1<<0);
//	SYSCTL->SRPWM &=~ (1<<0);
	//Enable outputs
	PWM0->ENABLE|= (1<<0)|(1<<1)|(1<<2);
}

void PWM_set_duty(uint16_t n){
	char pBuf[30];
//	PWM0->_0_CTL &= 0x0;
//	PWM0->_1_CTL &= 0x0;
//	n=n*30;
	PWM0->_1_CMPA=(n);
//	intToString(n,pBuf,15,10,5);
//	printString(pBuf);
//	printString("\n");

	
	//	PWM0->_0_CTL |= (1<<0);
//	PWM0->_1_CTL |= (1<<0);
}




void timer_init(){
	printString("OMAE WA MOU SHINDEIRU1\n");	
//	IRQn_Type->TIMER0A_IRQn;// |=(1<<19);
	
	printString("OMAE WA MOU SHINDEIRU2\n");	
	SYSCTL->RCGCTIMER|=(1<<0);
	//Disable timer
	printString("OMAE WA MOU SHINDEIRU3\n");	
	TIMER0->CTL &=~(1<<0);
	printString("OMAE WA MOU SHINDEIRU4\n");	
	TIMER0->CFG 	= 0x0;
	TIMER0->TAMR	= 0x2;
	printString("OMAE WA MOU SHINDEIRU5\n");	
	//LOAD value
	TIMER0->TAILR	= 3000;//6000;
	//bit 4 interrput match. bit 0 timeout interrupt mask
	TIMER0->IMR 	= 0x1;
	//Enable timer
	TIMER0->CTL 	|= (1<<0);
	printString("OMAE WA MOU SHINDEIRU6\n");	
	//Clear when interrupt
//	TIMER0->ICR 	&= 0x0;

	TIMER0->CC &=~(1<<0);
	NVIC->ISER[0]|=0x00080000;
//	NVIC->IP[0] = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // 8) priority 2
// NVIC_EN0_R |= NVIC_EN0_INT19;    // 9) enable interrupt 19 in NVIC
	
}

void timer_change(uint32_t n){
	//Disable timer
	TIMER0->CTL &=~(1<<0);
	//LOAD value
	TIMER0->TAILR= n;
	//Enable timer
	TIMER0->CTL |= (1<<0);
}




void UART_init(){
	
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
	
	
	//Interrupt FIFO Level
	UART0->IFLS|=(1 << 4); //RX FIFO >=1/2 Full
	
	//INTERRUPT Mask
	UART0->IM|=0x10; //Interrupt on receive 	
	
	//BAUDRATE BRD = 256000 
	//Baud BRD = BRDI + BRDF = UARTSysClk / (ClkDiv * Baud Rate)
	//BAUDRATED 65.1041666667
	//IBRD 65
	//FBRD 0.1041666667 -> integer(0.59375 * 64 + 0.5) = integer(7.1666666688)= 7
	UART0->CTL &=~ ((1 << 0)|(1 << 5));
	UART0->IBRD = 8;//58;
	UART0->FBRD = 54;//38;
	
	//BITS(7-SPS, 6:5 - WLEN, 4 - FEN, 3 - STP2, 2 - EPS,1 - PEN, 0 - BRK)
	//SETTING 0x0110000 for data length of 8 bits (WLEN-11), One stop bit (STP2-0), no parity(EPS-0), no fifo (FEN-0)
	// no interrupts (?)
	UART0->LCRH = 0x60|(1 << 4);
	
	//0x00 for system clock.
	UART0->CC  = 0x00;
	
	//Enable UART, setting UARTEN bit
	UART0->CTL  |= (1 << 0);
	
	NVIC->ISER[0]|=0x00000010;
	
}




void set_frequency(uint16_t freq_hertz){
	if(freq_hertz<1 || freq_hertz>200)
		return;
	timer_change(1200000/freq_hertz);
}