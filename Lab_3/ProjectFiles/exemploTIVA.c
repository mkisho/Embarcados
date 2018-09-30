
#include "cmsis_os.h"
#include "TM4C129.h"                   
#include <stdbool.h>
#include "grlib/grlib.h"

#include "rgb.h"
#include "cfaf128x128x16.h"
#include "buttons.h"
#include "buzzer.h"
#include "joy.h"
#include "led.h"

#define LED_A      0
#define LED_B      1
#define LED_C      2
#define LED_D      3
#define LED_CLK    7

#define jogador 	0
#define tiro 			1
#define barco 		2
#define aviao 		3
#define copter 		4
#define fuel  		5
#define ponte 		6
#define display 		7



tContext sContext;
//uint16_t x=50;
uint16_t velocidade=2;
uint16_t progresso=0;
uint16_t combustivel=40;

struct obstaculo{
	uint16_t type;
	uint16_t x;
	uint16_t y;
	uint16_t alive;
};

struct cenario{
	uint16_t tamanho;
	uint16_t y;
	uint16_t half;
};


const unsigned char Player_Image[] = {3,
0,
12,
0,
48, 
3,
240, 
63,
243,
255,
255,
51,
240,
195,
3,
0,
63,
3,
255,
12,
204,};


const unsigned char Bullet_Image[] = { 255,255,255//6, 102, 96,
};


const unsigned char Plane_Image[] = {1};
const unsigned char Ship_Image[] = {0,
1,
192,
0,
1,
192,
0,
1,
248,
0,
127,
248,
255,
255,
255,
31,
255,
255,
3,
255,
255,
3,
255,
248,};
const unsigned char Copter_Image[] = {0, 
63,
3,
240, 
0,
48, 
0,
252,
195,
255, 
255,
255,
255,
255,
192,
252,
0,
48,
0,
252,};
const unsigned char Bridge_Image[] = {1};
const unsigned char Fuel_Image[] = {206,
7,
231,
254,
31,
231,
254,
127,
255,
254,
103,
230,
126,
103,
224,
127,
255,
224,
126,
127,
225,
254,
127,
224,
127,
255,
231,
254,
127,
231,
254,
127,
231,
254,
7,
255,
240,};

	


const unsigned char display_fuel[]={0xff,0xff,0xff,0xff,0xff,0xff,0xb8,0x00,0x01,0x80,0x00,0x1d,0xb8,0x00,0x01,0x80,0x00,0x1d,0xb8,0x00,0x01,0x80,0x00,0x1d,0x80,0x00,0x10,0x00,0x00,0x01,0xbe,0x00,0x30,0x60,0x00,0x7d,0xbe,0x00,0x10,0x80,0x00,0x7d,0xb0,0x00,0x11,0x00,0x00,0x61,0xb0,0x00,0x3a,0x00,0x00,0x61,0xbc,0x00,0x05,0xc0,0x00,0x79,0xb0,0x00,0x08,0x40,0x00,0x61,0xb0,0x00,0x11,0xc0,0x00,0x61,0xbe,0x00,0x61,0x00,0x00,0x61,0xbe,0x00,0x01,0xc0,0x00,0x61,0x80,0x00,0x00,0x00,0x00,0x01,0xff,0xff,0xff,0xff,0xff,0xff};

	
//valores entre 5 e 11
struct cenario cenarioList[] = {
{5,0,0},
{6,50,0},
{8,100,0},
{11,150,0},
{7,200,0},
{7,1200,0},
{7,1500,0},
{7,2000,0},
{7,2100,0},
{7,2200,0},
};

	



struct obstaculo obstacleList[]={
{2,50,0,1},
{4,50,50,1},
{5,20,100,1},
{2,50,130,1},
{2,60,160,1},
{4,70,190,1},
{2,30,300,1},
{2,50,400,1},
{2,50,450,1},
{2,50,500,1},
{2,50,700,1},
{2,50,730,1},
{2,50,1000,1},
};
	


struct obstaculo bullet={1,-1,-1,1};
struct obstaculo player={1,50,90,1};
	

void GameState (void const *argument);               // thread function
osThreadId tid_GameState;                            // thread id
osThreadDef (GameState, osPriorityNormal, 1, 0);     // thread object

void Obstaculos (void const *argument);               // thread function
osThreadId tid_Obstaculos;                            // thread id
osThreadDef (Obstaculos, osPriorityNormal, 1, 0);     // thread object

void VeiculoJogador (void const *argument);               // thread function
osThreadId tid_VeiculoJogador;                            // thread id
osThreadDef (VeiculoJogador, osPriorityNormal, 1, 0);     // thread object

void Cenario (void const *argument);               // thread function
osThreadId tid_Cenario;                            // thread id
osThreadDef (Cenario, osPriorityNormal, 1, 0);     // thread object

