#define RCGCUART 0x618
#define RCGCGPIO 
#define GPIOPCTL
#define UARTIBRD 0x024
#define UARTLCRH 0x02C
#define UARTFBRD 0x028
#define UARTCC 0xFC8
#define UARTDMACTL 0x048
#define UARTCTL 0x030
#define UART0 0x4000.C000
#define UART1 0x4000.D000
#define UART2 0x4000.E000
#define UART3 0x4000.F000
#define UART4 0x4001.0000
#define UART5 0x4001.1000
#define UART6 0x4001.2000
#define UART7 0x4001.3000

#define UARTBASE                      0x4000C000UL
#define UART0_BASE                      0x4000C000UL
#define UART1_BASE                      0x4000D000UL
#define UART2_BASE                      0x4000E000UL
#define UART3_BASE                      0x4000F000UL
#define UART4_BASE                      0x40010000UL
#define UART5_BASE                      0x40011000UL
#define UART6_BASE                      0x40012000UL
#define UART7_BASE                      0x40013000UL



typedef unsigned __int32 uint32_t;



typedef struct {                                    /*!< UART0 Structure                                                       */
  uint32_t  DR;                                /*!< UART Data                                                             */
  
  union {
		uint32_t  ECR_UART_ALT;                    /*!< UART Receive Status/Error Clear                                       */
		uint32_t  RSR;                             /*!< UART Receive Status/Error Clear                                       */
  };
  uint32_t  RESERVED0[4];
  uint32_t  FR;                                /*!< UART Flag                                                             */
  uint32_t  RESERVED1;
  uint32_t  ILPR;                              /*!< UART IrDA Low-Power Register                                          */
  uint32_t  IBRD;                              /*!< UART Integer Baud-Rate Divisor                                        */
  uint32_t  FBRD;                              /*!< UART Fractional Baud-Rate Divisor                                     */
  uint32_t  LCRH;                              /*!< UART Line Control                                                     */
  uint32_t  CTL;                               /*!< UART Control                                                          */
  uint32_t  IFLS;                              /*!< UART Interrupt FIFO Level Select                                      */
  uint32_t  IM;                                /*!< UART Interrupt Mask                                                   */
  uint32_t  RIS;                               /*!< UART Raw Interrupt Status                                             */
  uint32_t  MIS;                               /*!< UART Masked Interrupt Status                                          */
  uint32_t  ICR;                               /*!< UART Interrupt Clear                                                  */
  uint32_t  DMACTL;                            /*!< UART DMA Control                                                      */
  uint32_t  RESERVED2[22];
  uint32_t  _9BITADDR;                         /*!< UART 9-Bit Self Address                                               */
  uint32_t  _9BITAMASK;                        /*!< UART 9-Bit Self Address Mask                                          */
  uint32_t  RESERVED3[965];
  uint32_t  PP;                                /*!< UART Peripheral Properties                                            */
  uint32_t  RESERVED4;
  uint32_t  CC;                                /*!< UART Clock Configuration                                              */
} UART0_Type;


