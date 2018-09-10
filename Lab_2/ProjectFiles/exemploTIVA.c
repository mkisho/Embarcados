//Chave teste 1: 0x7735AA29
//Chave teste 2: 0x00003401

#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include "grlib/grlib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*----------------------------------------------------------------------------
 * include libraries from drivers
 *----------------------------------------------------------------------------*/

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

//To print on the screen
tContext sContext;

int flagA =1;
int flagB =0;
int flagC =0;
int flagD =0;
int flagE =0;
int flagF =0;
int flagG =0;
int flagH =0;

int flag1 =0;
int flag2 =0;
int flag3 =0;
int flag4 =0;
int flag5 =0;

int flag_Primo=1;
int flag_Ultimo=1;
int flag_Penultimo=1;
int flag_AntePenultimo=1;

unsigned int pen_word;
unsigned int ult_word;
unsigned int ultimo_primo=2;
unsigned int primo=2;

uint32_t decoded[33];
uint32_t words[33];

signed long diff=0;

unsigned int chave=0x5D77E8D;
int cursor;
//char frase[]={0x1a, 0x56, 0xca, 0x88, 0x9e, 0xaa, 0x35, 0x77, 0x49, 0x56, 0xca, 0x88, 0x9d, 0xaa, 0x35, 0x77, 0x40, 0x56, 0xca, 0x88, 0x97, 0xaa, 0x35, 0x77, 0x3b, 0x56, 0xca, 0x88, 0x98, 0xaa, 0x35, 0x77, 0xf7, 0x55, 0xca, 0x88, 0x8a, 0xaa, 0x35, 0x77, 0xf7, 0x55, 0xca, 0x88, 0x7f, 0xaa, 0x35, 0x77, 0x40, 0x56, 0xca, 0x88, 0x8d, 0xaa, 0x35, 0x77, 0x38, 0x56, 0xca, 0x88, 0x49, 0xaa, 0x35, 0x77, 0x18, 0x56, 0xca, 0x88, 0x8d, 0xaa, 0x35, 0x77, 0x46, 0x56, 0xca, 0x88, 0x92, 0xaa, 0x35, 0x77, 0x3b, 0x56, 0xca, 0x88, 0x8a, 0xaa, 0x35, 0x77, 0x3b, 0x56, 0xca, 0x88, 0x98, 0xaa, 0x35, 0x77, 0xf7, 0x55, 0xca, 0x88, 0x49, 0xaa, 0x35, 0x77, 0x08, 0x56, 0xca, 0x88, 0x62, 0xaa, 0x35, 0x77, 0x0f, 0x56, 0xca, 0x88, 0x5f, 0xaa, 0x35, 0x77, 0x0f, 0x62, 0xc2, 0x98, 0x7b, 0xfe, 0xa0, 0x65, 0xe5, 0x55, 0xca, 0x88,
//};

char frase[]={0x43, 0xcc, 0xff, 0xff, 0x66, 0x34, 0x00, 0x00, 0x1f, 0xcc, 0xff, 0xff, 0x57, 0x34, 0x00, 0x00, 0x6e, 0xcc, 0xff, 0xff, 0x6d, 0x34, 0x00, 0x00, 0x73, 0xcc, 0xff, 0xff, 0x62, 0x34, 0x00, 0x00, 0x1f, 0xcc, 0xff, 0xff, 0x51, 0x34, 0x00, 0x00, 0x60, 0xcc, 0xff, 0xff, 0x73, 0x34, 0x00, 0x00, 0x60, 0xcc, 0xff, 0xff, 0x21, 0x34, 0x00, 0x00, 0x6e, 0xcc, 0xff, 0xff, 0x21, 0x34, 0x00, 0x00, 0x45, 0xcc, 0xff, 0xff, 0x76, 0x34, 0x00, 0x00, 0x73, 0xcc, 0xff, 0xff, 0x76, 0x34, 0x00, 0x00, 0x71, 0xcc, 0xff, 0xff, 0x70, 0x34, 0x00, 0x00, 0x1f, 0xcc, 0xff, 0xff, 0x21, 0x34, 0x00, 0x00, 0x1f, 0xcc, 0xff, 0xff, 0x21, 0x34, 0x00, 0x00, 0x30, 0xcc, 0xff, 0xff, 0x3a, 0x34, 0x00, 0x00, 0x37, 0xcc, 0xff, 0xff, 0x36, 0x34, 0x00, 0x00, 0x1f, 0x81, 0x00, 0x00, 0x03, 0x9c, 0x00, 0x00, 0xff, 0xcb, 0xff, 0xff,
};