void InteracaoUsuario (void const *argument);               // thread function
osThreadId tid_InteracaoUsuario;                            // thread id
osThreadDef (InteracaoUsuario, osPriorityNormal, 1, 0);     // thread object

void PainelControle (void const *argument);               // thread function
osThreadId tid_PainelControle;                            // thread id
osThreadDef (PainelControle, osPriorityNormal, 1, 0);     // thread object

osTimerId timer_Id;

//osSemaphoreId sid_Thread_Semaphore;                    // semaphore id
//osSemaphoreDef (Semaforo);                      // semaphore 

osMutexDef (display_mutex);    // Declare mutex
osMutexId  (display_mutex_id); // Mutex ID



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

















int check_colision(uint16_t type, uint16_t x,	uint16_t y, uint16_t j){
	uint16_t height;
	uint16_t width;
		if(type==barco){
			width=24;
			height=10;
		}
		else if(type==aviao){
			width=24;
			height=10;
		}
		else if(type==copter){
			width=16;
			height=10;			
		}
		else if(type==fuel){
			width=12;
			height=12;			
		}
		else if(type==ponte){
			width=24;
			height=10;			
		}
		
		//Checando por hit de tiro
		if(x < bullet.x){
			if( x+width > bullet.x){
				if(y < bullet.y && y+height > bullet.y){
					return 1;
				}
			}
		}
		else if(x < bullet.x+3){
			if( x+width > bullet.x+3){
				if(y < bullet.y && y+height > bullet.y){
					return 1;
				}
			}
		}
		//checando por hit de player
		if(x < player.x){
			if( x+width > player.x){
				if(y < player.y && y+height > player.y){
					return 2;
				}
			}
		}
		else if(x < (player.x+14)){
			if( x+width > (player.x+14)){
				if(y < player.y && y+height > player.y){
					return 2;
				}
			}
		}

	return 0;
}







void print(int clean, int x, int y, int type){
		int n;
		int m;
		int i=x;
		int j=y;
		int width=0;
		uint16_t size;
		const unsigned char *Image;
		if(type==jogador){
			Image=Player_Image;
			width=14;
			size=21;
		}
		else if(type==tiro){
			Image=Bullet_Image;
			width=4;
			size=3;
		}
		else if(type==barco){
			Image=Ship_Image;
			width=24;
			size=24;
		}
		else if(type==aviao){
			Image=Plane_Image;
			width=15;
			size=23;
		}
		else if(type==copter){
			Image=Copter_Image;
			width=16;
			size=20;
		}
		else if(type==fuel){
			Image=Fuel_Image;
			width=12;
			size=36;
		}
		else if(type==ponte){
			Image=Bridge_Image;
			width=24;
			size=23;
		}
		else if(type==display){
			Image=display_fuel;
			width=48;
			size=96;
		}
		
		if(clean){
			GrContextForegroundSet(&sContext, ClrBlue);
			for(n=0; n<size; n++){
				for(m=7;m>=0;m--){
				GrPixelDraw(&sContext,j,i);
				j++;
				if(j>=width+y){
						j=y;
						i++;
					}
				}
			}
			return;
		}
		
		if(type>1 && type <7){
			i=x-velocidade;
			GrContextForegroundSet(&sContext, ClrBlue);
			for(n=0; n<=velocidade; n++){
						for(m=y;m<=width+y;m++){
							GrPixelDraw(&sContext,j,i);
							j++;
							if(j>=width+y){
								j=y;
								i++;
						}
					}
				}
		}
		if(type==1){
			i=x+6;
			j=y;
			GrContextForegroundSet(&sContext, ClrBlue);
						for(m=y;m<=width+y;m++){
							GrPixelDraw(&sContext,j,i);
							j++;
				}
		}
		
		if(type==0){
			i=x;
			j=y-2;
			GrContextForegroundSet(&sContext, ClrBlue);
			for(n=5;n<=7;n++){
				for(m=0;m<=1;m++){
							GrPixelDraw(&sContext,y-2+m,x+n);
				}
			}
			for(n=5;n<=7;n++){
				for(m=0;m<=1;m++){
							GrPixelDraw(&sContext,y+width+m,x+n);
				}
			}
				
		}
		
		
		
		i=x;
		j=y;
		for(n=0; n<size; n++){
				for(m=7;m>=0;m--){
						if(Image[n] & (1 << m))
							GrContextForegroundSet(&sContext, ClrYellow);
						else{
							
							GrContextForegroundSet(&sContext, ClrBlue);
							if(type==7){
								GrContextForegroundSet(&sContext, ClrGray);
							}
						}
				GrPixelDraw(&sContext,j,i);
				j++;
				if(j>=width+y){
						j=y;
						i++;
				}
			}
		}
		
}

