
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

char flag_Primo=1;
char flag_Ultimo=1;
char flag_Penultimo=1;

unsigned int pen_word;
unsigned int ult_word;
unsigned int ultimo_primo=2;
uint32_t decoded[35];
uint32_t words[35];

signed long diff=0;

unsigned int chave=0x5D77E8D;
int cursor;
char frase[]={0x5a, 0x99, 0x01, 0x00, 0x66, 0x67, 0xfe, 0xff, 0x76, 0x99, 0x01, 0x00, 0x68, 0x67, 0xfe, 0xff, 0x7b, 0x99, 0x01, 0x00, 0x61, 0x67, 0xfe, 0xff, 0x27, 0x99, 0x01, 0x00, 0x26, 0x67, 0xfe, 0xff, 0x27, 0x99, 0x01, 0x00, 0x4c, 0x67, 0xfe, 0xff, 0x68, 0x99, 0x01, 0x00, 0x67, 0x67, 0xfe, 0xff, 0x7b, 0x99, 0x01, 0x00, 0x5a, 0x67, 0xfe, 0xff, 0x75, 0x99, 0x01, 0x00, 0x5a, 0x67, 0xfe, 0xff, 0x27, 0x99, 0x01, 0x00, 0x5f, 0x67, 0xfe, 0xff, 0x6c, 0x99, 0x01, 0x00, 0x5a, 0x67, 0xfe, 0xff, 0x7b, 0x99, 0x01, 0x00, 0x27, 0x67, 0xfe, 0xff, 0x27, 0x99, 0x01, 0x00, 0x4b, 0x67, 0xfe, 0xff, 0x76, 0x99, 0x01, 0x00, 0x5b, 0x67, 0xfe, 0xff, 0x27, 0x99, 0x01, 0x00, 0x4d, 0x67, 0xfe, 0xff, 0x6f, 0x99, 0x01, 0x00, 0x68, 0x67, 0xfe, 0xff, 0x74, 0x99, 0x01, 0x00, 0x5a, 0x67, 0xfe, 0xff, 0x7a, 0x99, 0x01, 0x00, 0x8a, 0x65, 0x02, 0x00, 0x8e, 0xbf, 0xfe, 0xff};
//char frase[]={0x88, 0x20, 0xae, 0x8b, 0x33, 0xe0, 0x51, 0x74, 0xa8, 0x20, 0xae, 0x8b, 0x2f, 0xe0, 0x51, 0x74, 0xbc, 0x20, 0xae, 0x8b, 0xdd, 0xdf, 0x51, 0x74, 0x85, 0x20, 0xae, 0x8b, 0x2f, 0xe0, 0x51, 0x74, 0xa8, 0x20, 0xae, 0x8b, 0x1e, 0xe0, 0x51, 0x74, 0xb7, 0x20, 0xae, 0x8b, 0x25, 0xe0, 0x51, 0x74, 0x63, 0x20, 0xae, 0x8b, 0x16, 0xe0, 0x51, 0x74, 0xb2, 0x20, 0xae, 0x8b, 0x32, 0xe0, 0x51, 0x74, 0x63, 0x20, 0xae, 0x8b, 0x11, 0xe0, 0x51, 0x74, 0xa4, 0x20, 0xae, 0x8b, 0x28, 0xe0, 0x51, 0x74, 0xa8, 0x20, 0xae, 0x8b, 0xdd, 0xdf, 0x51, 0x74, 0x70, 0x20, 0xae, 0x8b, 0xdd, 0xdf, 0x51, 0x74, 0x97, 0x20, 0xae, 0x8b, 0x25, 0xe0, 0x51, 0x74, 0xa8, 0x20, 0xae, 0x8b, 0xdd, 0xdf, 0x51, 0x74, 0x93, 0x20, 0xae, 0x8b, 0x2c, 0xe0, 0x51, 0x74, 0xaf, 0x20, 0xae, 0x8b, 0x26, 0xe0, 0x51, 0x74, 0xa6, 0x20, 0xae, 0x8b, 0x64, 0x30, 0x85, 0xd1, 0xbe, 0xdf, 0x51, 0x74};
//char frase[]={0x07, 0x6a, 0x00, 0x00, 0xb5, 0x96, 0xff, 0xff, 0x18, 0x6a, 0x00, 0x00, 0x6d, 0x96, 0xff, 0xff, 0xf7, 0x69, 0x00, 0x00, 0xbc, 0x96, 0xff, 0xff, 0x22, 0x6a, 0x00, 0x00, 0xbf, 0x96, 0xff, 0xff, 0x26, 0x6a, 0x00, 0x00, 0x6d, 0x96, 0xff, 0xff, 0xe0, 0x69, 0x00, 0x00, 0x6d, 0x96, 0xff, 0xff, 0xff, 0x69, 0x00, 0x00, 0xb6, 0x96, 0xff, 0xff, 0x1a, 0x6a, 0x00, 0x00, 0xb5, 0x96, 0xff, 0xff, 0x27, 0x6a, 0x00, 0x00, 0x6d, 0x96, 0xff, 0xff, 0x00, 0x6a, 0x00, 0x00, 0xc6, 0x96, 0xff, 0xff, 0xd3, 0x69, 0x00, 0x00, 0x93, 0x96, 0xff, 0xff, 0x1c, 0x6a, 0x00, 0x00, 0xbf, 0x96, 0xff, 0xff, 0x18, 0x6a, 0x00, 0x00, 0x6d, 0x96, 0xff, 0xff, 0xd3, 0x69, 0x00, 0x00, 0x6d, 0x96, 0xff, 0xff, 0xd3, 0x69, 0x00, 0x00, 0x6d, 0x96, 0xff, 0xff, 0xd3, 0x69, 0x00, 0x00, 0x6d, 0x96, 0xff, 0xff, 0xd3, 0x69, 0x00, 0x00, 0x8c, 0x9e, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
//char frase[]={0xc5, 0x97, 0xd7, 0x17, 0xf7, 0x68, 0x28, 0xe8, 0xd6, 0x97, 0xd7, 0x17, 0xaf, 0x68, 0x28, 0xe8, 0xb6, 0x97, 0xd7, 0x17, 0xf0, 0x68, 0x28, 0xe8, 0xd8, 0x97, 0xd7, 0x17, 0xfb, 0x68, 0x28, 0xe8, 0xd6, 0x97, 0xd7, 0x17, 0x02, 0x69, 0x28, 0xe8, 0x91, 0x97, 0xd7, 0x17, 0xbc, 0x68, 0x28, 0xe8, 0x91, 0x97, 0xd7, 0x17, 0xd7, 0x68, 0x28, 0xe8, 0xe0, 0x97, 0xd7, 0x17, 0x03, 0x69, 0x28, 0xe8, 0xd6, 0x97, 0xd7, 0x17, 0xfb, 0x68, 0x28, 0xe8, 0x91, 0x97, 0xd7, 0x17, 0xd2, 0x68, 0x28, 0xe8, 0xd2, 0x97, 0xd7, 0x17, 0xfb, 0x68, 0x28, 0xe8, 0xda, 0x97, 0xd7, 0x17, 0xf5, 0x68, 0x28, 0xe8, 0xe0, 0x97, 0xd7, 0x17, 0x01, 0x69, 0x28, 0xe8, 0xdf, 0x97, 0xd7, 0x17, 0xf8, 0x68, 0x28, 0xe8, 0xd2, 0x97, 0xd7, 0x17, 0xaf, 0x68, 0x28, 0xe8, 0x91, 0x97, 0xd7, 0x17, 0xaf, 0x68, 0x28, 0xe8, 0x91, 0x97, 0xd7, 0x17, 0x29, 0x63, 0xc3, 0x23, 0x8f, 0x68, 0x28, 0xe8};
//char frase[]={0xcd, 0x4f, 0x46, 0xad, 0xe7, 0xb0, 0xb9, 0x52, 0x00, 0x50, 0x46, 0xad, 0xd8, 0xb0, 0xb9, 0x52, 0xf0, 0x4f, 0x46, 0xad, 0x95, 0xb0, 0xb9, 0x52, 0xde, 0x4f, 0x46, 0xad, 0xe5, 0xb0, 0xb9, 0x52, 0xfd, 0x4f, 0x46, 0xad, 0xde, 0xb0, 0xb9, 0x52, 0xf9, 0x4f, 0x46, 0xad, 0xdc, 0xb0, 0xb9, 0x52, 0xfe, 0x4f, 0x46, 0xad, 0xe9, 0xb0, 0xb9, 0x52, 0xf0, 0x4f, 0x46, 0xad, 0xda, 0xb0, 0xb9, 0x52, 0xf9, 0x4f, 0x46, 0xad, 0x95, 0xb0, 0xb9, 0x52, 0xb8, 0x4f, 0x46, 0xad, 0x95, 0xb0, 0xb9, 0x52, 0xcd, 0x4f, 0x46, 0xad, 0xe4, 0xb0, 0xb9, 0x52, 0xfd, 0x4f, 0x46, 0xad, 0xe3, 0xb0, 0xb9, 0x52, 0xab, 0x4f, 0x46, 0xad, 0xe9, 0xb0, 0xb9, 0x52, 0xfa, 0x4f, 0x46, 0xad, 0x95, 0xb0, 0xb9, 0x52, 0xdd, 0x4f, 0x46, 0xad, 0xea, 0xb0, 0xb9, 0x52, 0xf9, 0x4f, 0x46, 0xad, 0x95, 0xb0, 0xb9, 0x52, 0xab, 0x4f, 0x46, 0xad, 0x50, 0x77, 0xe9, 0x03, 0x75, 0xb0, 0xb9, 0x52};

