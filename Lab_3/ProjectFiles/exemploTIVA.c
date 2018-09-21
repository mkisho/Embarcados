
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



tContext sContext;
uint16_t x=50;

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


const unsigned char Bullet_Image[] = { 6,
 102,
 96,
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
const unsigned char Copter_Image[] = {1};
const unsigned char Bridge_Image[] = {1};

	
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





void print_painel(uint16_t  combustivel,uint16_t  pontuacao, uint16_t  vidas){
			char pbufx[10];
			intToString(pontuacao, pbufx, 4, 10, 3);
  		GrStringDraw(&sContext, (char*)pbufx, -1,(sContext.psFont->ui8MaxWidth)*8, (sContext.psFont->ui8Height+2)*12,true);
			intToString(vidas, pbufx, 4, 10, 3);
	  GrStringDraw(&sContext, pbufx, -1,(sContext.psFont->ui8MaxWidth)*2, (sContext.psFont->ui8Height+2)*12,true);
//		GrStringDraw(&sContext, (char*)pbufx, -1,(sContext.psFont->ui8MaxWidth)*8, (sContext.psFont->ui8Height+2)*5,true);
//		GrStringDraw(&sContext, "ms", -1,(sContext.psFont->ui8MaxWidth)*14, (sContext.psFont->ui8Height+2)*5,true);
}





void print(int size, int x, int y, int type){
		int n;
		int m;
		int i=x;
		int j=y;
		int width=0;
		const unsigned char *Image;
		if(type==jogador){
			Image=Player_Image;
			width=14;
			size=21;
		}
		else if(type==tiro){
		}
		else if(type==barco){
			Image=Ship_Image;
			width=24;
			size=23;
		}
		else if(type==aviao){
			Image=Plane_Image;
		}
		else if(type==copter){
			Image=Copter_Image;
		}
		else if(type==fuel){
			//Image=Fuel_Image;
		}
		else if(type==ponte){
			Image=Bridge_Image;
		}
		
		
			for(n=0; n<size; n++){
					for(m=7;m>=0;m--){
							if(Image[n] & (1 << m))
								GrContextForegroundSet(&sContext, ClrYellow);
							else
								GrContextForegroundSet(&sContext, ClrBlue);
					GrPixelDraw(&sContext,j,i);
					j++;
					if(j>=width+y){
							j=y;
							i++;
					}
				}
			}
		
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
		uint16_t velocidade=0;
		

		while(1){
			horiz = joy_read_x();
				vert = joy_read_y();
		
	//			center = joy_read_center();
			if(horiz>0x850){
				x++;
			}
			else if(horiz<0x700){
				x--;
			}
			 if(vert>0x850){
				velocidade++;
			}
			else if(vert<0x700){
				velocidade--;
			}
			
			
		}
	
}
void Cenario (void const *argument) {
}
void VeiculoJogador (void const *argument) {
}
void PainelControle (void const *argument) {
	uint16_t   combustivel=100;
	uint16_t   pontuacao=0;
	uint16_t   vidas=3;
	
	print_painel(combustivel, pontuacao, vidas);
	
}
void Obstaculos (void const *argument) {
	print(21,50,50,2); 
}


void refresh (void const *n) {
	print(21,90,x,0);
}

osTimerDef(timer_0, refresh);


int Init_Thread (void) {
	timer_Id= osTimerCreate(osTimer(timer_0), osTimerPeriodic, (void*)0);
	osTimerStart(timer_Id, 1000);
	
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
	
	
	for(i=0; i<128; i++){
			for(j=0;j<128;j++){
					if(j >20 && j<100)
								GrContextForegroundSet(&sContext, ClrBlue);
					else
								GrContextForegroundSet(&sContext, ClrGreen);
					GrPixelDraw(&sContext,j,i);
				}
		}
	
	Init_Thread();
	
	if(osKernelStart()!=osOK){
		
	}
		


			osDelay (osWaitForever);

}
