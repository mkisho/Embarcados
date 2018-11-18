#include "PWM.h"


//Using PF2 pin, M0PWM2
void PWM_init(){
	int i ;
	//Enable and provide a clock to PWM module 0 in Run mode
	SYSCTL_PWM->RCGCPWM |= (1 << 0);
	
	//Habilitando Port F
	SYSCTL_PWM->RCGCGPIO |= (1 << 5);
	//delay for avoiding problems
	for(i=0;i<1000;i++);
	//Setting PF2 for using alternate functions (AFSEL) and enabling digital logic (DEN).
	GPIOF_AHB_PWM->AFSEL|= (1 << 0);
	GPIOF_AHB_PWM->DEN	|= (1 << 0);							
	GPIOF_AHB_PWM->AFSEL|= (1 << 1);
	GPIOF_AHB_PWM->DEN	|= (1 << 1);
	GPIOF_AHB_PWM->AFSEL|= (1 << 2);
	GPIOF_AHB_PWM->DEN	|= (1 << 2);
	//The M0PWM2 is connected to the PF2. 
	//It corresponds to the value on his respective nibble on GPIOF_AHB.
	GPIOF_AHB_PWM->PCTL	|= (1 << 2)|(1 << 1);
	GPIOF_AHB_PWM->PCTL	|= (1 << 5)|(1 << 6);
	GPIOF_AHB_PWM->PCTL	|= (1 << 10)|(1 << 9);	
	//bit 8 (USEPWM = 1, use pwm clock divisor), birt 2:0 (PWMDIV=0, for /2) 
	PWM0->CC &=~ ((1 << 0)|(1 << 1)|(1 << 2));	
	PWM0->CC |= (1 << 8);

//disabling PWM block 1			
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
	PWM0->_1_GENA = 0xC8;
	PWM0->_1_GENB = 0xC8;
	//pwm frequency
	//Clock source is 120MHz/2=60MHz. 6000 ticks -> 10KHz
	PWM0->_0_LOAD = 6001;
	PWM0->_1_LOAD = 6001;	
	
	//A and B values. This sets the duty cicle
	//A=150 for 50% duty cicle.
	PWM0->_0_CMPA=150;
	PWM0->_0_CMPB=150;
	PWM0->_1_CMPA=150;
	PWM0->_1_CMPB=150;
	//Enable timers
	PWM0->_0_CTL |= (1<<0);
	PWM0->_1_CTL |= (1<<0);
	//Enable outputs
	PWM0->ENABLE|= (1<<0)|(1<<1)|(1<<2);
	return;
}

void PWM_set_duty(uint16_t servo, uint16_t PWM){
	//Change compA for duty cicle of M0PWM2
	switch (servo){
		case 0:
			PWM0->_0_CMPA=(PWM);
			break;
		case 1:
			PWM0->_0_CMPB=(PWM);
			break;
		case 2:
			PWM0->_1_CMPA=(PWM);
			break;
		case 3:
			PWM0->_1_CMPB=(PWM);
			break;
		default:
			break;
	}

return;
}