void print_line(uint16_t size, uint16_t y, uint16_t type){
	uint16_t i;
	for(i=20; i<108; i++){
		if(i<44 || i>84){
			if((64-size*4)<i && i<(64+size*4))
				GrContextForegroundSet(&sContext, ClrBlue);
			else
				GrContextForegroundSet(&sContext, ClrGreen);
			GrPixelDraw(&sContext,i,y);
		}
	}
	
}



void print_painel(uint16_t  combustivel,uint16_t  pontuacao, uint16_t  vidas){
			char pbufx[10];

			GrContextForegroundSet(&sContext, ClrYellow);
			GrContextBackgroundSet(&sContext, ClrGray);
			intToString(pontuacao, pbufx, 4, 10, 4);
			
  		GrStringDraw(&sContext, (char*)pbufx, -1,50, 105,true);
			intToString(vidas, pbufx, 4, 10, 1);
			GrStringDraw(&sContext, pbufx, -1,(sContext.psFont->ui8MaxWidth)*5, (sContext.psFont->ui8Height+2)*12,true);
		  
			print(0,112,40,7);
			print(0,114 , combustivel+42, 1);
				print(0,118 , combustivel+42, 1);
//			GrStringDraw(&sContext, "ms", -1,(sContext.psFont->ui8MaxWidth)*14, (sContext.psFont->ui8Height+2)*5,true);
}







void init_all(){
	cfaf128x128x16Init();
	joy_init();
	buzzer_init(); 
	button_init();
	rgb_init();
}


void init_sidelong_menu(){
	uint8_t i;
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);

}




void GameState (void const *argument) {


}



void InteracaoUsuario (void const *argument) {
		uint16_t horiz =0;
		uint16_t vert =0;
		uint16_t velocidade=2;
		osEvent evt;
		bool s1_press;
		bool s2_press;
		while(1){
		evt = osSignalWait (0x01, 10000);
		if (evt.status == osEventSignal)  {
			horiz = joy_read_x();
			vert = joy_read_y();
		
	//			center = joy_read_center();
			if(horiz>0x900){
				player.x++;
				player.x++;
			}
			else if(horiz<0x700){
				player.x--;
				player.x--;
			}
			if(vert>0x850){
				velocidade=3;
			}
			else if(vert<0x700){
				velocidade=1;
			}
			s1_press=button_read_s1();
			if(s1_press==true){
				osSignalSet(tid_VeiculoJogador,0x2);
			}
			s2_press=button_read_s2();
			if(s2_press==true){
				buzzer_write(true);
			}
		}
		//osDelay(200);	
		}
	
}




