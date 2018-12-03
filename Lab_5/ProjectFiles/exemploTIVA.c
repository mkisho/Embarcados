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
#define ticks_factor  100
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

//======================

//TODO Escalonar												80%
//TODO Imprimir informações							70%
//TODO Ler comandos											0%
//TODO Gerar Gantt											0%
//TODO Comentar													20%



//    fprintf(file,"gantt\n");
//    fprintf(file,"    title A Gantt Diagram\n");
//    fprintf(file,"    dateFormat x\n");

// Struct que define as características de uma thread, como a sua prioridade, progresso e núm de ticks.
typedef struct {
	osThreadId tid; //Id da thread
	
	uint32_t estimated_ticks; // Num de ticks estimados que a thread leva para executar
	uint32_t max_ticks; // Num de ticks maximos que a thread pode levar para executar
	uint32_t execution_ticks; //Ticks que a thread levou para executar até o momento
	uint32_t relax_ticks; // Numero de ticks de relaxamento
	uint32_t start_tick; //Momento em que a thread iniciou
	int16_t static_priority; //Prioridade estática
	int16_t dynamic_priority; //Prioridade dinâmica
	uint16_t periodicity; //Periodicidade da thread
	int progress; //Porcentagem de progresso da thread
	bool master_fault; //Flag que sinaliza master fault da thread.
	bool secundary_fault; //Variavel para contar quantas secondary faults ocorreram
	bool running; //Flag que indica se a thread está executando
	bool waiting; //Flag que indica se a thread está em espera para executar
	int simple_id; //Defina em qual coluna da tela a thread será impressa.
} ThStatus;

ThStatus Threads[6]; //Cria a fila de threads

//Struct que define os angulos e posicoes dos motores
typedef struct pwm{
	int pwm_x;
	int pwm_y;
	int pwm_z;
}PWM;

//Struct que define os pontos de cada um dos desenhos
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
osThreadDef (UART_Publish, osPriorityHigh, 1, 0);       // thread object

void UART_Subscriber (void const *argument);               // thread function
osThreadId tid_UART_Subscriber;                            // thread id
osThreadDef (UART_Subscriber, osPriorityHigh, 1, 0);       // thread object

void Gerador_pontos(void const *argument);                // thread function
osThreadId tid_Gerador_pontos;                            // thread id
osThreadDef (Gerador_pontos, osPriorityNormal, 1, 0);     // thread object

void Fibonacci (void const *argument);               // thread function
osThreadId tid_Fibonacci;                            // thread id
osThreadDef (Fibonacci, osPriorityHigh, 1, 0);       // thread object

void Primo (void const *argument);               // thread function
osThreadId tid_Primo;                            // thread id
osThreadDef (Primo, osPriorityHigh, 1, 0);       // thread object

void Escalonador (void const *argument);             // thread function
osThreadId tid_Escalonador;                          // thread id
osThreadDef (Escalonador, osPriorityRealtime, 1, 0); // thread object

osMessageQDef(received_char, 5, uint32_t); // Declare a message queue
osMessageQId (received_char_id);           // Declare an ID for the message queue

osMessageQDef(update_int, 5, uint32_t); // Declare a message queue
osMessageQId (update_int_id);           // Declare an ID for the message queue

osMessageQDef(newPoint, 5, uint32_t); // Declare a message queue
osMessageQId (newPoint_id);           // Declare an ID for the message queue

osMessageQDef(exit_status, 5, uint32_t); // Declare a message queue
osMessageQId (tid_exit_status);           // Declare an ID for the message queue

Point point={0,0};

uint16_t type_geo=4;
uint16_t sec_fault=0;
bool master_fault=false;


//Funcao que converte int para string
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






//funcao que converte char para int
int charToInt(char c){
		int n=0;
		n = c-48;
		return n;
}

//funcao que inicializa tudo que precisa ser inicializado
void init_all(){
	//Init each driver
	UART_init();
	PWM_init();
	timer_init();
	cfaf128x128x16Init();
}

//Funcao que gera os pontos de cada uma das formas geometricas
Point gerarOnda(uint16_t type, uint16_t cont){

	Point point;
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
		
	}
	return point;
}

