#include "cmsis_os.h"
#include "TM4C1294NCPDT.h"             
#include <stdbool.h>
#include "grlib/grlib.h"
#include <math.h>
//#include "buttons.h"
//#include "buzzer.h"
//#include "PWM.h"
//#include "UART.h"


//System clock - 120MHz
//PWM clock - 60Mhz
//PWM cycle - 3000 PWM ticks ~= 20KHz = 50us
//Wave cyle - 1~200Hz = 1s ~ 5ms 
//Wave ticks = 12000 -> 20KHz = 50us (For 200Hz) 
//Wave ticks = 1200000 -> 100Hz = 10ms (For 1Hz)





void PWM_Update (void const *argument);               // thread function
osThreadId tid_PWM_Update;                            // thread id
osThreadDef (PWM_Update, osPriorityNormal, 1, 0);     // thread object

void UART_Publish (void const *argument);               // thread function
osThreadId tid_UART_Publish;                            // thread id
osThreadDef (UART_Publish, osPriorityNormal, 1, 0);     // thread object

void UART_Subscriber (void const *argument);               // thread function
osThreadId tid_UART_Subscriber;                            // thread id
osThreadDef (UART_Subscriber, osPriorityNormal, 1, 0);     // thread object


osTimerId timer_Id;

//osSemaphoreId sid_Thread_Semaphore;                    // semaphore id
//osSemaphoreDef (Semaforo);                      // semaphore 

osMutexDef (data_mutex);    // Declare mutex
osMutexId  (data_mutex_id); // Mutex ID


const uint16_t quadrada[] = {0};
const uint16_t senoidal[] = {0};
const uint16_t triangular[] = {0};
const uint16_t dentedeserra[] = {0};







static void intToString(int64_t value, char * pBuf, uint32_t len, uint32_t base, uint8_t zeros){
	static const char* pAscii = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	bool n = false;
	int pos = 0, d = 0;
	int64_t tmpValue = value;

	// the buffer must not be null and at least have a length of 2 to handle one
	// digit and null-terminator
	if (pBuf == NULL || len < 2)
			return;

	// a valid base cannot be less than 2 or larger than 36
	// a base value of 2 means binary representation. A value of 1 would mean only zeros
	// a base larger than 36 can only be used if a larger alphabet were used.
	if (base < 2 || base > 36)
			return;

	if (zeros > len)
		return;
	
	// negative value
	if (value < 0)
	{
			tmpValue = -tmpValue;
			value    = -value;
			pBuf[pos++] = '-';
			n = true;
	}

	// calculate the required length of the buffer
	do {
			pos++;
			tmpValue /= base;
	} while(tmpValue > 0);


	if (pos > len)
			// the len parameter is invalid.
			return;

	if(zeros > pos){
		pBuf[zeros] = '\0';
		do{
			pBuf[d++ + (n ? 1 : 0)] = pAscii[0]; 
		}
		while(zeros > d + pos);
	}
	else
		pBuf[pos] = '\0';

	pos += d;
	do {
			pBuf[--pos] = pAscii[value % base];
			value /= base;
	} while(value > 0);
}












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
	PWM0->_1_GENA = 0x8C;
	
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
	n=n*30;
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
	TIMER0->TAILR	= 6000;
	//bit 4 interrput match. bit 0 timeout interrupt mask
	TIMER0->IMR 	= 0x1;
	//Enable timer
	TIMER0->CTL 	|= (1<<0);
	printString("OMAE WA MOU SHINDEIRU6\n");	
	//Clear when interrupt
//	TIMER0->ICR 	&= 0x0;


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
	UART0->IFLS|=0x10; //RX FIFO >=1/2 Full
	
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
	UART0->LCRH = 0x60;
	
	//0x00 for system clock.
	UART0->CC  = 0x00;
	
	//Enable UART, setting UARTEN bit
	UART0->CTL  |= (1 << 0);
	
	NVIC->ISER[0]|=0x00000010;
	
}












void init_all(){
//button_init();
	UART_init();
	PWM_init();
	timer_init();
}



uint16_t gerarOnda(uint16_t type, uint16_t x){
		float senoide;
				char pBuf[20];
		switch (type){
			case 0: //quadrada
				if(x>50)
					return 100;
				else
					return 0;
			break;
			case 1: //triangular
				if(x<50)
					return x*2;
				else
					return 100-((x-50)*2);
			break;
			case 2: //dente de serra
				return x;
			break;
			case 3: //senoidal
				senoide=(sin(x*6.2831/100)+1)*50;

				return (int) senoide;
			break;
		}
	
}





