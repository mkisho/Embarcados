#define SYSCTL_BASE                     0x400FE000UL
#define GPIOF_AHB_BASE                  0x4005D000UL
#define PWM0_BASE                       0x40028000UL

typedef unsigned int uint32_t;
typedef unsigned short int uint16_t;

//Struct of the PWM Module
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




typedef struct {                                    /*!< GPIOA_AHB Structure                                                   */
    uint32_t  RESERVED0[255];
   uint32_t  DATA;                              /*!< GPIO Data                                                             */
   uint32_t  DIR;                               /*!< GPIO Direction                                                        */
   uint32_t  IS;                                /*!< GPIO Interrupt Sense                                                  */
   uint32_t  IBE;                               /*!< GPIO Interrupt Both Edges                                             */
   uint32_t  IEV;                               /*!< GPIO Interrupt Event                                                  */
   uint32_t  IM;                                /*!< GPIO Interrupt Mask                                                   */
   uint32_t  RIS;                               /*!< GPIO Raw Interrupt Status                                             */
   uint32_t  MIS;                               /*!< GPIO Masked Interrupt Status                                          */
    uint32_t  ICR;                               /*!< GPIO Interrupt Clear                                                  */
   uint32_t  AFSEL;                             /*!< GPIO Alternate Function Select                                        */
    uint32_t  RESERVED1[55];
   uint32_t  DR2R;                              /*!< GPIO 2-mA Drive Select                                                */
   uint32_t  DR4R;                              /*!< GPIO 4-mA Drive Select                                                */
   uint32_t  DR8R;                              /*!< GPIO 8-mA Drive Select                                                */
   uint32_t  ODR;                               /*!< GPIO Open Drain Select                                                */
   uint32_t  PUR;                               /*!< GPIO Pull-Up Select                                                   */
   uint32_t  PDR;                               /*!< GPIO Pull-Down Select                                                 */
   uint32_t  SLR;                               /*!< GPIO Slew Rate Control Select                                         */
   uint32_t  DEN;                               /*!< GPIO Digital Enable                                                   */
   uint32_t  LOCK;                              /*!< GPIO Lock                                                             */
    uint32_t  CR;                                /*!< GPIO Commit                                                           */
   uint32_t  AMSEL;                             /*!< GPIO Analog Mode Select                                               */
   uint32_t  PCTL;                              /*!< GPIO Port Control                                                     */
   uint32_t  ADCCTL;                            /*!< GPIO ADC Control                                                      */
   uint32_t  DMACTL;                            /*!< GPIO DMA Control                                                      */
   uint32_t  SI;                                /*!< GPIO Select Interrupt                                                 */
   uint32_t  DR12R;                             /*!< GPIO 12-mA Drive Select                                               */
   uint32_t  WAKEPEN;                           /*!< GPIO Wake Pin Enable                                                  */
   uint32_t  WAKELVL;                           /*!< GPIO Wake Level                                                       */
   uint32_t  WAKESTAT;                          /*!< GPIO Wake Status                                                      */
    uint32_t  RESERVED2[669];
   uint32_t  PP;                                /*!< GPIO Peripheral Property                                              */
   uint32_t  PC;                                /*!< GPIO Peripheral Configuration                                         */
} GPIOA_AHB_Type_PWM;