//Funcao que retorna o PWM para que um ponto especifico seja calculado
PWM calcula_PWM(float x, float y){
	float g;
	float d;
	PWM pwm;	
	g=atan(x/(y+10));
	pwm.pwm_z=2500.0-13.33*g*3.0*(180.0/3.14);
	d=sqrt(x*x+(y+10)*(y+10))-10;
	pwm.pwm_y = 2800+d*300;
	pwm.pwm_x = 2500+d*330;
	return pwm;
}

//Thread que calcula os numeros de fibonacci
//6400 ticks
//30%->8320
void Fibonacci (void const *argument) {
	osEvent evtSignal;
	int ticks;
	int a;
	int b;
	int aux;
	int cont;
	while(1){
		evtSignal = osSignalWait (0x01, osWaitForever);
		if (evtSignal.status == osEventSignal)  {	
			ticks= osKernelSysTick()/ticks_factor;
			for(cont=0;cont<100;cont++){
				aux = a + b;
				a = b;
				b = aux;
				if(cont%5==0){
					Threads[FIB].progress=cont/1;
					osMessagePut(tid_exit_status, 0, osWaitForever);
					osSignalWait(0x01, osWaitForever);
				}
			}
			osMessagePut(tid_exit_status, 1, osWaitForever);
		}
	}
}

//Thread que calcula a sequencia dos numeros primos
//2700 ticks
// 30% -> 3510
void Primo (void const *argument) {
	int ticks;
	int time;
	int n=1;
	char pBuf[10];
	int i =2;
	int max;
	osEvent evtSignal;
	while(1){
		evtSignal = osSignalWait (0x01, osWaitForever);
		if (evtSignal.status == osEventSignal){	
			time= osKernelSysTick()/ticks_factor;	
			max = sqrt(n);
			for (i =2; i<=n;i++){
				if (!(n%i)){
					break;
				}
				if(i%10==0){
					Threads[PRI].progress=i/max;
					osMessagePut(tid_exit_status, 0, osWaitForever);
					osSignalWait(0x01,osWaitForever);
				}
			}
			n++;
			osMessagePut(tid_exit_status, 1, osWaitForever);
		}
	}
}

//Thread que gera os pontos da cada forma geometrica
//2700 a 12000 ticks
//70%- 4590 a 20400
void Gerador_pontos (void const *argument) {
	int ticks;
	PWM pwm;
	uint16_t step=0;
	uint16_t type=0;
	uint16_t cmd=0;
	osEvent evtSignal;
	osEvent evtMessage;
	uint16_t count=0;
	char pBuf[20];

	while(1){
		evtSignal = osSignalWait(0x01, osWaitForever);	
		if (evtSignal.status == osEventSignal)  {			
			step=0;

			
			Threads[GER].progress=20;
			osMessagePut(tid_exit_status, 0, osWaitForever);
			osSignalWait(0x01,osWaitForever);				
	
			if(type_geo<3){
				point=gerarOnda(type_geo, count);
				if(count==500){
					count=0;
				}
			}
			else if(type_geo==3){
					point=gerarOnda(type, count);
					if(count==500){
							count=0;
					}
					if(type==0){
						if(count>300){
							count=0;
							type=1;
						}
					}
					else if(type==1){
						if(count>200){
							count=0;
							type=2;
						}
					}
					else {
						if(count>400){
							count=500;
							type=0;
							type_geo=4;
						}
					}
			}
			Threads[GER].progress=80;						
			osMessagePut(tid_exit_status, 0, osWaitForever);
			osSignalWait(0x01,osWaitForever);

			if(type_geo<4){
				count+=3;
			}
			else
				count=500;
			
			
			if(type_geo==0){
				if(count>300){
						count=500;
						type_geo=4;
				}
			}
			else if(type_geo==1){
				if(count>200){
					count=500;
					type_geo=4;
				}
			}
			else if(type_geo==2){
				if(count>400){
					count=500;
					type_geo=4;	
				}
			}
			osMessagePut(tid_exit_status, 1, osWaitForever);	
		}
	}
}