unsigned int antepenultimo, penultimo, ultimo, ult;

	
void byte_to_word(){
		int count;
		for (count=0; count<33; count++){
			words[count]=frase[count*4+3]*16777216+frase[count*4+2]*65536+frase[count*4+1]*256+frase[count*4];
		}
}
void word_to_byte(){
		int count;
		for (count=0; count<33; count++){
			frase[count*4]=decoded[count]/16777216;
			frase[count*4+1]=(decoded[count]-decoded[count]/16777216)/65536;
			frase[count*4+2]=(decoded[count]-(decoded[count]-decoded[count]/16777216)/65536)/256;
			frase[count*4+3]=decoded[count]-(decoded[count]-(decoded[count]-decoded[count]/16777216)/65536)/256;			
		}
}	
		
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
void GeraChave (void const *argument);               // thread function
osThreadId tid_GeraChave;                            // thread id
osThreadDef (GeraChave, osPriorityNormal, 1, 0);     // thread object
 
void Decoder (void const *argument);               // thread function
osThreadId tid_Decoder;                            // thread id
osThreadDef (Decoder, osPriorityNormal, 1, 0);     // thread object

void TesteAntePen (void const *argument);               // thread function
osThreadId tid_TesteAntePen;                            // thread id
osThreadDef (TesteAntePen, osPriorityNormal, 1, 0);     // thread object
 
void TestePen (void const *argument);               // thread function
osThreadId tid_TestePen;                            // thread id
osThreadDef (TestePen, osPriorityNormal, 1, 0);     // thread object
 
void TesteUlt (void const *argument);               // thread function
osThreadId tid_TesteUlt;                            // thread id
osThreadDef (TesteUlt, osPriorityNormal, 1, 0);     // thread object
 
void TestePri (void const *argument);               // thread function
osThreadId tid_TestePri;                            // thread id
osThreadDef (TestePri, osPriorityNormal, 1, 0);     // thread object

void TesteFib (void const *argument);               // thread function
osThreadId tid_TesteFib;                            // thread id
osThreadDef (TesteFib, osPriorityNormal, 1, 0);     // thread object
 
void Escrita (void const *argument);               // thread function
osThreadId tid_Escrita;                            // thread id
osThreadDef (Escrita, osPriorityNormal, 1, 0);     // thread object
 
void GeraChave (void const *argument) {
  uint32_t time;
	int i, j;
	chave=words[1]-127;
  while (1) {
		if(flagA){
			chave+=1;
			for (i=0;i<10000;i++){
				for (j=0;j<10;j++){
				}
			}		
			flagA=0;
			flagB=1;
		}
    osDelay (100);                          
  }
}

void Decoder (void const *argument) {
		int i, j;
		uint32_t time;
		while (1) {
			if(flagB){
				for(cursor=0;cursor<33;cursor++){
					if(cursor%2)
						decoded[cursor]=words[cursor]-chave;
					else 
						decoded[cursor]=words[cursor]+chave;
				}
			for (i=0;i<10000;i++){
				for (j=0;j<10;j++){
				}
			}
			flagB=0;				
			flag1=1;
			flag2=1;
			flag3=1;
			flag4=1;
			flag5=1;
			flagH=1;
		}
    osDelay (100);                           
  }
}

void TesteAntePen (void const *argument) {
		int i,j;
		while (1) {
			if(flag1){
				int a, b, auxiliar; 
				a = 0;
				b = 1;
				antepenultimo = decoded[30];
				while(b < antepenultimo){
					auxiliar = a + b;
					a = b;
					b = auxiliar;
				}	
				if(antepenultimo==b)
					flag_AntePenultimo=1;
				else
					flag_AntePenultimo=0;
				
				for (i=0;i<10000;i++){
					for (j=0;j<10;j++){
					}
				}
				flag1=0;
			}
			osDelay (100);
  }
}

void TestePen (void const *argument) {
	int i, j, auxiliar;
  while(1){
    if(flag2){
			penultimo=decoded[31];
			auxiliar = chave*2;
			if(penultimo==auxiliar)
				flag_Penultimo=1;
			else {
				flag_Penultimo=0;
			}	
			for (i=0;i<10000;i++){
				for (j=0;j<10;j++){
				}
			}	
			flag2=0;
		}
		osDelay (100);                           
  }
}