void PWM_Update (void const *argument) {
				uint16_t x=0;
				uint16_t total_steps=100;
				uint16_t step=0;
				uint16_t frequency=100; //Valor da frequencia
//				uint16_t cont_freq=0;
				uint16_t amplitude=33;
				osEvent evt;
				char pBuf[20];
				while(1){
					evt = osSignalWait (0x01, 10000);
					if (evt.status == osEventSignal)  {
//						if ((TIMER0->RIS & (1<<0)) || (TIMER0->MIS&(1<<0)))  {		
//								TIMER0->ICR |= (1<<0);				
//							if(cont_freq>=(200-frequency)){
								if(step<=total_steps){
										step++;
										x++;
										if(x>=100){
											x=0;
										}
										intToString(x,pBuf,15,10,5);
										printString(pBuf);
										printString("\n");
										PWM_set_duty(gerarOnda(3,x));
										
								}
								else
										step=0;
//								cont_freq=0;
						
					}
//							else
//								cont_freq++;
//					}
						
						
//					if(true){
//						osSignalSet(tid_UART_Publish,0x2);
//					}
					
					
				}
}


void UART_Publish (void const *argument) {

				osEvent evt;
				char pBuf[20];
	
	
				while(1){	

		//					printString("\n");
		//					intToString(PWM0->_1_COUNT,pBuf,15,10,5);
		//					printString("PWM1= ");
		//					printString(pBuf);
		//					printString("\n");
		//					osDelay(1000);
				}
	
				while(1){
					evt = osSignalWait (0x01, 10000);
						if (evt.status == osEventSignal)  {
					}
					if(true){
						osSignalSet(tid_UART_Publish,0x2);
					}
					
					
				}
}

void UART_Subscriber (void const *argument) {
	osEvent evt;
	uint16_t val;
	uint16_t n;
	char c;
	while(1){
	if(n<0 || n>100){
//		printString("Valor Invalido\n");
		return;
	}
	//		evt = osSignalWait (0x01, 10000);
//		if (evt.status == osEventSignal)  {
//			val = osMutexWait (data_mutex_id, 1000);// wait 10 mSec
//			switch (val) {
//				case osOK:
//					osMutexRelease (data_mutex_id);
//					break;
//				case osErrorResource:
//					break;
//				case osErrorParameter:
//					break;
//				default:
//					break;
//			}
//		evt = osSignalWait (0x02, 10);
//		if (evt.status == osEventSignal)  {
//		}
//		c=readChar();
//		printChar(c);
		osThreadYield (); 
//		}
	}
}



void refresh (void const *n) {

	osSignalSet(tid_UART_Subscriber,0x1);
	osSignalSet(tid_UART_Publish,0x1);
}

osTimerDef(timer_0, refresh);


int Init_Thread (void) {
	timer_Id= osTimerCreate(osTimer(timer_0), osTimerPeriodic, (void*)0);
	osTimerStart(timer_Id,1);

//	sid_Thread_Semaphore = osSemaphoreCreate(osSemaphore(Semaforo), 10);
//  if (!sid_Thread_Semaphore) return(-1);
	
	data_mutex_id = osMutexCreate(osMutex(data_mutex));
	
  tid_PWM_Update = osThreadCreate (osThread(PWM_Update), NULL);
  if (!tid_PWM_Update) return(-1);
	tid_UART_Subscriber = osThreadCreate (osThread(UART_Subscriber), NULL);
  if (!tid_UART_Subscriber) return(-1);
	tid_UART_Publish = osThreadCreate (osThread(UART_Publish), NULL);
  if (!tid_UART_Publish) return(-1);
  return(0);
}




int main (void) {
	osKernelInitialize();
	init_all();		
		
	Init_Thread();
	if(osKernelStart()!=osOK){
		
	}
	osDelay (osWaitForever);

}










void TIMER0A_Handler(void){
	TIMER0->ICR |= (1<<0);	
	osSignalSet(tid_PWM_Update,0x1);
}



void UART0_Handler(void){
		UART0->ICR |= (1<<0);	
		printString("OMAE WA MOU SHINDEIRU6\n");	
//	osSignalSet(tid_PWM_Update,0x1);
}

