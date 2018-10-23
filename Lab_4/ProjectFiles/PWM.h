#define RCGCPWM		0x640
#define RCGCGPIO	0x608
#define GPIOAFSEL	0x420
#define GPIOPCTL	0x52C
#define PWMCC			0xFC8
#define PWMBASE		0x40028000UL
//#define PWMCTL		0x000
//#define PWMGENA		0x020
//#define PWMGENB		0x024
//#define PWMLOAD		0x010
//#define PWMCMPA		0x018
//#define PWMCMPB		0x01C
 
//#define PWM0CTL		(PWMBASE*0 + PWMCTL)
//#define PWM0GENA	(PWMBASE*0 + PWMGENA)
//#define PWM0GENB	(PWMBASE*0 + PWMGENB)
//#define PWM0LOAD	(PWMBASE*0 + PWMLOAD)
//#define PWM0CMPA	(PWMBASE*0 + PWMCMPA)
//#define PWM0CMPB	(PWMBASE*0 + PWMCMPB)
//#define PWMENABLE	0x008

typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;


typedef struct {                                    /*!< PWM0 Structure                                                        */
  uint32_t  CTL;                               /*!< PWM Master Control                                                    */
  uint32_t  SYNC;                              /*!< PWM Time Base Sync                                                    */
  uint32_t  ENABLE;                            /*!< PWM Output Enable                                                     */
  uint32_t  INVERT;                            /*!< PWM Output Inversion                                                  */
  uint32_t  FAULT;                             /*!< PWM Output Fault                                                      */
  uint32_t  INTEN;                             /*!< PWM Interrupt Enable                                                  */
  uint32_t  RIS;                               /*!< PWM Raw Interrupt Status                                              */
  uint32_t  ISC;                               /*!< PWM Interrupt Status and Clear                                        */
  uint32_t  STATUS;                            /*!< PWM Status                                                            */
  uint32_t  FAULTVAL;                          /*!< PWM Fault Condition Value                                             */
  uint32_t  ENUPD;                             /*!< PWM Enable Update                                                     */
  uint32_t  RESERVED0[5];
  uint32_t  _0_CTL;                            /*!< PWM0 Control                                                          */
  uint32_t  _0_INTEN;                          /*!< PWM0 Interrupt and Trigger Enable                                     */
  uint32_t  _0_RIS;                            /*!< PWM0 Raw Interrupt Status                                             */
  uint32_t  _0_ISC;                            /*!< PWM0 Interrupt Status and Clear                                       */
  uint32_t  _0_LOAD;                           /*!< PWM0 Load                                                             */
  uint32_t  _0_COUNT;                          /*!< PWM0 Counter                                                          */
  uint32_t  _0_CMPA;                           /*!< PWM0 Compare A                                                        */
  uint32_t  _0_CMPB;                           /*!< PWM0 Compare B                                                        */
  uint32_t  _0_GENA;                           /*!< PWM0 Generator A Control                                              */
  uint32_t  _0_GENB;                           /*!< PWM0 Generator B Control                                              */
  uint32_t  _0_DBCTL;                          /*!< PWM0 Dead-Band Control                                                */
  uint32_t  _0_DBRISE;                         /*!< PWM0 Dead-Band Rising-Edge Delay                                      */
  uint32_t  _0_DBFALL;                         /*!< PWM0 Dead-Band Falling-Edge-Delay                                     */
   uint32_t  _0_FLTSRC0;                        /*!< PWM0 Fault Source 0                                                   */
   uint32_t  _0_FLTSRC1;                        /*!< PWM0 Fault Source 1                                                   */
   uint32_t  _0_MINFLTPER;                      /*!< PWM0 Minimum Fault Period                                             */
   uint32_t  _1_CTL;                            /*!< PWM1 Control                                                          */
   uint32_t  _1_INTEN;                          /*!< PWM1 Interrupt and Trigger Enable                                     */
   uint32_t  _1_RIS;                            /*!< PWM1 Raw Interrupt Status                                             */
   uint32_t  _1_ISC;                            /*!< PWM1 Interrupt Status and Clear                                       */
   uint32_t  _1_LOAD;                           /*!< PWM1 Load                                                             */
   uint32_t  _1_COUNT;                          /*!< PWM1 Counter                                                          */
   uint32_t  _1_CMPA;                           /*!< PWM1 Compare A                                                        */
   uint32_t  _1_CMPB;                           /*!< PWM1 Compare B                                                        */
   uint32_t  _1_GENA;                           /*!< PWM1 Generator A Control                                              */
   uint32_t  _1_GENB;                           /*!< PWM1 Generator B Control                                              */
   uint32_t  _1_DBCTL;                          /*!< PWM1 Dead-Band Control                                                */
   uint32_t  _1_DBRISE;                         /*!< PWM1 Dead-Band Rising-Edge Delay                                      */
   uint32_t  _1_DBFALL;                         /*!< PWM1 Dead-Band Falling-Edge-Delay                                     */
   uint32_t  _1_FLTSRC0;                        /*!< PWM1 Fault Source 0                                                   */
   uint32_t  _1_FLTSRC1;                        /*!< PWM1 Fault Source 1                                                   */
   uint32_t  _1_MINFLTPER;                      /*!< PWM1 Minimum Fault Period                                             */
   uint32_t  _2_CTL;                            /*!< PWM2 Control                                                          */
   uint32_t  _2_INTEN;                          /*!< PWM2 Interrupt and Trigger Enable                                     */
   uint32_t  _2_RIS;                            /*!< PWM2 Raw Interrupt Status                                             */
   uint32_t  _2_ISC;                            /*!< PWM2 Interrupt Status and Clear                                       */
   uint32_t  _2_LOAD;                           /*!< PWM2 Load                                                             */
   uint32_t  _2_COUNT;                          /*!< PWM2 Counter                                                          */
   uint32_t  _2_CMPA;                           /*!< PWM2 Compare A                                                        */
   uint32_t  _2_CMPB;                           /*!< PWM2 Compare B                                                        */
   uint32_t  _2_GENA;                           /*!< PWM2 Generator A Control                                              */
   uint32_t  _2_GENB;                           /*!< PWM2 Generator B Control                                              */
   uint32_t  _2_DBCTL;                          /*!< PWM2 Dead-Band Control                                                */
   uint32_t  _2_DBRISE;                         /*!< PWM2 Dead-Band Rising-Edge Delay                                      */
   uint32_t  _2_DBFALL;                         /*!< PWM2 Dead-Band Falling-Edge-Delay                                     */
   uint32_t  _2_FLTSRC0;                        /*!< PWM2 Fault Source 0                                                   */
   uint32_t  _2_FLTSRC1;                        /*!< PWM2 Fault Source 1                                                   */
   uint32_t  _2_MINFLTPER;                      /*!< PWM2 Minimum Fault Period                                             */
   uint32_t  _3_CTL;                            /*!< PWM3 Control                                                          */
   uint32_t  _3_INTEN;                          /*!< PWM3 Interrupt and Trigger Enable                                     */
   uint32_t  _3_RIS;                            /*!< PWM3 Raw Interrupt Status                                             */
   uint32_t  _3_ISC;                            /*!< PWM3 Interrupt Status and Clear                                       */
   uint32_t  _3_LOAD;                           /*!< PWM3 Load                                                             */
   uint32_t  _3_COUNT;                          /*!< PWM3 Counter                                                          */
   uint32_t  _3_CMPA;                           /*!< PWM3 Compare A                                                        */
   uint32_t  _3_CMPB;                           /*!< PWM3 Compare B                                                        */
   uint32_t  _3_GENA;                           /*!< PWM3 Generator A Control                                              */
   uint32_t  _3_GENB;                           /*!< PWM3 Generator B Control                                              */
   uint32_t  _3_DBCTL;                          /*!< PWM3 Dead-Band Control                                                */
   uint32_t  _3_DBRISE;                         /*!< PWM3 Dead-Band Rising-Edge Delay                                      */
   uint32_t  _3_DBFALL;                         /*!< PWM3 Dead-Band Falling-Edge-Delay                                     */
   uint32_t  _3_FLTSRC0;                        /*!< PWM3 Fault Source 0                                                   */
   uint32_t  _3_FLTSRC1;                        /*!< PWM3 Fault Source 1                                                   */
   uint32_t  _3_MINFLTPER;                      /*!< PWM3 Minimum Fault Period                                             */
    uint32_t  RESERVED1[432];
   uint32_t  _0_FLTSEN;                         /*!< PWM0 Fault Pin Logic Sense                                            */
    uint32_t  _0_FLTSTAT0;                       /*!< PWM0 Fault Status 0                                                   */
    uint32_t  _0_FLTSTAT1;                       /*!< PWM0 Fault Status 1                                                   */
    uint32_t  RESERVED2[29];
   uint32_t  _1_FLTSEN;                         /*!< PWM1 Fault Pin Logic Sense                                            */
    uint32_t  _1_FLTSTAT0;                       /*!< PWM1 Fault Status 0                                                   */
    uint32_t  _1_FLTSTAT1;                       /*!< PWM1 Fault Status 1                                                   */
    uint32_t  RESERVED3[29];
   uint32_t  _2_FLTSEN;                         /*!< PWM2 Fault Pin Logic Sense                                            */
    uint32_t  _2_FLTSTAT0;                       /*!< PWM2 Fault Status 0                                                   */
    uint32_t  _2_FLTSTAT1;                       /*!< PWM2 Fault Status 1                                                   */
    uint32_t  RESERVED4[29];
   uint32_t  _3_FLTSEN;                         /*!< PWM3 Fault Pin Logic Sense                                            */
    uint32_t  _3_FLTSTAT0;                       /*!< PWM3 Fault Status 0                                                   */
    uint32_t  _3_FLTSTAT1;                       /*!< PWM3 Fault Status 1                                                   */
    uint32_t  RESERVED5[397];
   uint32_t  PP;                                /*!< PWM Peripheral Properties                                             */
    uint32_t  RESERVED6;
   uint32_t  CC;                                /*!< PWM Clock Configuration                                               */
} PWM0_Type;



#define PWM0 ((PWM0_Type *) PWMBASE)