void Cenario (void const *argument) {
				uint16_t i,j,val=1;
				int m,n;
				osEvent evt;
				i=1;
				j=1;
				while(1){
					evt = osSignalWait (0x01, 10000);
						if (evt.status == osEventSignal)  {
							progresso+=velocidade;
							if(progresso-cenarioList[i].y>100){
								i++;
							}
							j=i;
							val = osMutexWait (display_mutex_id, 1000);
							switch (val) {
								case osOK:
									while(progresso-cenarioList[j].y>0){
										n=cenarioList[j-1].tamanho-cenarioList[j].tamanho;
											if(n>0){
												for(m=0;m<=n;m++){
													if(progresso-cenarioList[j].y>0){														
														print_line(cenarioList[j].tamanho-(m-n), progresso-cenarioList[j].y-m,0);
													}
												}
											}
											else if (n<0){
												n=-n;
												for(m=0;m<=n;m++){
													if(progresso-cenarioList[j].y>0){
														print_line(cenarioList[j].tamanho+(m-n), progresso-cenarioList[j].y-m,0);
													}
												}
											}
										j++;
									}
									osMutexRelease (display_mutex_id);
									break;
								case osErrorResource:
									break;
								case osErrorParameter:
									break;
								default:
									break;
							}		
					}				
//					if(true){
//						osSignalSet(tid_Obstaculos,0x2);
//					}					
				}
}
void VeiculoJogador (void const *argument) {
	osEvent evt;
	uint16_t val;
	
	
	while(1){
		bullet.y--;
		evt = osSignalWait (0x01, 10000);
		if (evt.status == osEventSignal)  {
			val = osMutexWait (display_mutex_id, 1000);// wait 10 mSec
			switch (val) {
				case osOK:
					print(0,player.y,player.x,0);
					if(bullet.y>0){
						print(0,bullet.y,bullet.x,1);
					}
					osMutexRelease (display_mutex_id);
					break;
				case osErrorResource:
					break;
				case osErrorParameter:
					break;
				default:
					break;
			}
		evt = osSignalWait (0x02, 10);
		if (evt.status == osEventSignal)  {
			bullet.x=player.x+6;
			bullet.y=90;
		}
		
		osThreadYield (); 
		}
	}
	
	
}
void PainelControle (void const *argument) {
	
	uint16_t   pontuacao=0;
	uint16_t   vidas=3;
	uint16_t val;
	osEvent evt;
	while(1){
		evt = osSignalWait (0x01, 10000);
		if (evt.status == osEventSignal)  {
		
			val = osMutexWait (display_mutex_id, 1000);
			switch (val) {
				case osOK:
					if(combustivel>0){
						combustivel--;
					}
				print_painel(combustivel, pontuacao, vidas);
					osMutexRelease (display_mutex_id);
					break;
				case osErrorResource:
					break;
				case osErrorParameter:
					break;
				default:
					break;
			}
		osThreadYield (); 
		}
	}
}
void Obstaculos (void const *argument) {

	uint16_t val;
	osEvent evt;
	int i,j=0;
	int col;
	while(1){
		evt = osSignalWait (0x01, 10000);
		if (evt.status == osEventSignal)  {
			
			val = osMutexWait (display_mutex_id, 1000);
			switch (val) {
				case osOK:
					if(progresso-obstacleList[i].y>100){
						print(1,progresso-obstacleList[j].y,obstacleList[j].x,obstacleList[j].type);
						i++;
					}
					j=i;
					
					
					while(progresso-obstacleList[j].y>0){
//						obstacleList[j].y=obstacleList[j].y+velocidade;
						if(obstacleList[j].alive==1){
							col = check_colision(obstacleList[j].type,obstacleList[j].x,progresso-obstacleList[j].y, j);	
							print(0,progresso-obstacleList[j].y,obstacleList[j].x,obstacleList[j].type);
							if (col==1){
								print(1,progresso-obstacleList[j].y,obstacleList[j].x,obstacleList[j].type);
								obstacleList[j].alive=0;
								bullet.x=-1;
								bullet.y=-1;
							}
							else if(col==2){
								if(obstacleList[j].type!=fuel){
									print(1,progresso-obstacleList[j].y,obstacleList[j].x,obstacleList[j].type);
									obstacleList[j].alive=0;
									buzzer_write(true);
								}
								else
									combustivel++;
							}
							
						}
						j++;
					}
					osMutexRelease (display_mutex_id);
					break;
				case osErrorResource:
					break;
				case osErrorParameter:
					break;
				default:
					break;
			}
		osThreadYield (); 
		}
	}
			
}


void refresh (void const *n) {
	
	osSignalSet(tid_PainelControle,0x1);
	osSignalSet(tid_VeiculoJogador,0x1);
	osSignalSet(tid_Obstaculos,0x1);
	osSignalSet(tid_InteracaoUsuario,0x1);
	osSignalSet(tid_Cenario,0x1);
}

osTimerDef(timer_0, refresh);


int Init_Thread (void) {
	timer_Id= osTimerCreate(osTimer(timer_0), osTimerPeriodic, (void*)0);
	osTimerStart(timer_Id, 1200);

//	sid_Thread_Semaphore = osSemaphoreCreate(osSemaphore(Semaforo), 10);
//  if (!sid_Thread_Semaphore) return(-1);
	
	display_mutex_id = osMutexCreate(osMutex(display_mutex));
	
  tid_GameState = osThreadCreate (osThread(GameState), NULL);
  if (!tid_GameState) return(-1);
  tid_InteracaoUsuario = osThreadCreate (osThread(InteracaoUsuario), NULL);
  if (!tid_InteracaoUsuario) return(-1);
	tid_Cenario = osThreadCreate (osThread(Cenario), NULL);
  if (!tid_Cenario) return(-1);
	tid_VeiculoJogador = osThreadCreate (osThread(VeiculoJogador), NULL);
  if (!tid_VeiculoJogador) return(-1);
	tid_Obstaculos = osThreadCreate (osThread(Obstaculos), NULL);
  if (!tid_Obstaculos) return(-1);
	tid_PainelControle = osThreadCreate (osThread(PainelControle), NULL);
  if (!tid_PainelControle) return(-1);	
  return(0);
}






int main (void) {

	bool s1_press, s2_press;
	uint8_t  	r, g, b;
	uint32_t color;
	uint16_t y, z, angle=0;
	
	int i,j,n,m =0;

	osKernelInitialize();
	init_all();
	
	init_sidelong_menu();
	
 //  GPIOInit();


	
	for(i=0; i<128; i++){
			for(j=0;j<128;j++){
					if(i<105){
						if(j >20 && j<100)
									GrContextForegroundSet(&sContext, ClrBlue);
						else
									GrContextForegroundSet(&sContext, ClrGreen);
					}
					else{
						GrContextForegroundSet(&sContext, ClrGray);
					}
					GrPixelDraw(&sContext,j,i);
				}
		}
	
	Init_Thread();
	
	if(osKernelStart()!=osOK){
		
	}
			osDelay (osWaitForever);

}
