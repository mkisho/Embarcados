
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


char flagA =1;
char flagB =0;
char flagC =0;
char flagD =0;
char flagE =0;
char flagF =0;
char flagG =0;
char flagH =0;

char flag_Primo=1;
char flag_Ultimo=1;
char flag_Penultimo=1;
char flag_AntePenultimo=1;

unsigned int pen_word;
unsigned int ult_word;
unsigned int ultimo_primo=2;
uint32_t decoded[35];
uint32_t words[35];

signed long diff=0;

unsigned int chave=0x5D77E8D;
int cursor;
char frase[]={0x1a, 0x56, 0xca, 0x88,  0x9e, 0xaa, 0x35, 0x77,  0x49, 0x56, 0xca, 0x88,  0x9d, 0xaa, 0x35, 0x77,  0x40, 0x56, 0xca, 0x88,  0x97, 0xaa, 0x35, 0x77,  0x3b, 0x56, 0xca, 0x88,  0x98, 0xaa, 0x35, 0x77,  0xf7, 0x55, 0xca, 0x88,  0x8a, 0xaa, 0x35, 0x77,  0xf7, 0x55, 0xca, 0x88,  0x7f, 0xaa, 0x35, 0x77,  0x40, 0x56, 0xca, 0x88,  0x8d, 0xaa, 0x35, 0x77,  0x38, 0x56, 0xca, 0x88,  0x49, 0xaa, 0x35, 0x77,  0x18, 0x56, 0xca, 0x88,  0x8d, 0xaa, 0x35, 0x77,  0x46, 0x56, 0xca, 0x88,  0x92, 0xaa, 0x35, 0x77,  0x3b, 0x56, 0xca, 0x88,  0x8a, 0xaa, 0x35, 0x77,  0x3b, 0x56, 0xca, 0x88,  0x98, 0xaa, 0x35, 0x77,  0xf7, 0x55, 0xca, 0x88,  0x5a, 0xaa, 0x35, 0x77,  0x10, 0x56, 0xca, 0x88,  0x61, 0xaa, 0x35, 0x77,  0x0d, 0x56, 0xca, 0x88,  0x29, 0xaa, 0x35, 0x77,  0xd7, 0x55, 0xca, 0x88,  0x29, 0xaa, 0x35, 0x77,  0xd7, 0x55, 0xca, 0x88,  0x49, 0x53, 0xc2, 0x97,  0x7b, 0xfe, 0xa0, 0x65, 
};



	
int antepenultimo, penultimo,ultimo;
	
int ticks_factor = 10000;

	

	
void byte_to_word(){
		int count;
		for (count=0; count<35; count++){
			words[count]=frase[count*4+3]*16777216+frase[count*4+2]*65536+frase[count*4+1]*256+frase[count*4];
		}
}
void word_to_byte(){
		int count;
		for (count=0; count<35; count++){
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
//	if(chave==0){
				chave=words[1]-127;
//	}
  while (1) {
		if(flagA){
			chave+=1;
			flagA=0;
			flagB=1;
		}
//		osThreadYield();
    osDelay (10);                          
  }
}

void Decoder (void const *argument) {
    uint32_t time;
		while (1) {
		
    if(flagB){
			for(cursor=0;cursor<35;cursor++){
				 if(words[cursor]%2==0)
					 decoded[cursor]=words[cursor]+chave;
					else decoded[cursor]=words[cursor]-chave;
				}
			flagB=0;
			flagC=1;
		}
				osThreadYield();
    //osDelay (1);                           
  }
}




void TesteAntePen (void const *argument) {

  while (1) {
    if(flagC){
			int a, b, auxiliar, i; 
			a = 0;
			b = 1;
			antepenultimo = decoded[32];
			for(i = 0; i < antepenultimo; i++){
				auxiliar = a + b;
				a = b;
				b = auxiliar;
				if(antepenultimo==chave)
					flag_Penultimo=1;
				else {
					flag_Penultimo=0;
				}	
			}
			flagC=0;
			flagD=1;
		}
		osThreadYield();
    //osDelay (1);                           
  }
}


void TestePen (void const *argument) {

  while (1) {
    if(flagC){
			penultimo=decoded[33];
			if(penultimo/2==chave)
				flag_Penultimo=1;
			else {
				flag_Penultimo=0;
			}	
			flagD=0;
			flagE=1;
		}
		osThreadYield();
    //osDelay (1);                           
  }
}



char primer (int n){
		int i =2;
		int max = sqrt(n);
		for (i =2; i<=max;i++){
     if (n%i == 0)
       return 0;
		}
		return 1;
}	



void TesteUlt (void const *argument) {
	unsigned int ult;
	char cond=0;
	ult=chave;
  while (1) {
    if(flagD){
			ultimo=decoded[34];
//			while(!cond){
//				cond=0;
//				ult--;
//				cond=primer(ult);
//			}
//			ultimo_primo=ult;
//			if(sqrt(ultimo*ultimo_primo)==chave){
//				flag_Ultimo=1;
//			}
//			else {
//				flag_Ultimo=0;
//			}
		  flagE=0;
			flagF=1;
		}
				osThreadYield();
   // osDelay (1);                            
  }
}




void TestePri (void const *argument) {
	uint32_t time;
	int i;
	int max;
  while (1) {
   if(flagE){
			flag_Primo=primer(chave);
			if(flag_Primo){
				ultimo_primo=chave;
			}
			flagF=0;
			flagG=1;
		}
    //osDelay (1);                             
		osThreadYield();
  }
}

void TesteFib (void const *argument) {
	//TODO implementar Fib
	uint32_t time;
	int i;
	int max;
  while (1) {
   if(flagE){
			flag_Primo=primer(chave);
			if(flag_Primo){
				ultimo_primo=chave;
			}
			flagG=0;
			flagH=1;
		}
    //osDelay (1);                             
		osThreadYield();
  }
}


void Escrita (void const *argument) {
    uint32_t time;
		int x;
		int y;
		
    while (1) {
    if(flagF){
//			for (x=0; x<7; x++){
//				for(y=0;y<20;y++){
			word_to_byte();
//			GrStringDrawCentered(&sContext, frase, 20, 10, (chave%20)*5, true);
			GrStringDraw(&sContext,"FINISH", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
//				}
//			}
			flagH=0;
			flagA=1;
			if( flag_Primo==1 && flag_Ultimo==1 && flag_Penultimo==1){
			//	osDelay(10000);
								GrStringDraw(&sContext,"FINISH", -1, 0, (sContext.psFont->ui8Height+2)*8, true);
			}
		}
		osThreadYield();
    //osDelay (1);                            
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

	FILE *f;
	init_all();
	init_sidelong_menu();
	byte_to_word();
	osKernelInitialize();
	Init_Thread();

	osKernelStart();

	
  while(1){
		osDelay (1000000);
	}	
}
