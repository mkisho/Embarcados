
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
uint16_t combustivel=400;
uint16_t   pontuacao=0;
uint16_t   vidas=3;

uint16_t index_obs=0;
uint16_t index_cen=0;

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
const unsigned char Bridge_Image[] = {24,
198,
49,
143,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
241,
140,
99,
24,};
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
//Stage1;
{8,0,0},
{5,150,0},
{8,300,0},
//Stage2;
{3,500,0},
{6,530,0},
{9,600,0},
{5,700,0},
{4,750,0},
{5,800,0},
{4,850,0},
{7,900,0},


//Stage3;
{3,1000,0},
{9,1030,0},

//Stage4;
{3,1500,0},
{7,1530,0},
{10,1600,0},
{7,1800,0},

//Stage5;
{3,2000,0},
{3,2030,0},
//Stage6;
{3,2500,0},
{3,2530,0},
//Stage7;
{3,3000,0},
{3,3030,0},
//Stage8;
{3,3500,0},
{3,3530,0},
//Stage9;
{3,4000,0},
{3,4030,0},

//Stage10;
{3,4500,0},
{3,4530,0},
{3,5000,0},
};

	



struct obstaculo obstacleList[]={
//Stage1;
{2,50,50,1},
{5,50,100,1},
{2,50,130,1},
{2,50,160,1},
{2,50,190,1},
{2,50,300,1},
{2,50,330,1},
{2,50,370,1},
{5,50,390,1},
{2,50,410,1},
{2,50,450,1},
{5,50,475,1},
{6,50,513,1},
//Stage2;

{2,70,560,1},
{4,30,610,1},
{5,60,650,1},
{2,60,720,1},
{2,50,740,1},
{4,55,770,1},
{4,50,830,1},
{2,40,860,1},
{5,50,890,1},
{4,40,910,1},
{2,55,930,1},
{2,40,950,1},

{6,50,1013,1},
//Stage3;
{2,50,1030,1},
{2,50,1060,1},
{4,50,1090,1},
{5,50,1110,1},
{2,50,1140,1},
{2,50,1170,1},
{4,50,1200,1},
{4,50,1230,1},
{5,50,1260,1},
{2,50,1290,1},
{4,50,1310,1},
{5,50,1340,1},
{2,50,1370,1},
{2,50,1400,1},
{5,50,1430,1},
{4,50,1460,1},
{6,50,1513,1},
//Stage4;
{2,50,1560,1},
{4,50,1590,1},
{2,70,1610,1},
{5,30,1940,1},
{4,30,1640,1},
{2,70,1680,1},
{4,30,1720,1},
{5,30,1760,1},
{2,70,1760,1},
{4,30,1810,1},
{2,70,1860,1},
{5,50,1861,1},
{4,50,1900,1},
{2,50,1950,1},
{6,50,2013,1},


//Stage5;
{6,50,2513,1},
//Stage6;
{6,50,3013,1},
//Stage7;
{6,50,3513,1},
//Stage8;
{6,50,4013,1},
//Stage9;
{6,50,4513,1},
//Stage10;
{6,50,5013,1},
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

osMutexDef (buzzer_mutex);    // Declare mutex
osMutexId  (buzzer_mutex_id); // Mutex ID

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









void pontuar(int type){
		if(type== barco){
			pontuacao+=30;
		}
		if(type== copter){
			pontuacao+=50;
		}
		if(type== fuel){
			pontuacao+= 80;
		}
		if(type== ponte){
			pontuacao+=500;
		}
}









int check_collision(uint16_t type, uint16_t x,	uint16_t y, uint16_t j){
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
			height=11;			
		}
		else if(type==fuel){
			width=12;
			height=25;			
		}
		else if(type==ponte){
			width=28;
			height=8;			
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



int check_collision_cenario(uint16_t i){
	if(player.x<(64-cenarioList[i].tamanho*4) || (player.x+14)>(64+cenarioList[i].tamanho*4)){
		return 1;
	}
	return 0;
}

int check_collision_obstacle_cenario(struct obstaculo obs, uint16_t i){
	
	
		if(obs.x<(64-cenarioList[i].tamanho*4) || (obs.x+14)>(64+cenarioList[i].tamanho*4)){
			return 1;
		}

	return 0;
}

int getColor(int y, int type){
	int Color;
	if(type==jogador){
			return ClrYellow;
		}
	else if(type==tiro){
			return ClrYellow;
		
		}
		else if(type==barco){
			if(y>4)
				return ClrGray;
			if(y>2)
				return ClrRed;
			return ClrBlack;
		}
		else if(type==aviao){
			return ClrBlue;
		}
		else if(type==copter){
			if(y>6)
				return ClrGreen;
			if(y>4)
				return ClrDarkBlue;
			if(y>2)
				return ClrGreen;
			return ClrYellow;
		}
		else if(type==fuel){
			if(y>19)
				return ClrWhite;
			if(y>12)
				return ClrRed;
			if(y>6)
				return ClrWhite;
			return ClrRed;
		}
		else if(type==ponte){
			if(y%2==0)
				return ClrBlack;
			return ClrBrown;
		}
		else if(type==display){
				return ClrYellow;
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
			size=28;
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
			j=y;
				for(m=y;m<=width+y;m++){
					GrPixelDraw(&sContext,j,i+1);
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
						if(Image[n] & (1 << m)){
							
							GrContextForegroundSet(&sContext, getColor(i-x,type));
							if(type==6){
								GrContextForegroundSet(&sContext, ClrGray);
							}
						}
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
//		if(i<44 || i>84){
			if((64-size*4)<i && i<(64+size*4))
				GrContextForegroundSet(&sContext, ClrBlue);
			else
				GrContextForegroundSet(&sContext, ClrGreen);
			GrPixelDraw(&sContext,i,y);
//		}
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
			intToString(progresso, pbufx, 5, 10, 1);
   		GrStringDraw(&sContext, pbufx, -1,100, 105,true);
}







void init_all(){
	cfaf128x128x16Init();
	joy_init();
	buzzer_init(); 
	button_init();
	rgb_init();
}


void init_sidelong_menu(){

	GrContextInit(&sContext, &g_sCfaf128x128x16);
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);

}




void GameState (void const *argument) {
		uint16_t bridges_passed=0;
		osEvent evt;
		uint16_t i,j;
		uint16_t n,m;
		while(1){
			evt = osSignalWait (0x01, 100);
			if (evt.status == osEventSignal)  {	
				if(player.alive==1){
							osSignalSet(tid_PainelControle,0x1);
							osSignalSet(tid_VeiculoJogador,0x1);
							osSignalSet(tid_Obstaculos,0x1);
							osSignalSet(tid_InteracaoUsuario,0x1);
							osSignalSet(tid_Cenario,0x1);
				}
				else{
					
						player.alive=1;
						vidas--;
						if(vidas==0){
							while(1);
						}
						n=0;
						m=0;
						progresso=bridges_passed*500;
						player.x=50;
						combustivel=400;
						osDelay(1000);
						while(cenarioList[n].y<progresso){
							n++;
						}
						
						index_cen=n;
						if(index_cen==0){
							index_cen=1;
						}
						index_obs=0;
						while(obstacleList[m].y<(progresso+500)){
							
							if(obstacleList[m].type!=ponte)
								obstacleList[m].alive=1;
							else
								index_obs=m;								
							m++;
						}

						for(i=0; i<128; i++){
							for(j=0;j<128;j++){
									if(i<105){
										if(j >30 && j<95)
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
						
				}
					
//					clean_screen();
				}
				evt = osSignalWait (0x02, 100);
				if (evt.status == osEventSignal)  {
					bridges_passed++;
				}
		 osThreadYield();
		}
}



void InteracaoUsuario (void const *argument) {
		uint16_t horiz =0;
		uint16_t vertical =0;
		uint16_t buzzer_ticks =0;
		osEvent evt;
		bool s1_press;
		bool s2_press;
		buzzer_per_set(100);
 	  buzzer_write(true);
		while(1){
		evt = osSignalWait (0x01, 10000);
		if (evt.status == osEventSignal)  {
			horiz = joy_read_x();
			vertical = joy_read_y();
			if(vertical>0x900){
				velocidade=3;
			}
			else if(vertical<0x700){
				velocidade=1;
			}
			else {
				velocidade=2;
			}
			if(buzzer_ticks<=0){
				if(vertical>0x900){
				buzzer_per_set(10000);
				}
				else {
					buzzer_per_set(100);
				}
			}
			if(horiz>0x900){
				player.x++;
				player.x++;
			}
			else if(horiz<0x700){
				player.x--;
				player.x--;
			}
			
			s1_press=button_read_s1();
			if(s1_press==true){
				
				osSignalSet(tid_VeiculoJogador,0x2);
			}
			s2_press=button_read_s2();
			if(s2_press==true){	
				buzzer_per_set(100);
				buzzer_write(true);
			}
		}
		evt = osSignalWait (0x02, 30);
		if (evt.status == osEventSignal){
				buzzer_per_set(60000);
				buzzer_ticks=5;
		}
		//osDelay(200);	
		if(buzzer_ticks>0)
			buzzer_ticks--;
		
		}
		
	
}




void Cenario (void const *argument) {
				uint16_t i,j,val=1;
				int m,n;
				osEvent evt;
				index_cen=1;
				j=1;
				while(1){
					evt = osSignalWait (0x01, 10000);
						if (evt.status == osEventSignal)  {
							progresso+=velocidade;
							if(progresso-cenarioList[index_cen].y>105){
								index_cen++;
							}
							j=index_cen;
							if(check_collision_cenario(index_cen-1)){
								player.alive=0;
								osSignalSet(tid_InteracaoUsuario,0x02);
							}
							
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
		bullet.y-=2;
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
			print(1,bullet.y-1,bullet.x,1);
			bullet.x=player.x+6;
			bullet.y=85;
			
		}
		
		osThreadYield (); 
		}
	}
	
	
}
void PainelControle (void const *argument) {
	


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
						if(combustivel==0)
								player.alive=0;
						//osSignalSet(tid_GameState,0x1);
					}
				print_painel(combustivel/10, pontuacao, vidas);
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
	int j=0;
	int col;
	while(1){
		evt = osSignalWait (0x01, 10000);
		if (evt.status == osEventSignal)  {
			
			val = osMutexWait (display_mutex_id, 1000);
			switch (val) {
				case osOK:
					if(progresso-obstacleList[index_obs].y>100){
						print(1,progresso-obstacleList[index_obs].y,obstacleList[index_obs].x,obstacleList[index_obs].type);
						index_obs++;
					}
					j=index_obs;
					
					
					while(progresso-obstacleList[j].y>0){
//						obstacleList[j].y=obstacleList[j].y+velocidade;
						if(obstacleList[j].alive==1){
							col = check_collision(obstacleList[j].type,obstacleList[j].x,progresso-obstacleList[j].y, j);	
							print(0,progresso-obstacleList[j].y,obstacleList[j].x,obstacleList[j].type);
							if (col==1){
								print(1,progresso-obstacleList[j].y,obstacleList[j].x,obstacleList[j].type);
								print(1,bullet.y-1,bullet.x,1);
								obstacleList[j].alive=0;
								pontuar(obstacleList[j].type);
								bullet.x=-1;
								bullet.y=-1;
								if(obstacleList[j].type==ponte){
									osSignalSet(tid_GameState,0x2);
								}
								
							}
							else if(col==2){
								if(obstacleList[j].type!=fuel){
									print(1,progresso-obstacleList[j].y,obstacleList[j].x,obstacleList[j].type);
									obstacleList[j].alive=0;
									osSignalSet(tid_InteracaoUsuario,0x02);
									player.alive=0;
//									osSignalSet(tid_GameState,0x01);
								}
								else
									if(combustivel<400)
										combustivel+=10;
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
	osSignalSet(tid_GameState,0x01);
}

osTimerDef(timer_0, refresh);


int Init_Thread (void) {
	timer_Id= osTimerCreate(osTimer(timer_0), osTimerPeriodic, (void*)0);
	osTimerStart(timer_Id, 1200);

//	sid_Thread_Semaphore = osSemaphoreCreate(osSemaphore(Semaforo), 10);
//  if (!sid_Thread_Semaphore) return(-1);
	
	display_mutex_id = osMutexCreate(osMutex(display_mutex));
	buzzer_mutex_id = osMutexCreate(osMutex(buzzer_mutex));
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

	
	
	int i,j =0;

	osKernelInitialize();
	init_all();
	
	init_sidelong_menu();
	
 //  GPIOInit();


	
	for(i=0; i<128; i++){
			for(j=0;j<128;j++){
					if(i<105){
						if(j >30 && j<95)
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
