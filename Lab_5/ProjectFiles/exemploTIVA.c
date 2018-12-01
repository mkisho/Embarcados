#include "cmsis_os.h"
#include "TM4C1294NCPDT.h"             
#include <stdbool.h>
#include "grlib/grlib.h"
#include <math.h>
#include "PWM.h"
#include "UART.h"
#include "timer.h"
#include "cfaf128x128x16.h"
#include "time.h"


//=======Constantes====
#define ticks_factor  10000
#define RETANGULO 1
#define CIRCULO   2 
#define LOSANGO   3
#define FAIXA     4

#define REALTIME  -100

#define PRI 0
#define UAR 1
#define GER 2
#define CON 3
#define FIB 4

//=====



//TODO Gerar pontos Bandeira
//TODO Mandar pontos para o controle
//TODO Calcular angulos
//TODO Arrumar signals
//TODO Escalonar
//TODO Imprimir informações
//TODO Ler comandos
//TODO Andar com comandos
//TODO Gerar Gantt
//TODO Comentar


typedef struct {
	osThreadId tid;
//	char name;
	uint32_t estimated_ticks;
	uint32_t max_ticks;
	uint32_t execution_ticks;
	uint32_t relax_ticks;
//	uint16_t aux_ticks;
	uint32_t start_tick;
	int16_t static_priority;
	int16_t dynamic_priority;
	uint16_t periodicity;
//	uint16_t await_time;
//	uint16_t delay;
//	float progress;
	bool master_fault;
	bool secundary_fault;
	bool running;
	bool awaiting;
} ThStatus;

ThStatus Threads[6];

typedef struct pwm{
	int pwm_x;
	int pwm_y;
	int pwm_z;
}PWM;

typedef struct point{
	float x;
	float y;
}Point;

tContext sContext;

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

void Escalonador (void const *argument); 
osThreadId tid_Escalonador;   
osThreadDef (Escalonador, osPriorityRealtime, 1, 0);

osMessageQDef(received_char, 5, uint32_t); // Declare a message queue
osMessageQId (received_char_id);           // Declare an ID for the message queue

osMessageQDef(update_int, 5, uint32_t); // Declare a message queue
osMessageQId (update_int_id);           // Declare an ID for the message queue

osMessageQDef(newPoint, 5, uint32_t); // Declare a message queue
osMessageQId (newPoint_id);           // Declare an ID for the message queue

osMessageQDef(exit_status, 5, uint32_t); // Declare a message queue
osMessageQId (tid_exit_status);           // Declare an ID for the message queue

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
	cfaf128x128x16Init();
}



//São 100 passos dentro de um ciclo. 
//Retorna um valor apropriado também entre 0 e 100 para o calculo do PWM no passo.
Point gerarOnda(uint16_t type, uint16_t cont){
//	float senoide;
	Point point;
//	char pBuf[20];
	float count=cont;
	switch (type){
		case 0: //retangulo
			if(count<50){
				point.x=-3;
				point.y=count*3/50;
				count++;
			}	
			else if(count<150){
				point.y=3;
				point.x=-3+(count-50)*3/50;
				count++;
			}	
			else if(count<200){
				point.x=3;
				point.y=3-(count-150)*3/50;										
				count++;
			}	
			else if(count<300){
				point.y=0;
				point.x=3-(count-200)*3/50;
				count++;
			}
			else {
				count=0;
			}	
			point.y+=2;
		break;
																
		case 1: //losango
			if(count<50){
				point.x=-count*3/50;
				point.y=count*1.5/50;
				count++;
			}	
			else if(count<100){
				point.y=1.5+(count-50)*1.5/50;
				point.x=-3+(count-50)*3/50;
				count++;
			}	
			else if(count<150){
				point.x=(count-100)*3/50;
				point.y=3-(count-100)*1.5/50;										
				count++;
			}	
			else if(count<200){
				point.y=1.5-(count-150)*1.5/50;
				point.x=3-(count-150)*3/50;
				count++;
			}
			else {
				count=0;
			}	
			point.y+=2;
		break;
			
		case 2: //circulo
				point.x=1.25*sin(count*3.14/180);
				point.y=3.5-1.25*cos(count*3.14/180);
		break;
			
		case 3: //faixa
			
		break;
		}
	return point;
}


