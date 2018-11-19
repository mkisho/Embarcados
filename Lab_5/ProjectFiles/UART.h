#include "TM4C1294NCPDT.h"

#define SYSCTL_BASE                     0x400FE000UL
#define GPIOA_AHB_BASE                  0x40058000UL
#define UART0_BASE                      0x4000C000UL

typedef unsigned int uint32_t;
typedef unsigned short int uint16_t;

#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler type
#endif






#define UART0 ((UART0_Type *) UART0_BASE)
#define GPIOA_AHB ((GPIOA_AHB_Type          *) GPIOA_AHB_BASE)
#define SYSCTL_UART  ((SYSCTL_Type *) SYSCTL_BASE)



//#define NVIC_BASE           (0xE000E100UL)        
#define NVIC_UART           ((NVIC_Type_UART      *)     NVIC_BASE     )

typedef unsigned char uint8_t;

typedef struct
{
  
	uint32_t ISER[8U];               /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register */
        uint32_t RESERVED0[24U];
   uint32_t ICER[8U];               /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register */
        uint32_t RSERVED1[24U];
  uint32_t ISPR[8U];               /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register */
        uint32_t RESERVED2[24U];
  uint32_t ICPR[8U];               /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register */
        uint32_t RESERVED3[24U];
  uint32_t IABR[8U];               /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register */
        uint32_t RESERVED4[56U];
  uint8_t  IP[240U];               /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
        uint32_t RESERVED5[644U];
  uint32_t STIR;                   /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register */
}  NVIC_Type_UART;



void UART_init();
extern char readChar(void);
extern void printChar(char c);
extern void printString(char* string);