//char* Frase[]={"5a","99","01","00", "66", "67","fe","ff","76","99","01","00","68","67","fe","ff","7b","99","01","00","61","67","fe","ff","27","99","01","00","26","67","fe","ff","27","99","01","00","4c","67","fe","ff","68","99","01","00","67","67","fe","ff","7b","99","01","00","5a","67","fe","ff","75","99","01","00","5a","67","fe","ff","27","99","01","00","5f","67","fe","ff","6c","99","01","00","5a","67","fe","ff","7b","99","01","00","27","67","fe","ff","27","99","01","00","4b","67","fe","ff","76","99","01","00","5b","67","fe","ff","27","99","01","00","4d","67","fe","ff","6f","99","01","00","68","67","fe","ff","74","99","01","00","5a","67","fe","ff","7a","99","01","00","8a","65","02","00","8e","bf","fe","ff"};
//char* Frase[]={"88","20","ae","8b","33","e0","51","74","a8","20","ae","8b","2f","e0","51","74","bc","20","ae","8b","dd","df","51","74","85","20","ae","8b","2f","e0","51","74","a8","20","ae","8b","1e","e0","51","74","b7","20","ae","8b","25","e0","51","74","63","20","ae","8b","16","e0","51","74","b2","20","ae","8b","32","e0","51","74","63","20","ae","8b","11","e0","51","74","a4","20","ae","8b","28","e0","51","74","a8","20","ae","8b","dd","df","51","74","70","20","ae","8b","dd","df","51","74","97","20","ae","8b","25","e0","51","74","a8","20","ae","8b","dd","df","51","74","93","20","ae","8b","2c","e0","51","74","af","20","ae","8b","26","e0","51","74","a6","20","ae","8b","64","30","85","d1","be","df","51","74"};
//char* Frase[]={"07","6a","00","00","b5","96","ff","ff","18","6a","00","00","6d","96","ff","ff","f7","69","00","00","bc","96","ff","ff","22","6a","00","00","bf","96","ff","ff","26","6a","00","00","6d","96","ff","ff","e0","69","00","00","6d","96","ff","ff","ff","69","00","00","b6","96","ff","ff","1a","6a","00","00","b5","96","ff","ff","27","6a","00","00","6d","96","ff","ff","00","6a","00","00","c6","96","ff","ff","d3","69","00","00","93","96","ff","ff","1c","6a","00","00","bf","96","ff","ff","18","6a","00","00","6d","96","ff","ff","d3","69","00","00","6d","96","ff","ff","d3","69","00","00","6d","96","ff","ff","d3","69","00","00","6d","96","ff","ff","d3","69","00","00","8c","9e","00","00","10","00","00","00"};
//char* Frase[]={"c5","97","d7","17","f7","68","28","e8","d6","97","d7","17","af","68","28","e8","b6","97","d7","17","f0","68","28","e8","d8","97","d7","17","fb","68","28","e8","d6","97","d7","17","02","69","28","e8","91","97","d7","17","bc","68","28","e8","91","97","d7","17","d7","68","28","e8","e0","97","d7","17","03","69","28","e8","d6","97","d7","17","fb","68","28","e8","91","97","d7","17","d2","68","28","e8","d2","97","d7","17","fb","68","28","e8","da","97","d7","17","f5","68","28","e8","e0","97","d7","17","01","69","28","e8","df","97","d7","17","f8","68","28","e8","d2","97","d7","17","af","68","28","e8","91","97","d7","17","af","68","28","e8","91","97","d7","17","29","63","c3","23","8f","68","28","e8"};
//char* Frase[]={"cd","4f","46","ad","e7","b0","b9","52","00","50","46","ad","d8","b0","b9","52","f0","4f","46","ad","95","b0","b9","52","de","4f","46","ad","e5","b0","b9","52","fd","4f","46","ad","de","b0","b9","52","f9","4f","46","ad","dc","b0","b9","52","fe","4f","46","ad","e9","b0","b9","52","f0","4f","46","ad","da","b0","b9","52","f9","4f","46","ad","95","b0","b9","52","b8","4f","46","ad","95","b0","b9","52","cd","4f","46","ad","e4","b0","b9","52","fd","4f","46","ad","e3","b0","b9","52","ab","4f","46","ad","e9","b0","b9","52","fa","4f","46","ad","95","b0","b9","52","dd","4f","46","ad","ea","b0","b9","52","f9","4f","46","ad","95","b0","b9","52","ab","4f","46","ad","50","77","e9","03","75","b0","b9","52"};

	
int penultimo,ultimo;
	