void calcula_PWM_angulos(uint16_t x, uint16_t  y){
	sin(x/sqrt(y*y+x*x));
	return;
}

//Position "0" (1.5 ms pulse) is middle, 
//"90" (~2ms pulse) is all the way to the right.
//"-90" (~1 ms pulse) is all the way to the left"
PWM calcula_PWM(float x, float y){
	float g;
	float d;
//	char pBuf[10];
	PWM pwm;	
	g=atan(x/(y+10));

	pwm.pwm_z=2500.0-13.33*g*3.0*(180.0/3.14);
	d=sqrt(x*x+(y+10)*(y+10))-10;
	pwm.pwm_y = 2800+d*300;
	pwm.pwm_x = 2500+d*330;
	return pwm;
}

/*
void Controle (void const *argument) {
	int time;
	int PWM;
	int angulo;
	int servo;
	int cmd;
	int x,y=5;
	bool inativo = false;
	osEvent evtMessage;
	osEvent evtSignal;
	while (1){
		evtSignal = osSignalWait (0x01, 0);
		if (evtSignal.status == osEventSignal){	

			time = osKernelSysTick()/ticks_factor;
			evtMessage = osMessageGet(newPoint_id,0);
			if (evtMessage.status == osEventMessage){
				while(inativo){
					
				}
				calcula_PWM_angulos(x,y);
				
				cmd=((uint16_t) evtMessage.value.p); 
				PWM = calcula_PWM(angulo);
				PWM_set_duty(0, PWM);
				
				PWM = calcula_PWM(angulo);
				PWM_set_duty(1, PWM);
				
				PWM = calcula_PWM(angulo);
				PWM_set_duty(2, PWM);
				
				PWM = calcula_PWM(angulo);
				PWM_set_duty(3, PWM);	
			}
		}
	}
	osThreadYield();
}
*/

void Fibonacci (void const *argument) {
//	osEvent evtMessage;
	osEvent evtSignal;
	int ticks;
	int a;
	int b;
	int aux;
	while(1){

		evtSignal = osSignalWait (0x01, osWaitForever);
		if (evtSignal.status == osEventSignal)  {	
				printString("TesteFibonacci1\n\r");
				ticks= osKernelSysTick()/ticks_factor;
				aux = a + b;
				a = b;
				b = aux;
				if(true){
			//				if(a%10==0){
						osMessagePut(tid_exit_status, 0, osWaitForever);
						osSignalWait(0x01, osWaitForever);
				}
						osMessagePut(tid_exit_status, 1, osWaitForever);
		}

	}
}

void Gerador_pontos (void const *argument) {


	while(1){
			osSignalWait(0x01, osWaitForever);
			osMessagePut(tid_exit_status, 0, osWaitForever);	
			printString("TesteGerador1\n\r");
		}
	
	/*
	int time;
	int type=0;
	bool manual = false;
	osEvent evtMessage;
	osEvent evtSignal;
	int rectangle_x = 3;
	int rectangle_y = 2;
	int circle_radius = 1;
	int losangle_x = 2;
	int losangle_y = 2 ;
	int x,y;
	int i=0;
	while(1){
		evtSignal = osSignalWait (0x01, 0);
		if (evtSignal.status == osEventSignal)  {	

			time = osKernelSysTick()/ticks_factor;
			if(!manual){
				if(type==RETANGULO){
					for(i=0;i<10;i++){
						x=rectangle_x;
						y=rectangle_y;
					}
					for(i=0;i<10;i++){
						//x=;
						y=rectangle_y;
					}
					for(i=0;i<10;i++){
						x=rectangle_x;
						//y=
					}
					for(i=0;i<10;i++){
						//x=
						y=rectangle_y;
					}
					
				}
				else if(type==CIRCULO){
					for(i=0;i<40;i++){
					
					}
				}
				else if(type==LOSANGO){
					for(i=0;i<10;i++){
					
					}
					for(i=0;i<10;i++){
					
					}
					for(i=0;i<10;i++){
					
					}
					for(i=0;i<10;i++){
					
					}
				}
				else if(type==FAIXA){
				
				}
			}
			
		//	osMessagePut();
		}
	}
	*/
}