//Thread que faz o controle dos motores
//8000 ticks
//40% -> 11200
void Controle (void const *argument) {
	int ticks;
	PWM pwm;
	uint16_t step=0;
	uint16_t type=0;
	uint16_t cmd=0;//Receive Uart Valuer
	osEvent evtSignal;
	osEvent evtMessage;
	uint16_t count=0;
	pwm.pwm_x = 3700;
	pwm.pwm_y = 3000;
	pwm.pwm_z = 2100;
	
	while(1){
		evtSignal = osSignalWait (0x01, osWaitForever);
		if (evtSignal.status == osEventSignal)  {	
			ticks = osKernelSysTick()/ticks_factor;
			pwm=calcula_PWM(point.x,point.y);

			Threads[CON].progress=70;						
			osMessagePut(tid_exit_status, 0, osWaitForever);
			osSignalWait(0x01,osWaitForever);
				
			PWM_set_duty(0,pwm.pwm_z);
			PWM_set_duty(2,pwm.pwm_x);
			PWM_set_duty(4,pwm.pwm_y);
													
			step++;
			osMessagePut(tid_exit_status, 1, osWaitForever);
		}
	}
}

//Thread utilizada para imprimir o Diagrama de Gantt
void UART_Publish (void const *argument) {
	int time;
	osEvent evt;
	osEvent evtSignal;
	char cmdType;
	int  value;
	
	while(1){	
		evtSignal = osSignalWait (0x01, osWaitForever);
		if (evtSignal.status == osEventSignal){	
			time = osKernelSysTick()/ticks_factor;
			evt = osMessageGet(update_int_id,osWaitForever);
			cmdType=(uint32_t) evt.value.p;
			evt = osMessageGet(update_int_id,osWaitForever);
			value=(int32_t) evt.value.p;
			printString("\n\r\n\r\n\r\n\r\n\r\n\r");
			osMessagePut(tid_exit_status, 1, osWaitForever);
		}
	}
}

//Thread que recebe informacoes da UART
//10%	
void UART_Subscriber (void const *argument) {
	int ticks;
	osEvent evtSignal;
	uint16_t n=1;
//	char recChar;
	while(1){
		
		evtSignal = osSignalWait (0x01, osWaitForever);
		if (evtSignal.status == osEventSignal)  {	
					char c = readChar();
					switch(c){
						case 'r':
							type_geo=0;
						break;
						case 'l':
							type_geo=1;
						break;
						case 'c':
							type_geo=2;
						break;
						case 'b':
							type_geo=3;
						break;
						default:
						break;
					
					}

					
					osMessagePut(tid_exit_status, 1, osWaitForever);
		}
					
	}
		osThreadYield (); 
}

//Funcao que inicializa as threads
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

//Funcao que inicializa o menu do display
void init_sidelong_menu(){
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
	
	//Escreve menu lateral:
	GrStringDraw(&sContext,"PR", -1, (sContext.psFont->ui8MaxWidth+2)*1*3, (sContext.psFont->ui8Height+2)*1, true);
	GrStringDraw(&sContext,"UA", -1, (sContext.psFont->ui8MaxWidth+2)*2*3, (sContext.psFont->ui8Height+2)*1, true);
	GrStringDraw(&sContext,"GP", -1, (sContext.psFont->ui8MaxWidth+2)*3*3, (sContext.psFont->ui8Height+2)*1, true);
	GrStringDraw(&sContext,"CO", -1, (sContext.psFont->ui8MaxWidth+2)*4*3, (sContext.psFont->ui8Height+2)*1, true);
	GrStringDraw(&sContext,"FI", -1, (sContext.psFont->ui8MaxWidth+2)*5*3, (sContext.psFont->ui8Height+2)*1, true);

	GrStringDraw(&sContext,"Pr ", -1, 0, (sContext.psFont->ui8Height+2)*2, true);
	GrStringDraw(&sContext,"TE ", -1, 0, (sContext.psFont->ui8Height+2)*3, true);
	GrStringDraw(&sContext,"Es ", -1, 0, (sContext.psFont->ui8Height+2)*4, true);
	GrStringDraw(&sContext,"PE ", -1, 0, (sContext.psFont->ui8Height+2)*5, true);
	GrStringDraw(&sContext,"At ", -1, 0, (sContext.psFont->ui8Height+2)*6, true);
	GrStringDraw(&sContext,"Fi ", -1, 0, (sContext.psFont->ui8Height+2)*7, true);
	GrStringDraw(&sContext,"Fa ", -1, 0, (sContext.psFont->ui8Height+2)*8, true);	
}