typedef struct {                                    /*!< SYSCTL Structure                                                      */
   uint32_t  DID0;                              /*!< Device Identification 0                                               */
   uint32_t  DID1;                              /*!< Device Identification 1                                               */
    uint32_t  RESERVED0[12];
   uint32_t  PTBOCTL;                           /*!< Power-Temp Brown Out Control                                          */
    uint32_t  RESERVED1[5];
   uint32_t  RIS;                               /*!< Raw Interrupt Status                                                  */
   uint32_t  IMC;                               /*!< Interrupt Mask Control                                                */
   uint32_t  MISC;                              /*!< Masked Interrupt Status and Clear                                     */
   uint32_t  RESC;                              /*!< Reset Cause                                                           */
   uint32_t  PWRTC;                             /*!< Power-Temperature Cause                                               */
   uint32_t  NMIC;                              /*!< NMI Cause Register                                                    */
    uint32_t  RESERVED2[5];
   uint32_t  MOSCCTL;                           /*!< Main Oscillator Control                                               */
    uint32_t  RESERVED3[12];
   uint32_t  RSCLKCFG;                          /*!< Run and Sleep Mode Configuration Register                             */
    uint32_t  RESERVED4[3];
   uint32_t  MEMTIM0;                           /*!< Memory Timing Parameter Register 0 for Main Flash and EEPROM          */
    uint32_t  RESERVED5[29];
   uint32_t  ALTCLKCFG;                         /*!< Alternate Clock Configuration                                         */
    uint32_t  RESERVED6[2];
   uint32_t  DSCLKCFG;                          /*!< Deep Sleep Clock Configuration Register                               */
   uint32_t  DIVSCLK;                           /*!< Divisor and Source Clock Configuration                                */
   uint32_t  SYSPROP;                           /*!< System Properties                                                     */
   uint32_t  PIOSCCAL;                          /*!< Precision Internal Oscillator Calibration                             */
   uint32_t  PIOSCSTAT;                         /*!< Precision Internal Oscillator Statistics                              */
    uint32_t  RESERVED7[2];
   uint32_t  PLLFREQ0;                          /*!< PLL Frequency 0                                                       */
   uint32_t  PLLFREQ1;                          /*!< PLL Frequency 1                                                       */
   uint32_t  PLLSTAT;                           /*!< PLL Status                                                            */
    uint32_t  RESERVED8[7];
   uint32_t  SLPPWRCFG;                         /*!< Sleep Power Configuration                                             */
   uint32_t  DSLPPWRCFG;                        /*!< Deep-Sleep Power Configuration                                        */
    uint32_t  RESERVED9[4];
   uint32_t  NVMSTAT;                           /*!< Non-Volatile Memory Information                                       */
    uint32_t  RESERVED10[4];
   uint32_t  LDOSPCTL;                          /*!< LDO Sleep Power Control                                               */
    uint32_t  RESERVED11;
   uint32_t  LDODPCTL;                          /*!< LDO Deep-Sleep Power Control                                          */
    uint32_t  RESERVED12[6];
   uint32_t  RESBEHAVCTL;                       /*!< Reset Behavior Control Register                                       */
    uint32_t  RESERVED13[6];
   uint32_t  HSSR;                              /*!< Hardware System Service Request                                       */
    uint32_t  RESERVED14[34];
   uint32_t  USBPDS;                            /*!< USB Power Domain Status                                               */
   uint32_t  USBMPC;                            /*!< USB Memory Power Control                                              */
   uint32_t  EMACPDS;                           /*!< Ethernet MAC Power Domain Status                                      */
   uint32_t  EMACMPC;                           /*!< Ethernet MAC Memory Power Control                                     */
    uint32_t  RESERVED15[28];
   uint32_t  PPWD;                              /*!< Watchdog Timer Peripheral Present                                     */
   uint32_t  PPTIMER;                           /*!< 16/32-Bit General-Purpose Timer Peripheral Present                    */
   uint32_t  PPGPIO;                            /*!< General-Purpose Input/Output Peripheral Present                       */
   uint32_t  PPDMA;                             /*!< Micro Direct Memory Access Peripheral Present                         */
   uint32_t  PPEPI;                             /*!< EPI Peripheral Present                                                */
   uint32_t  PPHIB;                             /*!< Hibernation Peripheral Present                                        */
   uint32_t  PPUART;                            /*!< Universal Asynchronous Receiver/Transmitter Peripheral Present        */
   uint32_t  PPSSI;                             /*!< Synchronous Serial Interface Peripheral Present                       */
   uint32_t  PPI2C;                             /*!< Inter-Integrated Circuit Peripheral Present                           */
    uint32_t  RESERVED16;
   uint32_t  PPUSB;                             /*!< Universal Serial Bus Peripheral Present                               */
    uint32_t  RESERVED17;
   uint32_t  PPEPHY;                            /*!< Ethernet PHY Peripheral Present                                       */
   uint32_t  PPCAN;                             /*!< Controller Area Network Peripheral Present                            */
   uint32_t  PPADC;                             /*!< Analog-to-Digital Converter Peripheral Present                        */
   uint32_t  PPACMP;                            /*!< Analog Comparator Peripheral Present                                  */
   uint32_t  PPPWM;                             /*!< Pulse Width Modulator Peripheral Present                              */
   uint32_t  PPQEI;                             /*!< Quadrature Encoder Interface Peripheral Present                       */
   uint32_t  PPLPC;                             /*!< Low Pin Count Interface Peripheral Present                            */
    uint32_t  RESERVED18;
   uint32_t  PPPECI;                            /*!< Platform Environment Control Interface Peripheral Present             */
   uint32_t  PPFAN;                             /*!< Fan Control Peripheral Present                                        */
   uint32_t  PPEEPROM;                          /*!< EEPROM Peripheral Present                                             */
   uint32_t  PPWTIMER;                          /*!< 32/64-Bit Wide General-Purpose Timer Peripheral Present               */
    uint32_t  RESERVED19[4];
   uint32_t  PPRTS;                             /*!< Remote Temperature Sensor Peripheral Present                          */
   uint32_t  PPCCM;                             /*!< CRC and Cryptographic Modules Peripheral Present                      */
    uint32_t  RESERVED20[6];
   uint32_t  PPLCD;                             /*!< LCD Peripheral Present                                                */
    uint32_t  RESERVED21;
   uint32_t  PPOWIRE;                           /*!< 1-Wire Peripheral Present                                             */
   uint32_t  PPEMAC;                            /*!< Ethernet MAC Peripheral Present                                       */
    uint32_t  RESERVED22;
   uint32_t  PPHIM;                             /*!< Human Interface Master Peripheral Present                             */
    uint32_t  RESERVED23[86];
   uint32_t  SRWD;                              /*!< Watchdog Timer Software Reset                                         */
   uint32_t  SRTIMER;                           /*!< 16/32-Bit General-Purpose Timer Software Reset                        */
   uint32_t  SRGPIO;                            /*!< General-Purpose Input/Output Software Reset                           */
   uint32_t  SRDMA;                             /*!< Micro Direct Memory Access Software Reset                             */
   uint32_t  SREPI;                             /*!< EPI Software Reset                                                    */
   uint32_t  SRHIB;                             /*!< Hibernation Software Reset                                            */
   uint32_t  SRUART;                            /*!< Universal Asynchronous Receiver/Transmitter Software Reset            */
   uint32_t  SRSSI;                             /*!< Synchronous Serial Interface Software Reset                           */
   uint32_t  SRI2C;                             /*!< Inter-Integrated Circuit Software Reset                               */
    uint32_t  RESERVED24;
   uint32_t  SRUSB;                             /*!< Universal Serial Bus Software Reset                                   */
    uint32_t  RESERVED25;
   uint32_t  SREPHY;                            /*!< Ethernet PHY Software Reset                                           */
   uint32_t  SRCAN;                             /*!< Controller Area Network Software Reset                                */
   uint32_t  SRADC;                             /*!< Analog-to-Digital Converter Software Reset                            */
   uint32_t  SRACMP;                            /*!< Analog Comparator Software Reset                                      */
   uint32_t  SRPWM;                             /*!< Pulse Width Modulator Software Reset                                  */
   uint32_t  SRQEI;                             /*!< Quadrature Encoder Interface Software Reset                           */
    uint32_t  RESERVED26[4];
   uint32_t  SREEPROM;                          /*!< EEPROM Software Reset                                                 */
    uint32_t  RESERVED27[6];
   uint32_t  SRCCM;                             /*!< CRC and Cryptographic Modules Software Reset                          */
    uint32_t  RESERVED28[9];
   uint32_t  SREMAC;                            /*!< Ethernet MAC Software Reset                                           */
    uint32_t  RESERVED29[24];
   uint32_t  RCGCWD;                            /*!< Watchdog Timer Run Mode Clock Gating Control                          */
   uint32_t  RCGCTIMER;                         /*!< 16/32-Bit General-Purpose Timer Run Mode Clock Gating Control         */
   uint32_t  RCGCGPIO;                          /*!< General-Purpose Input/Output Run Mode Clock Gating Control            */
   uint32_t  RCGCDMA;                           /*!< Micro Direct Memory Access Run Mode Clock Gating Control              */
   uint32_t  RCGCEPI;                           /*!< EPI Run Mode Clock Gating Control                                     */
   uint32_t  RCGCHIB;                           /*!< Hibernation Run Mode Clock Gating Control                             */
   uint32_t  RCGCUART;                          /*!< Universal Asynchronous Receiver/Transmitter Run Mode Clock Gating
                                                         Control                                                               */
   uint32_t  RCGCSSI;                           /*!< Synchronous Serial Interface Run Mode Clock Gating Control            */
   uint32_t  RCGCI2C;                           /*!< Inter-Integrated Circuit Run Mode Clock Gating Control                */
    uint32_t  RESERVED30;
   uint32_t  RCGCUSB;                           /*!< Universal Serial Bus Run Mode Clock Gating Control                    */
    uint32_t  RESERVED31;
   uint32_t  RCGCEPHY;                          /*!< Ethernet PHY Run Mode Clock Gating Control                            */
   uint32_t  RCGCCAN;                           /*!< Controller Area Network Run Mode Clock Gating Control                 */
   uint32_t  RCGCADC;                           /*!< Analog-to-Digital Converter Run Mode Clock Gating Control             */
   uint32_t  RCGCACMP;                          /*!< Analog Comparator Run Mode Clock Gating Control                       */
   uint32_t  RCGCPWM;                           /*!< Pulse Width Modulator Run Mode Clock Gating Control                   */
   uint32_t  RCGCQEI;                           /*!< Quadrature Encoder Interface Run Mode Clock Gating Control            */
    uint32_t  RESERVED32[4];
   uint32_t  RCGCEEPROM;                        /*!< EEPROM Run Mode Clock Gating Control                                  */
    uint32_t  RESERVED33[6];
   uint32_t  RCGCCCM;                           /*!< CRC and Cryptographic Modules Run Mode Clock Gating Control           */
    uint32_t  RESERVED34[9];
   uint32_t  RCGCEMAC;                          /*!< Ethernet MAC Run Mode Clock Gating Control                            */
    uint32_t  RESERVED35[24];
   uint32_t  SCGCWD;                            /*!< Watchdog Timer Sleep Mode Clock Gating Control                        */
   uint32_t  SCGCTIMER;                         /*!< 16/32-Bit General-Purpose Timer Sleep Mode Clock Gating Control       */
   uint32_t  SCGCGPIO;                          /*!< General-Purpose Input/Output Sleep Mode Clock Gating Control          */
   uint32_t  SCGCDMA;                           /*!< Micro Direct Memory Access Sleep Mode Clock Gating Control            */
   uint32_t  SCGCEPI;                           /*!< EPI Sleep Mode Clock Gating Control                                   */
   uint32_t  SCGCHIB;                           /*!< Hibernation Sleep Mode Clock Gating Control                           */
   uint32_t  SCGCUART;                          /*!< Universal Asynchronous Receiver/Transmitter Sleep Mode Clock
                                                         Gating Control                                                        */
   uint32_t  SCGCSSI;                           /*!< Synchronous Serial Interface Sleep Mode Clock Gating Control          */
   uint32_t  SCGCI2C;                           /*!< Inter-Integrated Circuit Sleep Mode Clock Gating Control              */
    uint32_t  RESERVED36;
   uint32_t  SCGCUSB;                           /*!< Universal Serial Bus Sleep Mode Clock Gating Control                  */
    uint32_t  RESERVED37;
   uint32_t  SCGCEPHY;                          /*!< Ethernet PHY Sleep Mode Clock Gating Control                          */
   uint32_t  SCGCCAN;                           /*!< Controller Area Network Sleep Mode Clock Gating Control               */
   uint32_t  SCGCADC;                           /*!< Analog-to-Digital Converter Sleep Mode Clock Gating Control           */
   uint32_t  SCGCACMP;                          /*!< Analog Comparator Sleep Mode Clock Gating Control                     */
   uint32_t  SCGCPWM;                           /*!< Pulse Width Modulator Sleep Mode Clock Gating Control                 */
   uint32_t  SCGCQEI;                           /*!< Quadrature Encoder Interface Sleep Mode Clock Gating Control          */
    uint32_t  RESERVED38[4];
   uint32_t  SCGCEEPROM;                        /*!< EEPROM Sleep Mode Clock Gating Control                                */
    uint32_t  RESERVED39[6];
   uint32_t  SCGCCCM;                           /*!< CRC and Cryptographic Modules Sleep Mode Clock Gating Control         */
    uint32_t  RESERVED40[9];
   uint32_t  SCGCEMAC;                          /*!< Ethernet MAC Sleep Mode Clock Gating Control                          */
    uint32_t  RESERVED41[24];
   uint32_t  DCGCWD;                            /*!< Watchdog Timer Deep-Sleep Mode Clock Gating Control                   */
   uint32_t  DCGCTIMER;                         /*!< 16/32-Bit General-Purpose Timer Deep-Sleep Mode Clock Gating
                                                         Control                                                               */
   uint32_t  DCGCGPIO;                          /*!< General-Purpose Input/Output Deep-Sleep Mode Clock Gating Control     */
   uint32_t  DCGCDMA;                           /*!< Micro Direct Memory Access Deep-Sleep Mode Clock Gating Control       */
   uint32_t  DCGCEPI;                           /*!< EPI Deep-Sleep Mode Clock Gating Control                              */
   uint32_t  DCGCHIB;                           /*!< Hibernation Deep-Sleep Mode Clock Gating Control                      */
   uint32_t  DCGCUART;                          /*!< Universal Asynchronous Receiver/Transmitter Deep-Sleep Mode
                                                         Clock Gating Control                                                  */
   uint32_t  DCGCSSI;                           /*!< Synchronous Serial Interface Deep-Sleep Mode Clock Gating Control     */
   uint32_t  DCGCI2C;                           /*!< Inter-Integrated Circuit Deep-Sleep Mode Clock Gating Control         */
    uint32_t  RESERVED42;
   uint32_t  DCGCUSB;                           /*!< Universal Serial Bus Deep-Sleep Mode Clock Gating Control             */
    uint32_t  RESERVED43;
   uint32_t  DCGCEPHY;                          /*!< Ethernet PHY Deep-Sleep Mode Clock Gating Control                     */
   uint32_t  DCGCCAN;                           /*!< Controller Area Network Deep-Sleep Mode Clock Gating Control          */
   uint32_t  DCGCADC;                           /*!< Analog-to-Digital Converter Deep-Sleep Mode Clock Gating Control      */
   uint32_t  DCGCACMP;                          /*!< Analog Comparator Deep-Sleep Mode Clock Gating Control                */
   uint32_t  DCGCPWM;                           /*!< Pulse Width Modulator Deep-Sleep Mode Clock Gating Control            */
   uint32_t  DCGCQEI;                           /*!< Quadrature Encoder Interface Deep-Sleep Mode Clock Gating Control     */
    uint32_t  RESERVED44[4];
   uint32_t  DCGCEEPROM;                        /*!< EEPROM Deep-Sleep Mode Clock Gating Control                           */
    uint32_t  RESERVED45[6];
   uint32_t  DCGCCCM;                           /*!< CRC and Cryptographic Modules Deep-Sleep Mode Clock Gating Control    */
    uint32_t  RESERVED46[9];
   uint32_t  DCGCEMAC;                          /*!< Ethernet MAC Deep-Sleep Mode Clock Gating Control                     */
    uint32_t  RESERVED47[24];
   uint32_t  PCWD;                              /*!< Watchdog Timer Power Control                                          */
   uint32_t  PCTIMER;                           /*!< 16/32-Bit General-Purpose Timer Power Control                         */
   uint32_t  PCGPIO;                            /*!< General-Purpose Input/Output Power Control                            */
   uint32_t  PCDMA;                             /*!< Micro Direct Memory Access Power Control                              */
   uint32_t  PCEPI;                             /*!< External Peripheral Interface Power Control                           */
   uint32_t  PCHIB;                             /*!< Hibernation Power Control                                             */
   uint32_t  PCUART;                            /*!< Universal Asynchronous Receiver/Transmitter Power Control             */
   uint32_t  PCSSI;                             /*!< Synchronous Serial Interface Power Control                            */
   uint32_t  PCI2C;                             /*!< Inter-Integrated Circuit Power Control                                */
    uint32_t  RESERVED48;
   uint32_t  PCUSB;                             /*!< Universal Serial Bus Power Control                                    */
    uint32_t  RESERVED49;
   uint32_t  PCEPHY;                            /*!< Ethernet PHY Power Control                                            */
   uint32_t  PCCAN;                             /*!< Controller Area Network Power Control                                 */
   uint32_t  PCADC;                             /*!< Analog-to-Digital Converter Power Control                             */
   uint32_t  PCACMP;                            /*!< Analog Comparator Power Control                                       */
   uint32_t  PCPWM;                             /*!< Pulse Width Modulator Power Control                                   */
   uint32_t  PCQEI;                             /*!< Quadrature Encoder Interface Power Control                            */
    uint32_t  RESERVED50[4];
   uint32_t  PCEEPROM;                          /*!< EEPROM Power Control                                                  */
    uint32_t  RESERVED51[6];
   uint32_t  PCCCM;                             /*!< CRC and Cryptographic Modules Power Control                           */
    uint32_t  RESERVED52[9];
   uint32_t  PCEMAC;                            /*!< Ethernet MAC Power Control                                            */
    uint32_t  RESERVED53[24];
   uint32_t  PRWD;                              /*!< Watchdog Timer Peripheral Ready                                       */
   uint32_t  PRTIMER;                           /*!< 16/32-Bit General-Purpose Timer Peripheral Ready                      */
   uint32_t  PRGPIO;                            /*!< General-Purpose Input/Output Peripheral Ready                         */
   uint32_t  PRDMA;                             /*!< Micro Direct Memory Access Peripheral Ready                           */
   uint32_t  PREPI;                             /*!< EPI Peripheral Ready                                                  */
   uint32_t  PRHIB;                             /*!< Hibernation Peripheral Ready                                          */
   uint32_t  PRUART;                            /*!< Universal Asynchronous Receiver/Transmitter Peripheral Ready          */
   uint32_t  PRSSI;                             /*!< Synchronous Serial Interface Peripheral Ready                         */
   uint32_t  PRI2C;                             /*!< Inter-Integrated Circuit Peripheral Ready                             */
    uint32_t  RESERVED54;
   uint32_t  PRUSB;                             /*!< Universal Serial Bus Peripheral Ready                                 */
    uint32_t  RESERVED55;
   uint32_t  PREPHY;                            /*!< Ethernet PHY Peripheral Ready                                         */
   uint32_t  PRCAN;                             /*!< Controller Area Network Peripheral Ready                              */
   uint32_t  PRADC;                             /*!< Analog-to-Digital Converter Peripheral Ready                          */
   uint32_t  PRACMP;                            /*!< Analog Comparator Peripheral Ready                                    */
   uint32_t  PRPWM;                             /*!< Pulse Width Modulator Peripheral Ready                                */
   uint32_t  PRQEI;                             /*!< Quadrature Encoder Interface Peripheral Ready                         */
    uint32_t  RESERVED56[4];
   uint32_t  PREEPROM;                          /*!< EEPROM Peripheral Ready                                               */
    uint32_t  RESERVED57[6];
   uint32_t  PRCCM;                             /*!< CRC and Cryptographic Modules Peripheral Ready                        */
    uint32_t  RESERVED58[9];
   uint32_t  PREMAC;                            /*!< Ethernet MAC Peripheral Ready                                         */
} SYSCTL_Type_PWM;


#define PWM0 ((PWM0_Type *) PWM0_BASE)
#define GPIOF_AHB ((GPIOA_AHB_Type_PWM          *) GPIOF_AHB_BASE)
#define SYSCTL  ((SYSCTL_Type_PWM *) SYSCTL_BASE)

extern void PWM_init();
extern void PWM_set_duty(uint16_t n);