void Primo (void const *argument) {
		int ticks;
		int n=1;
		int i =2;
		int max;
		osEvent evtSignal;
		while(1){

			evtSignal = osSignalWait (0x01, 0);
			if (evtSignal.status == osEventSignal)  {	
								printString("TestePrimo1\n\r");
					ticks = osKernelSysTick()/ticks_factor;
					max = sqrt(n);
					for (i =2; i<=max;i++){
							if (!(n%i)){
									break;
							}
							if(i%10==0){
								osMessagePut(tid_exit_status, 0, osWaitForever);
								osSignalWait(0x01,osWaitForever);
							}
					}
			osMessagePut(tid_exit_status, 1, osWaitForever);
			}

		}
}



//-90	3800
//-45	2700
//0		2100
//45	1500
//90	900	
//g=atg(x/(y+10));
//y=a+b*g;
//y=2100-13,33*g;

//2400  3400	
//2600  3600	
//2800  3700	
//3000  3700	
//3200  3900	
//3400	4100	
//3600  4400	
//PWM ombro  = 2400+d*200
//PWM servo  = 3500+d*100



//TODO Teste
void Controle (void const *argument) {
				int ticks;
				PWM pwm;
				Point point;
//				float x=0;
//				float y=0;
				
//				uint16_t total_steps=100;
				uint16_t step=0;
				uint16_t type=0;
				uint16_t cmd=0;//Receive Uart Valuer
				osEvent evtSignal;
				osEvent evtMessage;
				uint16_t count=0;
//				bool up = true;
				char pBuf[20];
				pwm.pwm_x = 3700;
				pwm.pwm_y = 3000;
				pwm.pwm_z = 2100;
	
	
				while(1){
						printString("TesteControle1\n\r");
					evtSignal = osSignalWait (0x01, osWaitForever);
					if (evtSignal.status == osEventSignal)  {	
							printString("TesteControle1\n\r");
							ticks = osKernelSysTick()/ticks_factor;
							if(step==8){
								step=0;
								evtMessage = osMessageGet(received_char_id,0);
								if (evtMessage.status == osEventMessage){
									cmd=((uint32_t) evtMessage.value.p); 
									switch(cmd){
										case '8':
											pwm.pwm_x+=200;
											break;
										case '2': 
											pwm.pwm_x-=200;
											break;
							  		case '6':
											pwm.pwm_y+=200;
											break;
										case '4':
											pwm.pwm_y-=200;
											break;
											case '7':
											pwm.pwm_z-=200;
											break;
											case '9':
											pwm.pwm_z+=200;
											break;
											case '5':
												switch(type){
													case 0:
													type++;
													break;
													case 1:
													type++;
													break;
													case 2:
													type++;
													break;
//													case 3:
//													type++;
//													break;
													default:
													type=0;
													break;
												}
											break;
										default:
											break;
										}
						}
										point=gerarOnda(type, count);
//									intToString(point.x*100, pBuf, 10, 10, 1);
//									printString(pBuf);
//									printString(" ");
//									intToString(point.y*100, pBuf, 10, 10, 1);
//									printString(pBuf);
//									printString(" ");
										count++;
										if(type==0){
											if(count>300){
												count=0;
												type++;
											}
										}
										else if(type==1){
											if(count>200){
												count=0;
												type++;
										}
										}
										else {
											if(count>400){
												count=0;
													type=0;
											
										}
										}
										pwm=calcula_PWM(point.x,point.y);										
										PWM_set_duty(0,pwm.pwm_z);
		//								PWM_set_duty(1,x);
									PWM_set_duty(2,pwm.pwm_x);
		//					PWM_set_duty(3,x);
									PWM_set_duty(4,pwm.pwm_y);
									
									
/*									
									intToString(count, pBuf, 10, 10, 1);
									printString(pBuf);
									printString(" ");
									intToString(pwm.pwm_x, pBuf, 10, 10, 1);
									printString(pBuf);
									printString(" ");
									intToString(pwm.pwm_y, pBuf, 10, 10, 1);
									printString(pBuf);
									printString(" ");
										intToString(pwm.pwm_z, pBuf, 10, 10, 1);
									printString(pBuf);
									printString("\n\r");
*/																	
								}
							step++;
							osMessagePut(tid_exit_status, 1, osWaitForever);
					}
			}
}