//Funcao que imprime informacoes das threads no display
void print_display (ThStatus thread, uint16_t n){
	bool s1_button;
	int display;
	char pBuf[10];
	n+=(2*(n+1));
	
	intToString(thread.static_priority, pBuf, 10, 10, 2);
	
	GrStringDraw(&sContext,pBuf, -1, (sContext.psFont->ui8MaxWidth+2)*n, (sContext.psFont->ui8Height+2)*2, true);
	intToString(thread.relax_ticks/ticks_factor, pBuf, 10, 10, 2);	
	GrStringDraw(&sContext,pBuf, -1, (sContext.psFont->ui8MaxWidth+2)*n, (sContext.psFont->ui8Height+2)*3, true);

//	GrStringDraw(&sContext,"Es", -1, (sContext.psFont->ui8MaxWidth+2)*n, (sContext.psFont->ui8Height+2)*4, true);
	intToString(thread.progress, pBuf, 10, 10, 2);
//	GrStringDraw(&sContext,pBuf, -1, (sContext.psFont->ui8MaxWidth+2)*n, (sContext.psFont->ui8Height+2)*5, true);

//	GrStringDraw(&sContext,"At", -1, (sContext.psFont->ui8MaxWidth+2)*n, (sContext.psFont->ui8Height+2)*6, true);

//	GrStringDraw(&sContext,"Fi", -1, (sContext.psFont->ui8MaxWidth+2)*n, (sContext.psFont->ui8Height+2)*7, true);
	intToString(sec_fault, pBuf, 10, 10, 2);
	GrStringDraw(&sContext,pBuf, -1, (sContext.psFont->ui8MaxWidth+2)*3, (sContext.psFont->ui8Height+2)*9, true);
}

//Funcao que atualiza as informacoes das threads
void update_info(uint32_t current_ticks, ThStatus *thread){
	thread->execution_ticks=current_ticks-thread->start_tick;
	thread->relax_ticks=thread->estimated_ticks-(current_ticks-thread->start_tick);
//	thread->dynamic_priority=
	if(thread->execution_ticks>thread->max_ticks){
		if(thread->static_priority==REALTIME){
			//thread->master_fault=true;
			master_fault=true;
		}
		else{
			thread->secundary_fault=true;

		}
	}
	
//	thread->dynamic_priority=-100+(100-thread->dynamic_priority)*progress
	
}

//Funcao encerra uma execucao da thread e redefine seus parametros iniciais
void stop_thread(uint32_t current_ticks, ThStatus *thread){
	thread->waiting=false;
	thread->progress=00;
	thread->execution_ticks=current_ticks-thread->start_tick;
	if(thread->secundary_fault){
		thread->static_priority--;
	}
	if(thread->execution_ticks<(thread->max_ticks)/2){
		thread->secundary_fault=true;
		if(thread->static_priority<100)
			thread->static_priority++;
	}
	if(thread->secundary_fault){
		sec_fault++;
		thread->secundary_fault=false;
	}
	
}

//Funcao que inicia uma execucao da thread e define seus parametros iniciais
void periodic_start(uint32_t current_ticks, ThStatus *thread){
		thread->waiting=true;
		thread->dynamic_priority=thread->static_priority;
		thread->start_tick=current_ticks;
		thread->relax_ticks=thread->max_ticks-thread->estimated_ticks;
		thread->progress=1;
		print_display(*thread, (*thread).simple_id);
}

