#include "PWM.h"
//#include "TM4C1294NCPDT.h"


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
//	PWM0->_0_GENB = 0x8C;
//	PWM0->_1_GENA = 0x8C;
	
	//pwm frequency
	//0x0000.018F for 60KHz. Load is a 16 bit value
	//Clock source is 120MHz/2=60MHz.
	//For 200Hz of PWM cycle, 300.000 clock ticks
	//For 1Hz cycle, 60.000.000 clock ticks
	//Start with 200Hz
	PWM0->_0_LOAD = 30000;
//	PWM0->_1_LOAD = 30000;	
//	PWM0->_2_LOAD = 3000000;
	
	//A and B values. This sets the duty cicle
	//A=150000 for 50% duty cicle.
	PWM0->_0_CMPA=15000;
//	PWM0->_1_CMPA=25000;
//	PWM0->_0_CMPB=000;
//	PWM0->_0_CMPB=0x0;

	//Enable timers
	PWM0->_0_CTL |= (1<<0);
//	SYSCTL->SRPWM &=~ (1<<0);
	//Enable outputs
	PWM0->ENABLE|= (1<<0)|(1<<1)|(1<<2);
}

void PWM_set_duty(uint16_t n){
	uint16_t load;
	uint16_t compA= load*0.01*n;
	if(n<0 || n>100){
		printString("Valor Invalido\n");
		return;
	}
	
	PWM0->_0_LOAD;
	PWM0->_0_CMPA;
	PWM0->_0_CMPB;
}