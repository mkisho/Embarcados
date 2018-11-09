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


//osTimerId timer_Id;

//osSemaphoreId sid_Thread_Semaphore;                    // semaphore id
//osSemaphoreDef (Semaforo);                      // semaphore 

//osMutexDef (data_mutex);    // Declare mutex
//osMutexId  (data_mutex_id); // Mutex ID


osMessageQDef(received_char, 5, uint32_t); // Declare a message queue
osMessageQId (received_char_id);           // Declare an ID for the message queue

osMessageQDef(update_int, 5, uint32_t); // Declare a message queue
osMessageQId (update_int_id);           // Declare an ID for the message queue


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


int charToInt(char c){
		int n=0;
		n = c-48;
		return n;
}



//int stringToInt(char* string){
//		int n=0;
//		while(*string)
//		{

//		}
		
//		return n;
//}









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
					return 99;
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
				uint16_t type=1;
				uint16_t cmd=0;
				osEvent evt;
				osEvent evtMessage;

				char pBuf[20];
				
				while(1){
					
					evtMessage = osMessageGet(received_char_id,0);
					if (evtMessage.status == osEventMessage){
						cmd=((uint16_t) evtMessage.value.p); 
//						printChar(cmd);
						switch(cmd){
							case 'z':
								if(frequency>1){
									frequency--;
									set_frequency(frequency);
									osMessagePut(update_int_id,	'f', osWaitForever);
									osMessagePut(update_int_id,	frequency, osWaitForever);
								}
							break;
							case 'x':
								if(frequency<200){
									frequency++;
									set_frequency(frequency);
									osMessagePut(update_int_id,	'f', osWaitForever);
									osMessagePut(update_int_id,	frequency, osWaitForever);
								}
							break;
							case 'v':
								if(amplitude>1){
									amplitude--;
									osMessagePut(update_int_id,	'a', osWaitForever);
									osMessagePut(update_int_id,	amplitude, osWaitForever);
								}
							break;
							case 'c':
								if(amplitude<33){
									amplitude++;
									osMessagePut(update_int_id,	'a', osWaitForever);
									osMessagePut(update_int_id,	amplitude, osWaitForever);
								}
							break;
							case 'q':
								type=0;
								osMessagePut(update_int_id,	't', osWaitForever);
								osMessagePut(update_int_id,	type, osWaitForever);
							break;
							case 't':
								type=1;
								osMessagePut(update_int_id,	't', osWaitForever);
								osMessagePut(update_int_id,	type, osWaitForever);
							break;
							case 'd':
								type=2;
								osMessagePut(update_int_id,	't', osWaitForever);
								osMessagePut(update_int_id,	type, osWaitForever);
							break;
							case 's':
								type=3;
								osMessagePut(update_int_id,	't', osWaitForever);
								osMessagePut(update_int_id,	type, osWaitForever);
							break;
							default:
							break;
						}
						
					}

					evt = osSignalWait (0x01, 10000);
					if (evt.status == osEventSignal)  {
						x=gerarOnda(type,step)*30;
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
				char cmdType;
				int  value;
	
				while(1){	
						printString("--Digite q para onda quadrada, t para onda triangular, d para onda dente-de-serra, s para onda senoidal\n\r");
						printString("--Digite z/x para aumentar/diminuir frequencia; c/v para aumentar/diminuir amplitude\n\r");
						evt = osMessageGet(update_int_id,osWaitForever);
						cmdType=(char) evt.value.p;
						evt = osMessageGet(update_int_id,osWaitForever);
						value=(int) evt.value.p;
						printString("\n\r\n\r\n\r\n\r\n\r\n\r");
						switch(cmdType){
							case 'f':
								printString("Frequencia alterada para: ");
								intToString(value, pBuf, 3, 10, 1);
								printString(pBuf);
								printString(" Hz\n\r");
							break;
							case 'a':
								printString("Amplitude alterada para: ");
								
								intToString(value/10, pBuf, 3, 10, 1);	
								printString(pBuf);
								printString(".");
								intToString((value%10), pBuf, 3, 10, 1);
								printString(pBuf);
								printString(" V\n\r");
							break;
							case 't':
								printString("Tipo alterado para: ");
								switch(value){
									case 0:
										printString("Onda Quadrada\r\n");
									break;
									case 1:
										printString("Onda Triangular\r\n");
										break;
									case 2:
										printString("Onda Dente de serra\r\n");
										break;
									case 3:
										printString("Onda Senoidal\r\n");
										break;
									default:
										break;
									
								}
							
							break;
							default:
							break;
						}
						
						
		//					osDelay(1000);	
//					osMessagePut(received_char_id,	'g', osWaitForever);//readChar(), osWaitForever);					
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
	uint16_t n=1;
	char pBuf[20];
	char recChar;
	while(1){
		if(n<0 || n>100){
			//		printString("Valor Invalido\n");
			return;
		}
//		evt = osMessageGet(received_char_id, osWaitForever);
//		recChar = (char) evt.value.p;

//		n=recChar;//charToInt(recChar);
		
//		osMessagePut(update_int_id,	n, 0);
		
//		evt = osSignalWait (0x02, 10);
//		if (evt.status == osEventSignal)  {
//		}
//		c=readChar();
//		printChar(c);
		osThreadYield (); 
	}
}





int Init_Thread (void) {

//	sid_Thread_Semaphore = osSemaphoreCreate(osSemaphore(Semaforo), 10);
//  if (!sid_Thread_Semaphore) return(-1);
	
//	data_mutex_id = osMutexCreate(osMutex(data_mutex));
	
  tid_PWM_Update = osThreadCreate (osThread(PWM_Update), NULL);
  if (!tid_PWM_Update) return(-1);
	tid_UART_Subscriber = osThreadCreate (osThread(UART_Subscriber), NULL);
  if (!tid_UART_Subscriber) return(-1);
	tid_UART_Publish = osThreadCreate (osThread(UART_Publish), NULL);
  if (!tid_UART_Publish) return(-1);
	
	received_char_id = osMessageCreate(osMessageQ(received_char), NULL);
	update_int_id = osMessageCreate(osMessageQ(update_int), NULL);
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
	//A LEITURA É RESPONSÁVEL PELO CLEAR DA INTERRUPÇÃO
	char c = readChar();
	osMessagePut(received_char_id,	c, 0);//readChar(), osWaitForever);

	
//		UART0->ICR |= (1<<5);	
		osSignalSet(tid_UART_Subscriber,0x1);
		
//		
//	osSignalSet(tid_PWM_Update,0x1);
}