int ticks_factor = 10000;

	

	
void byte_to_word(){
		int count;
		for (count=0; count<35; count++){
			words[count]=frase[count*4]*16777216+frase[count*4+1]*65536+frase[count*4+2]*256+frase[count*4+3];
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
 
void TestePen (void const *argument);               // thread function
osThreadId tid_TestePen;                            // thread id
osThreadDef (TestePen, osPriorityNormal, 1, 0);     // thread object
 
 void TesteUlt (void const *argument);               // thread function
osThreadId tid_TesteUlt;                            // thread id
osThreadDef (TesteUlt, osPriorityNormal, 1, 0);     // thread object
 
 void TestePri (void const *argument);               // thread function
osThreadId tid_TestePri;                            // thread id
osThreadDef (TestePri, osPriorityNormal, 1, 0);     // thread object
 
 void Escrita (void const *argument);               // thread function
osThreadId tid_Escrita;                            // thread id
osThreadDef (Escrita, osPriorityNormal, 1, 0);     // thread object
 

void GeraChave (void const *argument) {
  uint32_t time;
	//chave=words[33]/2-0x10000;
  while (1) {
		
		if(flagA){
			chave+=1;
			flagA=0;
			flagB=1;
		}
		osThreadYield();
    //osDelay (1);                          
  }
}

void Decoder (void const *argument) {
    uint32_t time;
		while (1) {
		
    if(flagB){
			for(cursor=0;cursor<35;cursor++){
				 if(words[cursor]%2==0)
					 decoded[cursor]=words[cursor]-chave;
					else decoded[cursor]=words[cursor]+chave;
				}
			flagB=0;
			flagC=1;
		}
				osThreadYield();
    //osDelay (1);                           
  }
}

void TestePen (void const *argument) {

  while (1) {
    if(flagC){
			penultimo=decoded[33];
			if(penultimo*2==chave||penultimo*2+1==chave)
				flag_Penultimo=1;
			else {
				diff= (penultimo*2)-chave;
				flag_Penultimo=0;
			
			}	
			flagC=0;
			flagD=1;
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
			if(sqrt(ultimo*ultimo_primo)==chave){
				flag_Ultimo=1;
			}
			else {
				flag_Ultimo=0;
			}
		  flagD=0;
			flagE=1;
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
			flagE=0;
			flagF=1;
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
			flagF=0;
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
	tid_TestePen = osThreadCreate (osThread(TestePen), NULL);
  if (!tid_TestePen) return(-1);
	tid_TesteUlt = osThreadCreate (osThread(TesteUlt), NULL);
  if (!tid_TesteUlt) return(-1);
	tid_TestePri = osThreadCreate (osThread(TestePri), NULL);
  if (!tid_TestePri) return(-1);
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