typedef struct {                                    /*!< SYSCTL Structure                                                      */
  __IO uint32_t  DID0;                              /*!< Device Identification 0                                               */
  __IO uint32_t  DID1;                              /*!< Device Identification 1                                               */
  __I  uint32_t  RESERVED0[12];
  __IO uint32_t  PTBOCTL;                           /*!< Power-Temp Brown Out Control                                          */
  __I  uint32_t  RESERVED1[5];
  __IO uint32_t  RIS;                               /*!< Raw Interrupt Status                                                  */
  __IO uint32_t  IMC;                               /*!< Interrupt Mask Control                                                */
  __IO uint32_t  MISC;                              /*!< Masked Interrupt Status and Clear                                     */
  __IO uint32_t  RESC;                              /*!< Reset Cause                                                           */
  __IO uint32_t  PWRTC;                             /*!< Power-Temperature Cause                                               */
  __IO uint32_t  NMIC;                              /*!< NMI Cause Register                                                    */
  __I  uint32_t  RESERVED2[5];
  __IO uint32_t  MOSCCTL;                           /*!< Main Oscillator Control                                               */
  __I  uint32_t  RESERVED3[12];
  __IO uint32_t  RSCLKCFG;                          /*!< Run and Sleep Mode Configuration Register                             */
  __I  uint32_t  RESERVED4[3];
  __IO uint32_t  MEMTIM0;                           /*!< Memory Timing Parameter Register 0 for Main Flash and EEPROM          */
  __I  uint32_t  RESERVED5[29];
  __IO uint32_t  ALTCLKCFG;                         /*!< Alternate Clock Configuration                                         */
  __I  uint32_t  RESERVED6[2];
  __IO uint32_t  DSCLKCFG;                          /*!< Deep Sleep Clock Configuration Register                               */
  __IO uint32_t  DIVSCLK;                           /*!< Divisor and Source Clock Configuration                                */
  __IO uint32_t  SYSPROP;                           /*!< System Properties                                                     */
  __IO uint32_t  PIOSCCAL;                          /*!< Precision Internal Oscillator Calibration                             */
  __IO uint32_t  PIOSCSTAT;                         /*!< Precision Internal Oscillator Statistics                              */
  __I  uint32_t  RESERVED7[2];
  __IO uint32_t  PLLFREQ0;                          /*!< PLL Frequency 0                                                       */
  __IO uint32_t  PLLFREQ1;                          /*!< PLL Frequency 1                                                       */
  __IO uint32_t  PLLSTAT;                           /*!< PLL Status                                                            */
  __I  uint32_t  RESERVED8[7];
  __IO uint32_t  SLPPWRCFG;                         /*!< Sleep Power Configuration                                             */
  __IO uint32_t  DSLPPWRCFG;                        /*!< Deep-Sleep Power Configuration                                        */
  __I  uint32_t  RESERVED9[4];
  __IO uint32_t  NVMSTAT;                           /*!< Non-Volatile Memory Information                                       */
  __I  uint32_t  RESERVED10[4];
  __IO uint32_t  LDOSPCTL;                          /*!< LDO Sleep Power Control                                               */
  __I  uint32_t  RESERVED11;
  __IO uint32_t  LDODPCTL;                          /*!< LDO Deep-Sleep Power Control                                          */
  __I  uint32_t  RESERVED12[6];
  __IO uint32_t  RESBEHAVCTL;                       /*!< Reset Behavior Control Register                                       */
  __I  uint32_t  RESERVED13[6];
  __IO uint32_t  HSSR;                              /*!< Hardware System Service Request                                       */
  __I  uint32_t  RESERVED14[34];
  __IO uint32_t  USBPDS;                            /*!< USB Power Domain Status                                               */
  __IO uint32_t  USBMPC;                            /*!< USB Memory Power Control                                              */
  __IO uint32_t  EMACPDS;                           /*!< Ethernet MAC Power Domain Status                                      */
  __IO uint32_t  EMACMPC;                           /*!< Ethernet MAC Memory Power Control                                     */
  __I  uint32_t  RESERVED15[28];
  __IO uint32_t  PPWD;                              /*!< Watchdog Timer Peripheral Present                                     */
  __IO uint32_t  PPTIMER;                           /*!< 16/32-Bit General-Purpose Timer Peripheral Present                    */
  __IO uint32_t  PPGPIO;                            /*!< General-Purpose Input/Output Peripheral Present                       */
  __IO uint32_t  PPDMA;                             /*!< Micro Direct Memory Access Peripheral Present                         */
  __IO uint32_t  PPEPI;                             /*!< EPI Peripheral Present                                                */
  __IO uint32_t  PPHIB;                             /*!< Hibernation Peripheral Present                                        */
  __IO uint32_t  PPUART;                            /*!< Universal Asynchronous Receiver/Transmitter Peripheral Present        */
  __IO uint32_t  PPSSI;                             /*!< Synchronous Serial Interface Peripheral Present                       */
  __IO uint32_t  PPI2C;                             /*!< Inter-Integrated Circuit Peripheral Present                           */
  __I  uint32_t  RESERVED16;
  __IO uint32_t  PPUSB;                             /*!< Universal Serial Bus Peripheral Present                               */
  __I  uint32_t  RESERVED17;
  __IO uint32_t  PPEPHY;                            /*!< Ethernet PHY Peripheral Present                                       */
  __IO uint32_t  PPCAN;                             /*!< Controller Area Network Peripheral Present                            */
  __IO uint32_t  PPADC;                             /*!< Analog-to-Digital Converter Peripheral Present                        */
  __IO uint32_t  PPACMP;                            /*!< Analog Comparator Peripheral Present                                  */
  __IO uint32_t  PPPWM;                             /*!< Pulse Width Modulator Peripheral Present                              */
  __IO uint32_t  PPQEI;                             /*!< Quadrature Encoder Interface Peripheral Present                       */
  __IO uint32_t  PPLPC;                             /*!< Low Pin Count Interface Peripheral Present                            */
  __I  uint32_t  RESERVED18;
  __IO uint32_t  PPPECI;                            /*!< Platform Environment Control Interface Peripheral Present             */
  __IO uint32_t  PPFAN;                             /*!< Fan Control Peripheral Present                                        */
  __IO uint32_t  PPEEPROM;                          /*!< EEPROM Peripheral Present                                             */
  __IO uint32_t  PPWTIMER;                          /*!< 32/64-Bit Wide General-Purpose Timer Peripheral Present               */
  __I  uint32_t  RESERVED19[4];
  __IO uint32_t  PPRTS;                             /*!< Remote Temperature Sensor Peripheral Present                          */
  __IO uint32_t  PPCCM;                             /*!< CRC and Cryptographic Modules Peripheral Present                      */
  __I  uint32_t  RESERVED20[6];
  __IO uint32_t  PPLCD;                             /*!< LCD Peripheral Present                                                */
  __I  uint32_t  RESERVED21;
  __IO uint32_t  PPOWIRE;                           /*!< 1-Wire Peripheral Present                                             */
  __IO uint32_t  PPEMAC;                            /*!< Ethernet MAC Peripheral Present                                       */
  __I  uint32_t  RESERVED22;
  __IO uint32_t  PPHIM;                             /*!< Human Interface Master Peripheral Present                             */
  __I  uint32_t  RESERVED23[86];
  __IO uint32_t  SRWD;                              /*!< Watchdog Timer Software Reset                                         */
  __IO uint32_t  SRTIMER;                           /*!< 16/32-Bit General-Purpose Timer Software Reset                        */
  __IO uint32_t  SRGPIO;                            /*!< General-Purpose Input/Output Software Reset                           */
  __IO uint32_t  SRDMA;                             /*!< Micro Direct Memory Access Software Reset                             */
  __IO uint32_t  SREPI;                             /*!< EPI Software Reset                                                    */
  __IO uint32_t  SRHIB;                             /*!< Hibernation Software Reset                                            */
  __IO uint32_t  SRUART;                            /*!< Universal Asynchronous Receiver/Transmitter Software Reset            */
  __IO uint32_t  SRSSI;                             /*!< Synchronous Serial Interface Software Reset                           */
  __IO uint32_t  SRI2C;                             /*!< Inter-Integrated Circuit Software Reset                               */
  __I  uint32_t  RESERVED24;
  __IO uint32_t  SRUSB;                             /*!< Universal Serial Bus Software Reset                                   */
  __I  uint32_t  RESERVED25;
  __IO uint32_t  SREPHY;                            /*!< Ethernet PHY Software Reset                                           */
  __IO uint32_t  SRCAN;                             /*!< Controller Area Network Software Reset                                */
  __IO uint32_t  SRADC;                             /*!< Analog-to-Digital Converter Software Reset                            */
  __IO uint32_t  SRACMP;                            /*!< Analog Comparator Software Reset                                      */
  __IO uint32_t  SRPWM;                             /*!< Pulse Width Modulator Software Reset                                  */
  __IO uint32_t  SRQEI;                             /*!< Quadrature Encoder Interface Software Reset                           */
  __I  uint32_t  RESERVED26[4];
  __IO uint32_t  SREEPROM;                          /*!< EEPROM Software Reset                                                 */
  __I  uint32_t  RESERVED27[6];
  __IO uint32_t  SRCCM;                             /*!< CRC and Cryptographic Modules Software Reset                          */
  __I  uint32_t  RESERVED28[9];
  __IO uint32_t  SREMAC;                            /*!< Ethernet MAC Software Reset                                           */
  __I  uint32_t  RESERVED29[24];
  __IO uint32_t  RCGCWD;                            /*!< Watchdog Timer Run Mode Clock Gating Control                          */
  __IO uint32_t  RCGCTIMER;                         /*!< 16/32-Bit General-Purpose Timer Run Mode Clock Gating Control         */
  __IO uint32_t  RCGCGPIO;                          /*!< General-Purpose Input/Output Run Mode Clock Gating Control            */
  __IO uint32_t  RCGCDMA;                           /*!< Micro Direct Memory Access Run Mode Clock Gating Control              */
  __IO uint32_t  RCGCEPI;                           /*!< EPI Run Mode Clock Gating Control                                     */
  __IO uint32_t  RCGCHIB;                           /*!< Hibernation Run Mode Clock Gating Control                             */
  __IO uint32_t  RCGCUART;                          /*!< Universal Asynchronous Receiver/Transmitter Run Mode Clock Gating
                                                         Control                                                               */
  __IO uint32_t  RCGCSSI;                           /*!< Synchronous Serial Interface Run Mode Clock Gating Control            */
  __IO uint32_t  RCGCI2C;                           /*!< Inter-Integrated Circuit Run Mode Clock Gating Control                */
  __I  uint32_t  RESERVED30;
  __IO uint32_t  RCGCUSB;                           /*!< Universal Serial Bus Run Mode Clock Gating Control                    */
  __I  uint32_t  RESERVED31;
  __IO uint32_t  RCGCEPHY;                          /*!< Ethernet PHY Run Mode Clock Gating Control                            */
  __IO uint32_t  RCGCCAN;                           /*!< Controller Area Network Run Mode Clock Gating Control                 */
  __IO uint32_t  RCGCADC;                           /*!< Analog-to-Digital Converter Run Mode Clock Gating Control             */
  __IO uint32_t  RCGCACMP;                          /*!< Analog Comparator Run Mode Clock Gating Control                       */
  __IO uint32_t  RCGCPWM;                           /*!< Pulse Width Modulator Run Mode Clock Gating Control                   */
  __IO uint32_t  RCGCQEI;                           /*!< Quadrature Encoder Interface Run Mode Clock Gating Control            */
  __I  uint32_t  RESERVED32[4];
  __IO uint32_t  RCGCEEPROM;                        /*!< EEPROM Run Mode Clock Gating Control                                  */
  __I  uint32_t  RESERVED33[6];
  __IO uint32_t  RCGCCCM;                           /*!< CRC and Cryptographic Modules Run Mode Clock Gating Control           */
  __I  uint32_t  RESERVED34[9];
  __IO uint32_t  RCGCEMAC;                          /*!< Ethernet MAC Run Mode Clock Gating Control                            */
  __I  uint32_t  RESERVED35[24];
  __IO uint32_t  SCGCWD;                            /*!< Watchdog Timer Sleep Mode Clock Gating Control                        */
  __IO uint32_t  SCGCTIMER;                         /*!< 16/32-Bit General-Purpose Timer Sleep Mode Clock Gating Control       */
  __IO uint32_t  SCGCGPIO;                          /*!< General-Purpose Input/Output Sleep Mode Clock Gating Control          */
  __IO uint32_t  SCGCDMA;                           /*!< Micro Direct Memory Access Sleep Mode Clock Gating Control            */
  __IO uint32_t  SCGCEPI;                           /*!< EPI Sleep Mode Clock Gating Control                                   */
  __IO uint32_t  SCGCHIB;                           /*!< Hibernation Sleep Mode Clock Gating Control                           */
  __IO uint32_t  SCGCUART;                          /*!< Universal Asynchronous Receiver/Transmitter Sleep Mode Clock
                                                         Gating Control                                                        */
  __IO uint32_t  SCGCSSI;                           /*!< Synchronous Serial Interface Sleep Mode Clock Gating Control          */
  __IO uint32_t  SCGCI2C;                           /*!< Inter-Integrated Circuit Sleep Mode Clock Gating Control              */
  __I  uint32_t  RESERVED36;
  __IO uint32_t  SCGCUSB;                           /*!< Universal Serial Bus Sleep Mode Clock Gating Control                  */
  __I  uint32_t  RESERVED37;
  __IO uint32_t  SCGCEPHY;                          /*!< Ethernet PHY Sleep Mode Clock Gating Control                          */
  __IO uint32_t  SCGCCAN;                           /*!< Controller Area Network Sleep Mode Clock Gating Control               */
  __IO uint32_t  SCGCADC;                           /*!< Analog-to-Digital Converter Sleep Mode Clock Gating Control           */
  __IO uint32_t  SCGCACMP;                          /*!< Analog Comparator Sleep Mode Clock Gating Control                     */
  __IO uint32_t  SCGCPWM;                           /*!< Pulse Width Modulator Sleep Mode Clock Gating Control                 */
  __IO uint32_t  SCGCQEI;                           /*!< Quadrature Encoder Interface Sleep Mode Clock Gating Control          */
  __I  uint32_t  RESERVED38[4];
  __IO uint32_t  SCGCEEPROM;                        /*!< EEPROM Sleep Mode Clock Gating Control                                */
  __I  uint32_t  RESERVED39[6];
  __IO uint32_t  SCGCCCM;                           /*!< CRC and Cryptographic Modules Sleep Mode Clock Gating Control         */
  __I  uint32_t  RESERVED40[9];
  __IO uint32_t  SCGCEMAC;                          /*!< Ethernet MAC Sleep Mode Clock Gating Control                          */
  __I  uint32_t  RESERVED41[24];
  __IO uint32_t  DCGCWD;                            /*!< Watchdog Timer Deep-Sleep Mode Clock Gating Control                   */
  __IO uint32_t  DCGCTIMER;                         /*!< 16/32-Bit General-Purpose Timer Deep-Sleep Mode Clock Gating
                                                         Control                                                               */
  __IO uint32_t  DCGCGPIO;                          /*!< General-Purpose Input/Output Deep-Sleep Mode Clock Gating Control     */
  __IO uint32_t  DCGCDMA;                           /*!< Micro Direct Memory Access Deep-Sleep Mode Clock Gating Control       */
  __IO uint32_t  DCGCEPI;                           /*!< EPI Deep-Sleep Mode Clock Gating Control                              */
  __IO uint32_t  DCGCHIB;                           /*!< Hibernation Deep-Sleep Mode Clock Gating Control                      */
  __IO uint32_t  DCGCUART;                          /*!< Universal Asynchronous Receiver/Transmitter Deep-Sleep Mode
                                                         Clock Gating Control                                                  */
  __IO uint32_t  DCGCSSI;                           /*!< Synchronous Serial Interface Deep-Sleep Mode Clock Gating Control     */
  __IO uint32_t  DCGCI2C;                           /*!< Inter-Integrated Circuit Deep-Sleep Mode Clock Gating Control         */
  __I  uint32_t  RESERVED42;
  __IO uint32_t  DCGCUSB;                           /*!< Universal Serial Bus Deep-Sleep Mode Clock Gating Control             */
  __I  uint32_t  RESERVED43;
  __IO uint32_t  DCGCEPHY;                          /*!< Ethernet PHY Deep-Sleep Mode Clock Gating Control                     */
  __IO uint32_t  DCGCCAN;                           /*!< Controller Area Network Deep-Sleep Mode Clock Gating Control          */
  __IO uint32_t  DCGCADC;                           /*!< Analog-to-Digital Converter Deep-Sleep Mode Clock Gating Control      */
  __IO uint32_t  DCGCACMP;                          /*!< Analog Comparator Deep-Sleep Mode Clock Gating Control                */
  __IO uint32_t  DCGCPWM;                           /*!< Pulse Width Modulator Deep-Sleep Mode Clock Gating Control            */
  __IO uint32_t  DCGCQEI;                           /*!< Quadrature Encoder Interface Deep-Sleep Mode Clock Gating Control     */
  __I  uint32_t  RESERVED44[4];
  __IO uint32_t  DCGCEEPROM;                        /*!< EEPROM Deep-Sleep Mode Clock Gating Control                           */
  __I  uint32_t  RESERVED45[6];
  __IO uint32_t  DCGCCCM;                           /*!< CRC and Cryptographic Modules Deep-Sleep Mode Clock Gating Control    */
  __I  uint32_t  RESERVED46[9];
  __IO uint32_t  DCGCEMAC;                          /*!< Ethernet MAC Deep-Sleep Mode Clock Gating Control                     */
  __I  uint32_t  RESERVED47[24];
  __IO uint32_t  PCWD;                              /*!< Watchdog Timer Power Control                                          */
  __IO uint32_t  PCTIMER;                           /*!< 16/32-Bit General-Purpose Timer Power Control                         */
  __IO uint32_t  PCGPIO;                            /*!< General-Purpose Input/Output Power Control                            */
  __IO uint32_t  PCDMA;                             /*!< Micro Direct Memory Access Power Control                              */
  __IO uint32_t  PCEPI;                             /*!< External Peripheral Interface Power Control                           */
  __IO uint32_t  PCHIB;                             /*!< Hibernation Power Control                                             */
  __IO uint32_t  PCUART;                            /*!< Universal Asynchronous Receiver/Transmitter Power Control             */
  __IO uint32_t  PCSSI;                             /*!< Synchronous Serial Interface Power Control                            */
  __IO uint32_t  PCI2C;                             /*!< Inter-Integrated Circuit Power Control                                */
  __I  uint32_t  RESERVED48;
  __IO uint32_t  PCUSB;                             /*!< Universal Serial Bus Power Control                                    */
  __I  uint32_t  RESERVED49;
  __IO uint32_t  PCEPHY;                            /*!< Ethernet PHY Power Control                                            */
  __IO uint32_t  PCCAN;                             /*!< Controller Area Network Power Control                                 */
  __IO uint32_t  PCADC;                             /*!< Analog-to-Digital Converter Power Control                             */
  __IO uint32_t  PCACMP;                            /*!< Analog Comparator Power Control                                       */
  __IO uint32_t  PCPWM;                             /*!< Pulse Width Modulator Power Control                                   */
  __IO uint32_t  PCQEI;                             /*!< Quadrature Encoder Interface Power Control                            */
  __I  uint32_t  RESERVED50[4];
  __IO uint32_t  PCEEPROM;                          /*!< EEPROM Power Control                                                  */
  __I  uint32_t  RESERVED51[6];
  __IO uint32_t  PCCCM;                             /*!< CRC and Cryptographic Modules Power Control                           */
  __I  uint32_t  RESERVED52[9];
  __IO uint32_t  PCEMAC;                            /*!< Ethernet MAC Power Control                                            */
  __I  uint32_t  RESERVED53[24];
  __IO uint32_t  PRWD;                              /*!< Watchdog Timer Peripheral Ready                                       */
  __IO uint32_t  PRTIMER;                           /*!< 16/32-Bit General-Purpose Timer Peripheral Ready                      */
  __IO uint32_t  PRGPIO;                            /*!< General-Purpose Input/Output Peripheral Ready                         */
  __IO uint32_t  PRDMA;                             /*!< Micro Direct Memory Access Peripheral Ready                           */
  __IO uint32_t  PREPI;                             /*!< EPI Peripheral Ready                                                  */
  __IO uint32_t  PRHIB;                             /*!< Hibernation Peripheral Ready                                          */
  __IO uint32_t  PRUART;                            /*!< Universal Asynchronous Receiver/Transmitter Peripheral Ready          */
  __IO uint32_t  PRSSI;                             /*!< Synchronous Serial Interface Peripheral Ready                         */
  __IO uint32_t  PRI2C;                             /*!< Inter-Integrated Circuit Peripheral Ready                             */
  __I  uint32_t  RESERVED54;
  __IO uint32_t  PRUSB;                             /*!< Universal Serial Bus Peripheral Ready                                 */
  __I  uint32_t  RESERVED55;
  __IO uint32_t  PREPHY;                            /*!< Ethernet PHY Peripheral Ready                                         */
  __IO uint32_t  PRCAN;                             /*!< Controller Area Network Peripheral Ready                              */
  __IO uint32_t  PRADC;                             /*!< Analog-to-Digital Converter Peripheral Ready                          */
  __IO uint32_t  PRACMP;                            /*!< Analog Comparator Peripheral Ready                                    */
  __IO uint32_t  PRPWM;                             /*!< Pulse Width Modulator Peripheral Ready                                */
  __IO uint32_t  PRQEI;                             /*!< Quadrature Encoder Interface Peripheral Ready                         */
  __I  uint32_t  RESERVED56[4];
  __IO uint32_t  PREEPROM;                          /*!< EEPROM Peripheral Ready                                               */
  __I  uint32_t  RESERVED57[6];
  __IO uint32_t  PRCCM;                             /*!< CRC and Cryptographic Modules Peripheral Ready                        */
  __I  uint32_t  RESERVED58[9];
  __IO uint32_t  PREMAC;                            /*!< Ethernet MAC Peripheral Ready                                         */
} SYSCTL_Type;

#define UART ((UART0_Type *) UARTBASE)