void UART_Publish (void const *argument) {
				int time;
				osEvent evt;
				osEvent evtSignal;
//				char pBuf[20];
				char cmdType;
				int  value;
	
				while(1){	
						evtSignal = osSignalWait (0x01, 0);
						if (evtSignal.status == osEventSignal)  {	

						time = osKernelSysTick()/ticks_factor;
						evt = osMessageGet(update_int_id,osWaitForever);
						cmdType=(uint32_t) evt.value.p;
						evt = osMessageGet(update_int_id,osWaitForever);
						value=(int32_t) evt.value.p;
						printString("\n\r\n\r\n\r\n\r\n\r\n\r");
						switch(cmdType){
							default:
							break;
						}
				}
			}
}


//Não foi usada no final
void UART_Subscriber (void const *argument) {
	int ticks;
	osEvent evtSignal;
	uint16_t n=1;
//	char pBuf[20];
//	char recChar;
	while(1){

			evtSignal = osSignalWait (0x01, 0);
			if (evtSignal.status == osEventSignal)  {	
					printString("Uart1\n\r");
					ticks = osKernelSysTick()/ticks_factor;
					if(n>100){
						return;
					}
					osMessagePut(tid_exit_status, 1, osWaitForever);
			}
		osThreadYield (); 
	}
}





int Init_Thread (void) {
  tid_Escalonador= osThreadCreate (osThread(Escalonador), NULL);
  if (!tid_Escalonador) return(-1);
	
  tid_Controle = osThreadCreate (osThread(Controle), NULL);
  if (!tid_Controle) return(-1);
	
	tid_UART_Subscriber = osThreadCreate (osThread(UART_Subscriber), NULL);
	if (!tid_UART_Subscriber) return(-1);
	
//	tid_UART_Publish = osThreadCreate (osThread(UART_Publish), NULL);
//  if (!tid_UART_Publish) return(-1);

	tid_Gerador_pontos = osThreadCreate (osThread(Gerador_pontos ), NULL);
  if (!tid_Gerador_pontos ) return(-1);
	
	tid_Fibonacci= osThreadCreate (osThread(Fibonacci), NULL);
  if (!tid_Fibonacci) return(-1);
	
	tid_Primo= osThreadCreate (osThread(Primo), NULL);
 if (!tid_Primo) return(-1);

	received_char_id = osMessageCreate(osMessageQ(received_char), NULL);
	update_int_id = osMessageCreate(osMessageQ(update_int), NULL);

	newPoint_id = osMessageCreate(osMessageQ(newPoint), NULL);
	tid_exit_status = osMessageCreate(osMessageQ(exit_status), NULL);

	return(0);
}






