//#include "PWM.h"
#include "TM4C1294NCPDT.h"


void PWM_init(){
	SYSCTL->RCGCPWM
	PWM0->CTL |= (1 << 0);
	
	
}