//Thread que realiza o escalonamento das threads. O escalonador propriamente dito.
void Escalonador(void const *argument) {
	bool s1_button=false;
	bool execute=true;
	osEvent evtSignal;
	osEvent evtMessage;
	int i=0;
	int cont=0;
	int time=0;
	int exit_status=0;
	uint16_t secondary_faults=0;
//	bool master_fault=false;
	uint32_t ticks=0;	
	char pBuf[15];
	uint32_t start;
	uint32_t stop_tick;
	uint32_t effective_time;
	uint32_t init_time;
	int auxTid=0;
	int auxDyn=100;
	ThStatus *aux;
	ThStatus Gerador_Pontos = {0, 9000  ,48000 ,0,0,0,10,10,1,0,false,0,false,false,GER};

	ThStatus Uart_Subscriber ={0, 5000 ,40000 ,0,0,0,-30,-30,0,0,false,0,false,false, UAR};
	ThStatus Controle = 			{0, 8000  ,48000 ,0,0,0,0,0,1,0,false,0,false,false, CON};
	ThStatus Primos = 				{0, 2700  ,268100  ,0,0,0,-100,-100,2,0,false,0,false,false, PRI};
	ThStatus Fibonacci = 			{0, 2305  ,78580  ,0,0,0,0,0,10,0,false,0,false,false, FIB};
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
	
	effective_time= osKernelSysTick()/ticks_factor;	
	init_time= osKernelSysTick()/ticks_factor;	

	while(1){


		if(!master_fault){
			//Se tiver sinal do timer, Passa as threads que precisarem executar para o mode waiting (adiciona na thread)
			evtSignal = osSignalWait (0x01, 0);
			if (evtSignal.status == osEventSignal)  {
				if(true){
					cont++;
					ticks=osKernelSysTick()/ticks_factor;
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
			if(	UART0->RIS &= (1<<4)){
					periodic_start(ticks, &Threads[UAR]);
			}
			
			
			
			
			
			
			
			
			
			//Checa e ativa a thread que tem maior prioridade
//			evtSignal = osSignalWait (0x02, 0);
//			if (evtSignal.status == osEventSignal)  {	
				if(true){
					auxTid=0;
					auxDyn=100000;
				for(i=0;i<5;i++){
					if(Threads[i].waiting){
						if(Threads[i].dynamic_priority < auxDyn){
							aux=&Threads[i];
							auxTid=(int32_t) Threads[i].tid;
							auxDyn=Threads[i].dynamic_priority;
						}

					}
				}


				
				execute=true;
				if(auxTid== (int32_t) Threads[0].tid){
					printString("P: ");
				}
				else if(auxTid== (int32_t)Threads[1].tid){
					printString("U: ");
				}
				else if(auxTid==(int32_t)Threads[2].tid){
					printString("G: ");
				}
				else if(auxTid==(int32_t)Threads[3].tid){
					printString("C: ");
				}
				else if(auxTid==(int32_t)Threads[4].tid){
					printString("F: ");
				}
				else{
					execute=false;
				}

				if(execute){
					start = osKernelSysTick()/ticks_factor;
					osSignalSet((*aux).tid,0x1);
					evtMessage = osMessageGet(tid_exit_status, osWaitForever);
					
					stop_tick = osKernelSysTick()/ticks_factor;

					intToString(effective_time-init_time, pBuf, 10, 10, 1);
					printString(pBuf);
					printString(", ");
					intToString(stop_tick-start, pBuf, 10, 10, 1);
					printString(pBuf);
					printString("\n\r");
					
					effective_time+=stop_tick-start;					
								if (evtMessage.status == osEventMessage){
									exit_status=(int) evtMessage.value.p;

									switch(exit_status){
										case 0: //Still running											
										break;
										case 1: //Exit sucessfully	
											stop_thread(stop_tick, aux);
										break;
										default:
										break;
										
									}
								}
							}
			}
							
			for(i=0;i<5;i++){
				update_info(stop_tick, &Threads[i]);
			}
		}
		else{
			printString("MASTER FAULT");
		
		}
		
		//Checa se o botão foi pressionado. Gera Gantt
		if(s1_button){
				intToString(osKernelSysTick()/ticks_factor-time, pBuf, 10, 10, 1);
				printString(pBuf);
				printString("\n\r:");							
				osSignalSet(tid_UART_Publish,0x1);
		}	
	}
	
}

//Funcao main que realiza as inicializacoes do programa
int main (void) {

	osKernelInitialize();
	init_all();		
	init_sidelong_menu();
	Init_Thread();
	if(osKernelStart()!=osOK){
	}
	osDelay (osWaitForever);

}

//Funcao que periodicamente manda sinais para o escalonador
void TIMER0A_Handler(void){
	//clear na interrupção do timer
	TIMER0->ICR |= (1<<0);
	
	//Sinaliza para a thread atualizar PWM
	osSignalSet(tid_Escalonador,0x1);
}


//void UART0_Handler(void){
	//A LEITURA É RESPONSÁVEL PELO CLEAR DA INTERRUPÇÃO
//	char c = readChar();
	
//	osMessagePut(received_char_id,	c, 0);//readChar(), osWaitForever);
//	osSignalSet(tid_UART_Subscriber,0x1);
//}