char primer (int n){
		int i =2;
		int max = sqrt(n);
		for (i =2; i<=max;i++){
     if (!(n%i))
       return 0;
		}
		return 1;
}	


void TesteUlt (void const *argument) {
	int i, j;
  while (1) {
    if(flag3){
			ultimo=decoded[32];
			ult=(chave+primo)/antepenultimo;
			if(ult == ultimo)
				flag_Ultimo=1;
			else
				flag_Ultimo=0;
			for (i=0;i<10000;i++){
				for (j=0;j<10;j++){
				}
			}	
			flag3=0;	
		}
    osDelay (100);                            
  }
}

void TestePri (void const *argument) {
	uint32_t time;
	int i, j;
	int max;
  while (1) {
		if(flag4){
			flag_Primo=primer(chave);
			if(flag_Primo){
				ultimo_primo=primo;
				primo=chave;
			}
			for (i=0;i<10000;i++){
				for (j=0;j<10;j++){
				}
			}
			flag4=0;
		}
		osDelay (100);                             
  }
}

void TesteFib (void const *argument) {
	int i, j;
	uint32_t time;
	int max;
  while(1) {
		if(flag5){
			for (i=0;i<10000;i++){
				for (j=0;j<10;j++){
				}
			}
		flag5=0;
		}
    osDelay (100);                             
  }
}

void Escrita (void const *argument) {
		uint32_t time;
		int i, j;
		int x, y;
    while (1){
			if(flagH){
				if(!flag2 && !flag3 && !flag4 && !flag5){
					for (x=0; x<33; x++){
						word_to_byte();
//						GrStringDraw(&sContext, frase, 30, 0, 0, true);
//						osDelay(1000);
					}
					for (i=0;i<10000;i++){
						for (j=0;j<10;j++){
						}
					}
					flagH=0;
					flagA=1;
					if( flag_Primo==1 && flag_Ultimo==1 && flag_Penultimo==1 && flag_AntePenultimo==1){
						osDelay(1);
//					GrStringDraw(&sContext,"FINISH", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
					}
				}
			}
			osDelay (100);                            
		}
}
void init_all(){
	cfaf128x128x16Init();
	rgb_init();
}

void init_sidelong_menu(){
	uint8_t i;
	GrContextInit(&sContext, &g_sCfaf128x128x16);
	
	GrFlush(&sContext);
	GrContextFontSet(&sContext, g_psFontFixed6x8);
	
	GrContextForegroundSet(&sContext, ClrWhite);
	GrContextBackgroundSet(&sContext, ClrBlack);
	
	//Escreve menu lateral:
	GrStringDraw(&sContext,"---------------------", -1, 0, (sContext.psFont->ui8Height+2)*1, true);
}

int Init_Thread (void) {
  tid_GeraChave = osThreadCreate (osThread(GeraChave), NULL);
  if (!tid_GeraChave) return(-1);
  tid_Decoder = osThreadCreate (osThread(Decoder), NULL);
  if (!tid_Decoder) return(-1);
	tid_TesteAntePen = osThreadCreate (osThread(TesteAntePen), NULL);
  if (!tid_TesteAntePen) return(-1);
	tid_TestePen = osThreadCreate (osThread(TestePen), NULL);
  if (!tid_TestePen) return(-1);
	tid_TesteUlt = osThreadCreate (osThread(TesteUlt), NULL);
  if (!tid_TesteUlt) return(-1);
	tid_TestePri = osThreadCreate (osThread(TestePri), NULL);
  if (!tid_TestePri) return(-1);
	tid_TesteFib = osThreadCreate (osThread(TesteFib), NULL);
  if (!tid_TesteFib) return(-1);
	tid_Escrita = osThreadCreate (osThread(Escrita), NULL);
  if (!tid_Escrita) return(-1);	
  return(0);
}
 
/*----------------------------------------------------------------------------
 *      Main
 *---------------------------------------------------------------------------*/
int main (void) {

//	FILE *f;
//	init_all();
//	init_sidelong_menu();
	byte_to_word();
	osKernelInitialize();
	//TODO Inicializar primo
	Init_Thread();

	if(osKernelStart()!=0){
		while(1){
			osDelay (1000000);
		}
	}
}