void init_sidelong_menu(){
//	uint8_t i;
	printString("TesteScreen1\n\r");
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
	
	//Escreve menu lateral:
	GrStringDraw(&sContext,"Pr ", -1, 0, (sContext.psFont->ui8Height+2)*1, true);
	GrStringDraw(&sContext,"TE ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"Es ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"PE ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"At ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"Fi ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"Fa ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	
}


void print_display (ThStatus thread, uint16_t n){
	bool s1_button;
	int display;
	char pBuf[10];
	intToString(thread.static_priority, pBuf, 10, 10, 1);
	GrStringDraw(&sContext,"Prioridade = ", -1, 0, (sContext.psFont->ui8Height+2)*1, true);
	intToString(thread.execution_ticks, pBuf, 10, 10, 1);	
	GrStringDraw(&sContext,"Tempo relax= ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	//	intToString(thread., pBuf, 10, 10, 1);
	GrStringDraw(&sContext,"Estado     = ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	//intToString(thread.estimated_ticks-thread., pBuf, 10, 10, 1);
	GrStringDraw(&sContext,"Per. exec  = ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
//	intToString(thread., pBuf, 10, 10, 1);
	GrStringDraw(&sContext,"Atraso     = ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
//		intToString(thread., pBuf, 10, 10, 1);
	GrStringDraw(&sContext,"Fila       = ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
//		intToString(thread., pBuf, 10, 10, 1);
	GrStringDraw(&sContext,"Faltas     = ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
}


void update_info(uint32_t current_ticks, ThStatus *thread){
	thread->execution_ticks=current_ticks-thread->start_tick;
	thread->relax_ticks=thread->estimated_ticks-(current_ticks-thread->start_tick);
//	thread->dynamic_priority=
	if(thread->execution_ticks>thread->max_ticks){
		if(thread->static_priority==REALTIME){
			thread->master_fault=true;
		}
		else{
			thread->secundary_fault=true;
			thread->static_priority--;
		}
	}
	
//	thread->dynamic_priority=-100+(100-thread->dynamic_priority)*progress
	
}

void stop_thread(uint32_t current_ticks, ThStatus *thread){
			thread->awaiting=false;
			if(thread->execution_ticks<(thread->max_ticks)/2){
					thread->secundary_fault=true;
					thread->static_priority++;
			}
}
void periodic_start(uint32_t current_ticks, ThStatus *thread){
		thread->awaiting=true;
		thread->dynamic_priority=thread->static_priority;
		thread->start_tick=current_ticks;
		thread->relax_ticks=thread->max_ticks;
}

void Escalonador(void const *argument) {
	bool s1_button=false;
	bool execute=true;
	osEvent evtSignal;
	osEvent evtMessage;
	int i=0;
	int cont=0;
	int exit_status=0;
	uint16_t secondary_faults=0;
	bool master_fault=false;
	uint32_t ticks=0;	
	char pBuf[15];
//	init_threads_info();
	/*	osThreadId tid;
	uint32_t estimated_ticks;
	uint32_t max_ticks;
	uint32_t execution_ticks;
	uint32_t relax_ticks;
	uint32_t start_tick;
	int32_t static_priority;
	int32_t dynamic_priority;
	uint32_t periodicity;
	bool master_fault;
	bool secundary_fault;
	bool running;
	bool awaiting;
*/
	ThStatus aux= {0, 0,0,0,0,0,1000,1000,0,false,false,false,false};
	ThStatus Gerador_Pontos = {0, 50000,80000,0,50000,0,10,10,1,false,false,false,false};
	ThStatus Uart_Subscriber = {0, 50000,80000,0,50000,0,-30,-30,0,false,false,false,false};
	ThStatus Controle = {0, 50000,80000,0,50000,0,0,0,1,false,false,false,false};
	ThStatus Primos = {0, 50000,80000,0,50000,0,-100,-100,2,false,false,false,false};
	ThStatus Fibonacci = {0, 50000,80000,0,50000,0,0,0,10,false,false,false,false};
	Gerador_Pontos.tid = tid_Gerador_pontos;
	Uart_Subscriber.tid = tid_UART_Subscriber;
	Controle.tid = tid_Controle;
	Primos.tid = tid_Primo;
	Fibonacci.tid = tid_Fibonacci;
	
	Threads[PRI]=Primos;
	Threads[UAR]=Uart_Subscriber;
	Threads[GER]=Gerador_Pontos;
	Threads[CON]=Controle;
	Threads[FIB]=Fibonacci;
	
	
	
	while(1){
		if(!master_fault){
//					printString("N");
			//------------------------------------------------------------------------------------
			//Se tiver sinal do timer, Passa as threads que precisarem executar para o mode waiting (adiciona na thread)
			evtSignal = osSignalWait (0x01, 0);
			if (evtSignal.status == osEventSignal)  {
						printString("TICK\n\r");
				if(true){//MUDAR PARA CADA 0,1ms


							//Cada cont equivale a 0.1s, ativa threads de acordo com sua frequência
							cont++;
							periodic_start(ticks, &Threads[CON]);
							periodic_start(ticks, &Threads[GER]);
							if(cont%2==0){
								periodic_start(ticks, &Threads[PRI]);
							}
							if(cont%10==0){
								periodic_start(ticks, &Threads[FIB]);
							}
						}
					}
			//------------------------------------------------------------------------------------

			//Checa e ativa a thread que tem maior prioridade
//			evtSignal = osSignalWait (0x02, 0);
//			if (evtSignal.status == osEventSignal)  {	
				if(true){
				for(i=0;i<5;i++){
					if(Threads[i].awaiting){
						if(Threads[i].dynamic_priority < aux.dynamic_priority){
							aux=Threads[i];
						}
					}
				}
					execute=true;
//				printString("Escolhi essa thread= ");
				if(aux.tid==Threads[0].tid){
					printString("Primo");
				}
				else if(aux.tid==Threads[1].tid){
					printString("UART");
				}
				else if(aux.tid==Threads[2].tid){
					printString("Gerador");
				}
				else if(aux.tid==Threads[3].tid){
					printString("Controle");
				}
				else if(aux.tid==Threads[4].tid){
					printString("Fibonnaci");
				}
				else{
					execute=false;
//					printString("W");
				}
	//				printString("\n\r");
				
				if(execute){
								osSignalSet(aux.tid,0x1);
								evtMessage = osMessageGet(tid_exit_status, osWaitForever);

								if (evtMessage.status == osEventMessage){
									exit_status=(int) evtMessage.value.p;

									
									switch(exit_status){
										case 0: //Still running
											
										break;
										case 1: //Exit sucessfully	
											stop_thread(ticks, &aux);
										break;
										case 2:
										break;
										case 3:							
										break;
										case 4:
										break;
										default:
										break;
									
									}
								}
							}
			}
							
			for(i=0;i<5;i++){
				update_info(ticks, &Threads[i]);
			}
		}
			for(i=0;i<5;i++){
//				print_display(Threads[i], i);
			} 
		
		//Checa se o botão foi pressionado. Gera Gantt
		if(s1_button){
				osSignalSet(tid_UART_Publish,0x1);
		}	
		

	}
	
}



int main (void) {
//	char pBuf[20];

	osKernelInitialize();
			init_all();		
			printString("TesteMain1\n\r");


	init_sidelong_menu();

	Init_Thread();
	printString("TestenMain2\n\r");
	if(osKernelStart()!=osOK){
	}
	osDelay (osWaitForever);

}










void TIMER0A_Handler(void){
	//clear na interrupção do timer
	TIMER0->ICR |= (1<<0);
	
	//Sinaliza para a thread atualizar PWM
	osSignalSet(tid_Escalonador,0x1);
	osSignalSet(tid_Controle,0x1);
}



void UART0_Handler(void){
	//A LEITURA É RESPONSÁVEL PELO CLEAR DA INTERRUPÇÃO
	char c = readChar();
	
	osMessagePut(received_char_id,	c, 0);//readChar(), osWaitForever);
//	osSignalSet(tid_UART_Subscriber,0x1);
}
