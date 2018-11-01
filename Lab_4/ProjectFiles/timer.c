#include "timer.h"





void timer_init(){
	int i=0;
//	IRQn_Type->TIMER0A_IRQn;// |=(1<<19);
	
	SYSCTL_Timer->RCGCTIMER|=(1<<0);
	for(i=0;i<1000;i++);
	//Disable timer
	TIMER0->CTL &=~(1<<0);
	TIMER0->CFG 	= 0x0;
	TIMER0->TAMR	= 0x2;	//LOAD value
	TIMER0->TAILR	= 3000;//6000;
	//bit 4 interrput match. bit 0 timeout interrupt mask
	TIMER0->IMR 	= 0x1;
	//Enable timer
	TIMER0->CTL 	|= (1<<0);
	//Clear when interrupt
//	TIMER0->ICR 	&= 0x0;

	TIMER0->CC &=~(1<<0);
	NVIC_Timer->ISER[0]|=0x00080000;
//	NVIC->IP[0] = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // 8) priority 2
// NVIC_EN0_R |= NVIC_EN0_INT19;    // 9) enable interrupt 19 in NVIC
	return;
}

void timer_change(uint32_t n){
	//Disable timer
	TIMER0->CTL &=~(1<<0);
	//LOAD value
	TIMER0->TAILR= n;
	//Enable timer
	TIMER0->CTL |= (1<<0);
	return;
}

void set_frequency(uint16_t freq_hertz){
	if(freq_hertz<1 || freq_hertz>200)
		return;
	timer_change(1200000/freq_hertz);
	return;
}