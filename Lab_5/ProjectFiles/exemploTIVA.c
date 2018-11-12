#include "cmsis_os.h"
//#include "TM4C1294NCPDT.h"             
#include <stdbool.h>
#include "grlib/grlib.h"
#include <math.h>
#include "PWM.h"
#include "UART.h"
#include "timer.h"




void Controle (void const *argument);               // thread function
osThreadId tid_Controle;                            // thread id
osThreadDef (Controle, osPriorityNormal, 1, 0);     // thread object

void UART_Publish (void const *argument);               // thread function
osThreadId tid_UART_Publish;                            // thread id
osThreadDef (UART_Publish, osPriorityHigh, 1, 0);     // thread object

void UART_Subscriber (void const *argument);               // thread function
osThreadId tid_UART_Subscriber;                            // thread id
osThreadDef (UART_Subscriber, osPriorityHigh, 1, 0);     // thread object


void Gerador_pontos(void const *argument);               // thread function
osThreadId tid_Gerador_pontos;                            // thread id
osThreadDef (Gerador_pontos, osPriorityNormal, 1, 0);     // thread object

void Fibonacci (void const *argument);               // thread function
osThreadId tid_Fibonacci;                            // thread id
osThreadDef (Fibonacci, osPriorityHigh, 1, 0);     // thread object

void Primo (void const *argument);               // thread function
osThreadId tid_Primo;                            // thread id
osThreadDef (Primo, osPriorityHigh, 1, 0);     // thread object







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









void init_all(){
	//Init each driver
	UART_init();
	PWM_init();
	timer_init();
}


//São 100 passos dentro de um ciclo. 
//Retorna um valor apropriado também entre 0 e 100 para o calculo do PWM no passo.
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

int calcula_PWM(int angulo){
	return 1;

}

void Controle (void const *argument) {
	int PWM;
	int angulo;
	int servo;
	while (1){
		PWM = calcula_PWM(angulo);
		PWM_set_duty(servo, PWM);
		PWM = calcula_PWM(angulo);
		PWM_set_duty(servo, PWM);
		PWM = calcula_PWM(angulo);
		PWM_set_duty(servo, PWM);
		PWM = calcula_PWM(angulo);
		PWM_set_duty(servo, PWM);	
	}
}


void Fibonacci (void const *argument) {
	int a;
	int b;
	int aux;
	while(1){
		aux = a + b;
		a = b;
		b = aux;
	}
}
void Gerador_pontos (void const *argument) {
	
	
}


void Primo (void const *argument) {
		int n=1;
		int i =2;
		int max;
		while(1){
				max = sqrt(n);
				for (i =2; i<=max;i++){
						if (!(n%i)){
					
						}
				}
		}
}


void PWM_Update (void const *argument) {
				uint16_t x=0;
				uint16_t total_steps=100;
				uint16_t step=0;
				uint16_t frequency=200; //Valor da frequencia
				uint16_t amplitude=33; //Valor da amplitude *10 (para trabalhar com inteiros)
				//0-quadrado; 1-triangular;2-dente;3-senoide
				uint16_t type=1;
				uint16_t cmd=0;//Receive Uart Valuer
				osEvent evtSignal;
				osEvent evtMessage;
				char pBuf[20];
				while(1){
					
					evtMessage = osMessageGet(received_char_id,0);
					//Se recebe msg, trata apropriadamente, e envia msg do resultado para UARTPublish
					if (evtMessage.status == osEventMessage){
						cmd=((uint16_t) evtMessage.value.p); 
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
					
					//Ativado pela interrupção do timer. Avança um passo no ciclo, atualizando PWM
					evtSignal = osSignalWait (0x01, 10000);
					if (evtSignal.status == osEventSignal)  {
						x=gerarOnda(type,step)*3;
						x=x*amplitude/33;
//						PWM_set_duty(x);
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
				}
}


//Não foi usada no final
void UART_Subscriber (void const *argument) {
	osEvent evt;
	uint16_t n=1;
	char pBuf[20];
	char recChar;
	while(1){
		if(n<0 || n>100){
			return;
		}
		osThreadYield (); 
	}
}





int Init_Thread (void) {
  tid_Controle = osThreadCreate (osThread(Controle), NULL);
  if (!tid_Controle) return(-1);
	
	tid_UART_Subscriber = osThreadCreate (osThread(UART_Subscriber), NULL);
  if (!tid_UART_Subscriber) return(-1);
	
	tid_UART_Publish = osThreadCreate (osThread(UART_Publish), NULL);
  if (!tid_UART_Publish) return(-1);

	tid_Gerador_pontos = osThreadCreate (osThread(Gerador_pontos ), NULL);
  if (!tid_Gerador_pontos ) return(-1);
	
	tid_Fibonacci= osThreadCreate (osThread(Fibonacci), NULL);
  if (!tid_Fibonacci) return(-1);
	
	tid_Primo= osThreadCreate (osThread(Primo), NULL);
  if (!tid_Primo) return(-1);

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
	//clear na interrupção do timer
	TIMER0->ICR |= (1<<0);	
	//Sinaliza para a thread atualizar PWM
//	osSignalSet(tid_PWM_Update,0x1);
}



void UART0_Handler(void){
	//A LEITURA É RESPONSÁVEL PELO CLEAR DA INTERRUPÇÃO
	char c = readChar();
	osMessagePut(received_char_id,	c, 0);//readChar(), osWaitForever);
//	osSignalSet(tid_UART_Subscriber,0x1);
}

