
#include "cmsis_os.h"
#include "TM4C129.h"                   
#include <stdbool.h>
#include "grlib/grlib.h"

#include "rgb.h"
#include "cfaf128x128x16.h"
#include "servo.h"
#include "temp.h"
#include "opt.h"
#include "buttons.h"
#include "buzzer.h"
#include "joy.h"
#include "mic.h"
#include "accel.h"
#include "led.h"

#define LED_A      0
#define LED_B      1
#define LED_C      2
#define LED_D      3
#define LED_CLK    7

tContext sContext;

const unsigned char Plane[] = {3,
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


const unsigned char Bullet[] = { 6,
 102,
 96,
};


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







void print(int n, int x, int y){
		int m;
		int i=x;
		int j=y;
	
			for(n=0; n<21; n++){
					for(m=7;m>=0;m--){
//						if(s1_press){
							if(Plane[n] & (1 << m))
								GrContextForegroundSet(&sContext, ClrWhite);
							else
								GrContextForegroundSet(&sContext, ClrBlack);
//						}
					GrPixelDraw(&sContext,j,i);
					j++;
					if(j>=14+y){
							j=y;
							i++;
					}
				}
			}
		i=0;
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
	


int Init_Thread (void) {
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
	uint16_t x, y, z, angle=0;
	int i,j,n,m =0;
	uint16_t horiz =0;
	init_all();
	init_sidelong_menu();
	osKernelInitialize();
	Init_Thread();
	if(osKernelStart()!=0){
		while(1){
			osDelay (1000000);
		}
	}
	x=0;
	y=12;
  while(1){
//		for(i=0; i<128; i++){
//			for(j=0; j<128; j++){
//					GrContextForegroundSet(&sContext, ClrWhite);
//					GrPixelDraw(&sContext,j,i);
//			}
//		}
		
			
		print(21,110,x);


/*  Joystick		*/		
			horiz = joy_read_x();
//			y = joy_read_y();
//			center = joy_read_center();
	  if(horiz>0x800){
			x++;
		}
		else if(horiz<0x700){
			x--;
		}
	
//			GrContextBackgroundSet(&sContext, ClrBlack);
//			GrContextForegroundSet(&sContext, ClrWhite);
//			GrStringDraw(&sContext,(char*)pbufx, -1, (sContext.psFont->ui8MaxWidth)*6,  (sContext.psFont->ui8Height+2)*7, true);
//			GrStringDraw(&sContext,(char*)pbufy, -1,  (sContext.psFont->ui8MaxWidth)*11, (sContext.psFont->ui8Height+2)*7, true);
//			GrStringDraw(&sContext,(char*)pbufz, -1,  (sContext.psFont->ui8MaxWidth)*18, (sContext.psFont->ui8Height+2)*7, true);
			

/*	Botoes 	*/			
//			s1_press = button_read_s1();
//			s2_press = button_read_s2();

//			GrContextBackgroundSet(&sContext, ClrBlack);
//			GrContextForegroundSet(&sContext, ClrWhite);
//			GrStringDraw(&sContext,(char*)pbufx, -1, (sContext.psFont->ui8MaxWidth)*6,  (sContext.psFont->ui8Height+2)*8, true);
//			GrStringDraw(&sContext,(char*)pbufy, -1,  (sContext.psFont->ui8MaxWidth)*11, (sContext.psFont->ui8Height+2)*8, true);
			
	}	
}
