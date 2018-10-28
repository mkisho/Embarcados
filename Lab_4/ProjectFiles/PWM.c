//#include "PWM.h"
#include "TM4C1294NCPDT.h"


void PWM_init(){
	//Enable and provide a clock to PWM module 0 in Run mode
	SYSCTL->RCGCPWM |= (1 << 0);
	
	//CHECAR
	SYSCTL->RCGCGPIO |= (1 << 0);
	
	
	//Setting PF0 for using alternate functions (AFSEL) and enabling digital logic (DEN).
	GPIOF_AHB->AFSEL|= (1 << 0);
	GPIOF_AHB->DEN	|= (1 << 0);
	//The M0PWM0 is connected to the PF0. 
	//It corresponds to the value on his respective nibble on GPIOF_AHB.
	GPIOF_AHB->PCTL|= (1 << 5)|(1 << 6);
	
	//bit 8 (USEPWM = 1, use pwm clock divisor), birt 2:0 (PWMDIV=0, for /2) 
	//MUST SET BITS 0,1,2 TO 0
	PWM0->CC |= (1 << 8);
	
	//PWM0->CTL =0x0;
	
	//Setting actions taken by pwm for each event.
	//11:10  ACTCMPBD
	//9:8 ACTCMPBU
	//7:6 ACTCMPAD
	//5:4 ACTCMPAU 
	//3:2 ACTLOAD
	//1:0 ACTZERO
	//0-nothing; 1-invert; 2-drive low; 3-drive high
	//0x0000.008C-> 1000 1100 -> Drive high on load, drive low on comp A Down. For everything else, do nothing
	PWM0->_0_GENA= 0x8C;
	//PWM0->_0_GENB= 0x00;
	
	//pwm frequency
	//0x0000.018F for 60KHz. Load is a 16 bit value
	//Clock source is 120MHz/2=60MHz.
	//For 200Hz of PWM cycle, 300.000 clock ticks
	//For 1Hz cycle, 60.000.000 clock ticks
	//Start with 200Hz
	PWM0->_0_LOAD= 300000;
	
	//A and B values. This sets the duty cicle
	//A=150000 for 50% duty cicle.
	PWM0->_0_CMPA=150000;
	PWM0->_0_CMPB=0x0;
	
	//Enable timers
	PWM0->CTL |= 1;
	
	//Enable outputs
	PWM0->ENABLE=0x03;
}

void PWM_set_duty(uint16_t n){
	uint16_t load;
	uint16_t compA= load*0.01*n;
	
	PWM0->_0_LOAD;
	PWM0->_0_CMPA;
	PWM0->_0_CMPB;
	
}
