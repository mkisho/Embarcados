#include "cmsis_os.h"
//#include "TM4C1294NCPDT.h"             
#include <stdbool.h>
#include "grlib/grlib.h"
#include <math.h>
#include "PWM.h"
#include "UART.h"
#include "timer.h"

//System clock - 120MHz
//PWM clock - 60Mhz
//PWM cycle - 3000 PWM ticks ~= 20KHz = 50us
//Wave cyle - 1~200Hz = 1s ~ 5ms 
//Wave ticks = 6000 -> 20KHz = 50us (For 200Hz) 
//Wave ticks = 1200000 -> 100Hz = 10ms (For 1Hz)





void PWM_Update (void const *argument);               // thread function
osThreadId tid_PWM_Update;                            // thread id
osThreadDef (PWM_Update, osPriorityHigh, 1, 0);     // thread object

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











void init_all(){
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
				uint16_t frequency=200; //Valor da frequencia
//				uint16_t cont_freq=0;
				uint16_t amplitude=33;
				osEvent evt;
				char pBuf[20];
				set_frequency(frequency);
				while(1){
					evt = osSignalWait (0x01, 10000);
					if (evt.status == osEventSignal)  {
						x=gerarOnda(2,step)*30;
						x=x*amplitude/33;
						PWM_set_duty(x);
						if(step<=total_steps){
										step++;
					}
					else
						step=0;
					}
				}
}


void UART_Publish (void const *argument) {

				osEvent evt;
				char pBuf[20];
	
	
				while(1){	

							intToString(UART0->RIS,pBuf,15,10,5);
//							printString("PWM1= ");
//							printString(pBuf);
//							printString("\n");
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
		UART0->ICR |= (1<<5);	
		printChar(readChar());
		printString("What do i do\n");	
//	osSignalSet(tid_PWM_Update,0x1);
}

