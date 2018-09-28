
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

//#define P1_2_HIGH() (LPC_GPIO1->DATA |= (0x1<<2))
//#define P1_2_LOW()  (LPC_GPIO1->DATA &= ~(0x1<<2))

tContext sContext;
uint16_t x=50;
uint16_t velocidade=2;



struct obstaculo{
	uint16_t type;
	uint16_t x;
	uint16_t y;
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

struct obstaculo obstacleList[]={{2,50,0},{2,50,50}};
	
	
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


static uint32_t notes[] = {
        2272, // A - 440 Hz
        2024, // B - 494 Hz
        3816, // C - 262 Hz
        3401, // D - 294 Hz
        3030, // E - 330 Hz
        2865, // F - 349 Hz
        2551, // G - 392 Hz
        1136, // a - 880 Hz
        1012, // b - 988 Hz
        1912, // c - 523 Hz
        1703, // d - 587 Hz
        1517, // e - 659 Hz
        1432, // f - 698 Hz
        1275, // g - 784 Hz
};


static const char *songs[] = {
   "E2,E2,E4,E2,E2,E4,E2,G2,C2,D2,E8,F2,F2,F2,F2,F2,E2,E2,E2,E2,D2,D2,E2,D4,G4,E2,E2,E4,E2,E2,E4,E2,G2,C2,D2,E8,F2,F2,F2,F2,F2,E2,E2,E2,G2,G2,F2,D2,C8,",

   "D4,B4,B4,A4,A4,G4,E4,D4.D2,E4,E4,A4,F4,D8.D4,d4,d4,c4,c4,B4,G4,E4.E2,F4,F4,A4,A4,G8,",

   "G2,A2,G2,E8.G2,A2,G2,E8.d4.d2,B8,c4,c2,G8,A4,A2,c2,B2,A2,G2,A2,G2,E8,",

   "C1,D4,C2,C2,D4,C2,C2,F3,F1,E2,E2,F4.E1,D4,E2,F2,D4,E2,F2,G3,B1,G2,B2,G4.F1,E4,D2,C2,E4,D2,C2,D3,C1,C2,F2,A4.c1,B2,G2,A2,F2,G2,E2,F2,D2,C3,A1,C2,G2,F4,",

   "A1,B2,c2,A3,A1,A2,B2,A2,F2,G4,G2,A2,G2,E2,F4,F2,G2,F2,D2,E4,E2,F2,E2,C2,E4,E2,F2,A2,c2,d4.c1,B2,F2,B4,B2,c2,B2,E2,A4.",

   "E2,E2,E2,D2,E2,G2,B3,G1,E2,E2,G2,E2,E4,D2,D2,F2,F2,A2,A2,F2,F2,G3,G1,E2,E2,G2,E2,E4,D4,E2,E2,E2,D2,G2,A2,B4,c2,B2,A2,G2,F2,E2,G3,F1,F8,E4,",

   "A2,A1,B1,A2,F2,F1,G1,F2,E2,E1,F1,E1,C1,D1,E1,F1,G1,A2,A2,A1,B1,A2,E2,A1,B1,c2,d2,F2,G2,A4_D1,D1,D1,E1,F1,E1,D1,E1,F2,D2,E1,E1,E1,F1,G1,F1,E1,F1,G2,E2,F1,G1,A2,G1,F1,G1,A1,B2,A1,G1,A1,B1,c2,B1,A1,d2,d2,",
};

static void playNote(uint32_t note, uint32_t durationMs) {

    uint32_t t = 0;

    if (note > 0) {

        while (t < (durationMs*1000)) {
            P1_2_HIGH();
            delay32Us(0, note / 2);

            P1_2_LOW();
            delay32Us(0, note / 2);

            t += note;
        }

    }
    else {
        delay32Ms(0, durationMs);
    }
}

static uint32_t getNote(uint8_t ch){
    if (ch >= 'A' && ch <= 'G')
        return notes[ch - 'A'];

    if (ch >= 'a' && ch <= 'g')
        return notes[ch - 'a' + 7];

    return 0;
}

static uint32_t getDuration(uint8_t ch){
    if (ch < '0' || ch > '9')
        return 400;

    /* number of ms */

    return (ch - '0') * 200;
}

static uint32_t getPause(uint8_t ch){
    switch (ch) {
    case '+':
        return 0;
    case ',':
        return 5;
    case '.':
        return 20;
    case '_':
        return 30;
    default:
        return 5;
    }
}


static void playSong(uint8_t *song) {
    uint32_t note = 0;
    uint32_t dur  = 0;
    uint32_t pause = 0;
    while(*song != '\0') {
        note = getNote(*song++);
        if (*song == '\0')
            break;
        dur  = getDuration(*song++);
        if (*song == '\0')
            break;
        pause = getPause(*song++);

        playNote(note, dur);
        delay32Ms(0, pause);
    }
}


















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
			width=24;
			size=23;
		}
		else if(type==copter){
			Image=Copter_Image;
			width=16;
			size=20;
		}
		else if(type==fuel){
			Image=Fuel_Image;
			width=24;
			size=23;
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

void print_painel(uint16_t  combustivel,uint16_t  pontuacao, uint16_t  vidas){
			char pbufx[10];

			GrContextForegroundSet(&sContext, ClrYellow);
			GrContextBackgroundSet(&sContext, ClrGray);
			intToString(pontuacao, pbufx, 4, 10, 4);
			
  		GrStringDraw(&sContext, (char*)pbufx, -1,50, 105,true);
			intToString(vidas, pbufx, 4, 10, 1);
			GrStringDraw(&sContext, pbufx, -1,(sContext.psFont->ui8MaxWidth)*5, (sContext.psFont->ui8Height+2)*12,true);
		  
			print(96,112,40,7);
			print(4,114 , combustivel+42, 1);
				print(4,118 , combustivel+42, 1);
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

		while(1){
		evt = osSignalWait (0x01, 10000);
		if (evt.status == osEventSignal)  {
			horiz = joy_read_x();
				vert = joy_read_y();
		
	//			center = joy_read_center();
			if(horiz>0x900){
				x++;
				x++;
			}
			else if(horiz<0x700){
				x--;
				x--;
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
		}
		//osDelay(200);	
		}
	
}
void Cenario (void const *argument) {
			
}
void VeiculoJogador (void const *argument) {
	osEvent evt;
	uint16_t val;
	struct obstaculo bullet={1,-1,-1};
	while(1){
		bullet.y--;
		evt = osSignalWait (0x01, 10000);
		if (evt.status == osEventSignal)  {
			val = osMutexWait (display_mutex_id, 1000);// wait 10 mSec
			switch (val) {
				case osOK:
					print(21,90,x,0);
					if(bullet.y>0){
						print(3,bullet.y,bullet.x,1);
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
			bullet.x=x;
			bullet.y=90;
		}
		
		osThreadYield (); 
		}
	}
	
	
}
void PainelControle (void const *argument) {
	uint16_t   combustivel=40;
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
	int i;
	while(1){
		evt = osSignalWait (0x01, 10000);
		if (evt.status == osEventSignal)  {
			
			val = osMutexWait (display_mutex_id, 1000);
			switch (val) {
				case osOK:
					obstacleList[0].y=obstacleList[0].y+velocidade;
					print(21,obstacleList[0].y,50,2); 
				print(21,obstacleList[1].y,50,4); 

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
	
}

osTimerDef(timer_0, refresh);


int Init_Thread (void) {
	timer_Id= osTimerCreate(osTimer(timer_0), osTimerPeriodic, (void*)0);
	osTimerStart(timer_Id, 1000);

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
