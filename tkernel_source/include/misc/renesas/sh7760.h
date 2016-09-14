/*
 *----------------------------------------------------------------------
 *    T-Kernel Software Library
 *
 *    Copyright(c) 2015 by T.Yokobayashi.
 *----------------------------------------------------------------------
 *
 *    Releaseed by T.Yokobayashi at 2015/12/02.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sh7760.h
 *
 *       RENESAS SH7760(SH-4) register definitions
 *
 *       * this is included from C program source files
 */

#ifndef	__INCLUDE_SH7760_H__
#define	__INCLUDE_SH7760_H__

#define	BIT_ORDER_RIGHT	1		/* ビットフィールドを最下位ビットから割当 */

//2003.12                                                                    

/************************************************************************/
/*      SH7760 Include File                                Ver 2.0      */
/************************************************************************/
struct st_mmu {                                         /* struct MMU   */
              unsigned int      PTEH;                   /* PTEH         */
              unsigned int      PTEL;                   /* PTEL         */
              unsigned int      TTB ;                   /* TTB          */
              unsigned int      TEA ;                   /* TEA          */
              unsigned int      MMUCR;                  /* MMUCR        */
              char              wk[32];                 /*              */
              unsigned int      PTEA;                   /* PTEA         */
};                                                      /*              */
struct st_cache {                                       /* struct CACHE */
                unsigned int    CCR;                    /* CCR          */
                 char           wk[24];                 /*              */
                 unsigned int   QACR0;                  /* QACR0        */
                 unsigned int   QACR1;                  /* QACR1        */
};                                                      /*              */
struct st_exp {                                         /* struct EXP   */
               unsigned int     TRA;                    /* TRA          */
               unsigned int     EXPEVT;                 /* EXPEVT       */
               unsigned int     INTEVT;                 /* INTEVT       */
};                                                      /*              */
struct st_intc {                                        /* struct INTC  */
               union {                                  /* INTPRI00     */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned int      :4;       /*    rfu       */
                            unsigned int      :4;       /*    rfu       */
                            unsigned int      :4;       /*    rfu       */
                            unsigned int      :4;       /*    rfu       */
                            unsigned int _IRQ7:4;       /*    IRQ7      */
                            unsigned int _IRQ6:4;       /*    IRQ6      */
                            unsigned int _IRQ5:4;       /*    IRQ5      */
                            unsigned int _IRQ4:4;       /*    IRQ4      */
  #else
                            unsigned int _IRQ4:4;       /*    IRQ4      */
                            unsigned int _IRQ5:4;       /*    IRQ5      */
                            unsigned int _IRQ6:4;       /*    IRQ6      */
                            unsigned int _IRQ7:4;       /*    IRQ7      */
  #endif
                            }     BIT;                  /*              */
                     }          INTPRI00;               /*              */
               union {                                  /* INTPRI04     */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned int _IIC1  :4;     /*    IIC1      */
                            unsigned int _IIC0  :4;     /*    IIC0      */
                            unsigned int _HAC1  :4;     /*    HAC1      */
                            unsigned int _HAC0  :4;     /*    HAC0      */
                            unsigned int _SSI1  :4;     /*    SSI1      */
                            unsigned int _SSI0  :4;     /*    SSI0      */
                            unsigned int _HCAN21:4;     /*    HCAN21    */
                            unsigned int _HCAN20:4;     /*    HCAN20    */
  #else
                            unsigned int _HCAN20:4;     /*    HCAN20    */
                            unsigned int _HCAN21:4;     /*    HCAN21    */
                            unsigned int _SSI0  :4;     /*    SSI0      */
                            unsigned int _SSI1  :4;     /*    SSI1      */
                            unsigned int _HAC0  :4;     /*    HAC0      */
                            unsigned int _HAC1  :4;     /*    HAC1      */
                            unsigned int _IIC0  :4;     /*    IIC0      */
                            unsigned int _IIC1  :4;     /*    IIC1      */
  #endif
                            }     BIT;                  /*              */
                     }          INTPRI04;               /*              */
               union {                                  /* INTPRI08     */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned int _HSPI  :4;     /*    HSPI      */
                            unsigned int _SIM   :4;     /*    SIM       */
                            unsigned int _SCIF2 :4;     /*    SCIF2     */
                            unsigned int _SCIF1 :4;     /*    SCIF1     */
                            unsigned int _SCIF0 :4;     /*    SCIF0     */
                            unsigned int _DMABRG:4;     /*    DMABRG    */
                            unsigned int _LCDC  :4;     /*    LCDC      */
                            unsigned int _USB   :4;     /*    USB       */
  #else
                            unsigned int _USB   :4;     /*    USB       */
                            unsigned int _LCDC  :4;     /*    LCDC      */
                            unsigned int _DMABRG:4;     /*    DMABRG    */
                            unsigned int _SCIF0 :4;     /*    SCIF0     */
                            unsigned int _SCIF1 :4;     /*    SCIF1     */
                            unsigned int _SCIF2 :4;     /*    SCIF2     */
                            unsigned int _SIM   :4;     /*    SIM       */
                            unsigned int _HSPI  :4;     /*    HSPI      */
  #endif
                            }     BIT;                  /*              */
                     }          INTPRI08;               /*              */
               union {                                  /* INTPRI0C     */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned int _CMT  :4;      /*    CMT       */
                            unsigned int _ADC  :4;      /*    ADC       */
                            unsigned int       :4;      /*              */
                            unsigned int _MFI  :4;      /*    MFI       */
                            unsigned int       :4;      /*              */
                            unsigned int _MMCIF:4;      /*    MMCIF     */
                            unsigned int       :8;      /*              */
  #else
                            unsigned int       :8;      /*              */
                            unsigned int _MMCIF:4;      /*    MMCIF     */
                            unsigned int       :4;      /*              */
                            unsigned int _MFI  :4;      /*    MFI       */
                            unsigned int       :4;      /*              */
                            unsigned int _ADC  :4;      /*    ADC       */
                            unsigned int _CMT  :4;      /*    CMT       */
  #endif
                            }     BIT;                  /*              */
                     }          INTPRI0C;               /*              */
               char             wk1[16];                /*              */
               union {                                  /* INTREQ00     */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned int TXI2    :1;    /*    TXI2      */
                            unsigned int BRI2    :1;    /*    BRI2      */
                            unsigned int RXI2    :1;    /*    RXI2      */
                            unsigned int ERI2    :1;    /*    ERI2      */
                            unsigned int TXI1    :1;    /*    TXI1      */
                            unsigned int BRI1    :1;    /*    BRI1      */
                            unsigned int RXI1    :1;    /*    RXI1      */
                            unsigned int ERI1    :1;    /*    ERI1      */
                            unsigned int TXI0    :1;    /*    TXI0      */
                            unsigned int BRI0    :1;    /*    BRI0      */
                            unsigned int RXI0    :1;    /*    RXI0      */
                            unsigned int ERI0    :1;    /*    ERI0      */
                            unsigned int DMABRGI2:1;    /*    DMABRGI2  */
                            unsigned int DMABRGI1:1;    /*    DMABRGI1  */
                            unsigned int DMABRGI0:1;    /*    DMABRGI0  */
                            unsigned int         :1;    /*              */
                            unsigned int VINT    :1;    /*    VINT      */
                            unsigned int USBI    :1;    /*    USBI      */
                            unsigned int IICI1   :1;    /*    IICI1     */
                            unsigned int IICI0   :1;    /*    IICI0     */
                            unsigned int HACI1   :1;    /*    HACI1     */
                            unsigned int HACI0   :1;    /*    HACI0     */
                            unsigned int SSII1   :1;    /*    SSII1     */
                            unsigned int SSII0   :1;    /*    SSII0     */
                            unsigned int HCANI1  :1;    /*    HCANI1    */
                            unsigned int HCANI0  :1;    /*    HCANI0    */
                            unsigned int         :2;    /*              */
                            unsigned int IRQ7    :1;    /*    IRQ7      */
                            unsigned int IRQ6    :1;    /*    IRQ6      */
                            unsigned int IRQ5    :1;    /*    IRQ5      */
                            unsigned int IRQ4    :1;    /*    IRQ4      */
  #else
                            unsigned int IRQ4    :1;    /*    IRQ4      */
                            unsigned int IRQ5    :1;    /*    IRQ5      */
                            unsigned int IRQ6    :1;    /*    IRQ6      */
                            unsigned int IRQ7    :1;    /*    IRQ7      */
                            unsigned int         :2;    /*              */
                            unsigned int HCANI0  :1;    /*    HCANI0    */
                            unsigned int HCANI1  :1;    /*    HCANI1    */
                            unsigned int SSII0   :1;    /*    SSII0     */
                            unsigned int SSII1   :1;    /*    SSII1     */
                            unsigned int HACI0   :1;    /*    HACI0     */
                            unsigned int HACI1   :1;    /*    HACI1     */
                            unsigned int IICI0   :1;    /*    IICI0     */
                            unsigned int IICI1   :1;    /*    IICI1     */
                            unsigned int USBI    :1;    /*    USBI      */
                            unsigned int VINT    :1;    /*    VINT      */
                            unsigned int         :1;    /*              */
                            unsigned int DMABRGI0:1;    /*    DMABRGI0  */
                            unsigned int DMABRGI1:1;    /*    DMABRGI1  */
                            unsigned int DMABRGI2:1;    /*    DMABRGI2  */
                            unsigned int ERI0    :1;    /*    ERI0      */
                            unsigned int RXI0    :1;    /*    RXI0      */
                            unsigned int BRI0    :1;    /*    BRI0      */
                            unsigned int TXI0    :1;    /*    TXI0      */
                            unsigned int ERI1    :1;    /*    ERI1      */
                            unsigned int RXI1    :1;    /*    RXI1      */
                            unsigned int BRI1    :1;    /*    BRI1      */
                            unsigned int TXI1    :1;    /*    TXI1      */
                            unsigned int ERI2    :1;    /*    ERI2      */
                            unsigned int RXI2    :1;    /*    RXI2      */
                            unsigned int BRI2    :1;    /*    BRI2      */
                            unsigned int TXI2    :1;    /*    TXI2      */
  #endif
                            }     BIT;                  /*              */
                     }          INTREQ00;               /*              */
               union {                                  /* INTREQ04     */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned int CMTI  :1;      /*    CMTI      */
                            unsigned int ADI   :1;      /*    ADI       */
                            unsigned int       :1;      /*              */
                            unsigned int MFII  :1;      /*    MFII      */
                            unsigned int       :8;      /*              */
                            unsigned int MMCI3 :1;      /*    MMCI3     */
                            unsigned int MMCI2 :1;      /*    MMCI2     */
                            unsigned int MMCI1 :1;      /*    MMCI1     */
                            unsigned int MMCI0 :1;      /*    MMCI0     */
                            unsigned int HSPII :1;      /*    HSPII     */
                            unsigned int SMITEI:1;      /*    SIMTEI    */
                            unsigned int SMITXI:1;      /*    SIMTXI    */
                            unsigned int SMIRXI:1;      /*    SIMRXI    */
                            unsigned int SMIERI:1;      /*    SIMERI    */
                            unsigned int       :8;      /*              */
  #else
                            unsigned int       :8;      /*              */
                            unsigned int SMIERI:1;      /*    SIMERI    */
                            unsigned int SMIRXI:1;      /*    SIMRXI    */
                            unsigned int SMITXI:1;      /*    SIMTXI    */
                            unsigned int SMITEI:1;      /*    SIMTEI    */
                            unsigned int HSPII :1;      /*    HSPII     */
                            unsigned int MMCI0 :1;      /*    MMCI0     */
                            unsigned int MMCI1 :1;      /*    MMCI1     */
                            unsigned int MMCI2 :1;      /*    MMCI2     */
                            unsigned int MMCI3 :1;      /*    MMCI3     */
                            unsigned int       :8;      /*              */
                            unsigned int MFII  :1;      /*    MFII      */
                            unsigned int       :1;      /*              */
                            unsigned int ADI   :1;      /*    ADI       */
                            unsigned int CMTI  :1;      /*    CMTI      */
  #endif
                            }     BIT;                  /*              */
                     }          INTREQ04;               /*              */
               char             wk2[24];                /*              */
               union {                                  /* INTMSK00     */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned int TXI2    :1;    /*    TXI2      */
                            unsigned int BRI2    :1;    /*    BRI2      */
                            unsigned int RXI2    :1;    /*    RXI2      */
                            unsigned int ERI2    :1;    /*    ERI2      */
                            unsigned int TXI1    :1;    /*    TXI1      */
                            unsigned int BRI1    :1;    /*    BRI1      */
                            unsigned int RXI1    :1;    /*    RXI1      */
                            unsigned int ERI1    :1;    /*    ERI1      */
                            unsigned int TXI0    :1;    /*    TXI0      */
                            unsigned int BRI0    :1;    /*    BRI0      */
                            unsigned int RXI0    :1;    /*    RXI0      */
                            unsigned int ERI0    :1;    /*    ERI0      */
                            unsigned int DMABRGI2:1;    /*    DMABRGI2  */
                            unsigned int DMABRGI1:1;    /*    DMABRGI1  */
                            unsigned int DMABRGI0:1;    /*    DMABRGI0  */
                            unsigned int         :1;    /*              */
                            unsigned int VINT    :1;    /*    VINT      */
                            unsigned int USBI    :1;    /*    USBI      */
                            unsigned int IICI1   :1;    /*    IICI1     */
                            unsigned int IICI0   :1;    /*    IICI0     */
                            unsigned int HACI1   :1;    /*    HACI1     */
                            unsigned int HACI0   :1;    /*    HACI0     */
                            unsigned int SSII1   :1;    /*    SSII1     */
                            unsigned int SSII0   :1;    /*    SSII0     */
                            unsigned int HCANI1  :1;    /*    HCANI1    */
                            unsigned int HCANI0  :1;    /*    HCANI0    */
                            unsigned int         :2;    /*              */
                            unsigned int IRQ7    :1;    /*    IRQ7      */
                            unsigned int IRQ6    :1;    /*    IRQ6      */
                            unsigned int IRQ5    :1;    /*    IRQ5      */
                            unsigned int IRQ4    :1;    /*    IRQ4      */
  #else
                            unsigned int IRQ4    :1;    /*    IRQ4      */
                            unsigned int IRQ5    :1;    /*    IRQ5      */
                            unsigned int IRQ6    :1;    /*    IRQ6      */
                            unsigned int IRQ7    :1;    /*    IRQ7      */
                            unsigned int         :2;    /*              */
                            unsigned int HCANI0  :1;    /*    HCANI0    */
                            unsigned int HCANI1  :1;    /*    HCANI1    */
                            unsigned int SSII0   :1;    /*    SSII0     */
                            unsigned int SSII1   :1;    /*    SSII1     */
                            unsigned int HACI0   :1;    /*    HACI0     */
                            unsigned int HACI1   :1;    /*    HACI1     */
                            unsigned int IICI0   :1;    /*    IICI0     */
                            unsigned int IICI1   :1;    /*    IICI1     */
                            unsigned int USBI    :1;    /*    USBI      */
                            unsigned int VINT    :1;    /*    VINT      */
                            unsigned int         :1;    /*              */
                            unsigned int DMABRGI0:1;    /*    DMABRGI0  */
                            unsigned int DMABRGI1:1;    /*    DMABRGI1  */
                            unsigned int DMABRGI2:1;    /*    DMABRGI2  */
                            unsigned int ERI0    :1;    /*    ERI0      */
                            unsigned int RXI0    :1;    /*    RXI0      */
                            unsigned int BRI0    :1;    /*    BRI0      */
                            unsigned int TXI0    :1;    /*    TXI0      */
                            unsigned int ERI1    :1;    /*    ERI1      */
                            unsigned int RXI1    :1;    /*    RXI1      */
                            unsigned int BRI1    :1;    /*    BRI1      */
                            unsigned int TXI1    :1;    /*    TXI1      */
                            unsigned int ERI2    :1;    /*    ERI2      */
                            unsigned int RXI2    :1;    /*    RXI2      */
                            unsigned int BRI2    :1;    /*    BRI2      */
                            unsigned int TXI2    :1;    /*    TXI2      */
  #endif
                            }     BIT;                  /*              */
                     }          INTMSK00;               /*              */
               union {                                  /* INTMSK04     */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned int CMTI  :1;      /*    CMTI      */
                            unsigned int ADI   :1;      /*    ADI       */
                            unsigned int       :1;      /*              */
                            unsigned int MFII  :1;      /*    MFII      */
                            unsigned int       :8;      /*              */
                            unsigned int MMCI3 :1;      /*    MMCI3     */
                            unsigned int MMCI2 :1;      /*    MMCI2     */
                            unsigned int MMCI1 :1;      /*    MMCI1     */
                            unsigned int MMCI0 :1;      /*    MMCI0     */
                            unsigned int HSPII :1;      /*    HSPII     */
                            unsigned int SMITEI:1;      /*    SIMTEI    */
                            unsigned int SMITXI:1;      /*    SIMTXI    */
                            unsigned int SMIRXI:1;      /*    SIMRXI    */
                            unsigned int SMIERI:1;      /*    SIMERI    */
                            unsigned int       :8;      /*              */
  #else
                            unsigned int       :8;      /*              */
                            unsigned int SMIERI:1;      /*    SIMERI    */
                            unsigned int SMIRXI:1;      /*    SIMRXI    */
                            unsigned int SMITXI:1;      /*    SIMTXI    */
                            unsigned int SMITEI:1;      /*    SIMTEI    */
                            unsigned int HSPII :1;      /*    HSPII     */
                            unsigned int MMCI0 :1;      /*    MMCI0     */
                            unsigned int MMCI1 :1;      /*    MMCI1     */
                            unsigned int MMCI2 :1;      /*    MMCI2     */
                            unsigned int MMCI3 :1;      /*    MMCI3     */
                            unsigned int       :8;      /*              */
                            unsigned int MFII  :1;      /*    MFII      */
                            unsigned int       :1;      /*              */
                            unsigned int ADI   :1;      /*    ADI       */
                            unsigned int CMTI  :1;      /*    CMTI      */
  #endif
                            }     BIT;                  /*              */
                     }          INTMSK04;               /*              */
               char             wk3[24];                /*              */
               unsigned int     INTMSKCLR00;            /* INTMSKCLR00  */
               unsigned int     INTMSKCLR04;            /* INTMSKCLR04  */
               char             wk4[29884312];          /*              */
               union {                                  /* ICR          */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short IRLM:1;      /*    IRLM      */
                            unsigned short NMIE:1;      /*    NMIE      */
                            unsigned short NMIB:1;      /*    NMIB      */
                            unsigned short     :4;      /*              */
                            unsigned short MAI :1;      /*    MAI       */
                            unsigned short NMIL:1;      /*    NMIL      */
  #else
                            unsigned short NMIL:1;      /*    NMIL      */
                            unsigned short MAI :1;      /*    MAI       */
                            unsigned short     :4;      /*              */
                            unsigned short NMIB:1;      /*    NMIB      */
                            unsigned short NMIE:1;      /*    NMIE      */
                            unsigned short IRLM:1;      /*    IRLM      */
  #endif
                            }       BIT;                /*              */
                     }          ICR;                    /*              */
               char             wk5[2];                 /*              */
               union {                                  /* IPRA         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short      :4;     /*              */
                            unsigned short _TMU2:4;     /*    TMU2      */
                            unsigned short _TMU1:4;     /*    TMU1      */
                            unsigned short _TMU0:4;     /*    TMU0      */
  #else
                            unsigned short _TMU0:4;     /*    TMU0      */
                            unsigned short _TMU1:4;     /*    TMU1      */
                            unsigned short _TMU2:4;     /*    TMU2      */
  #endif
                            }       BIT;                /*              */
                     }          IPRA;                   /*              */
               char             wk6[2];                 /*              */
               union {                                  /* IPRB         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short      :4;     /*              */
                            unsigned short      :4;     /*              */
                            unsigned short _REF:4;      /*    REF       */
                            unsigned short _WDT:4;      /*    WDT       */
  #else
                            unsigned short _WDT:4;      /*    WDT       */
                            unsigned short _REF:4;      /*    REF       */
  #endif
                            }       BIT;                /*              */
                     }          IPRB;                   /*              */
               char             wk7[2];                 /*              */
               union {                                  /* IPRC         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short _HUDI:4;     /*    HUDI      */
                            unsigned short      :4;     /*              */
                            unsigned short _DMAC:4;     /*    DMAC      */
                            unsigned short _GPIO:4;     /*    GPIO      */
  #else
                            unsigned short _GPIO:4;     /*    GPIO      */
                            unsigned short _DMAC:4;     /*    DMAC      */
                            unsigned short      :4;     /*              */
                            unsigned short _HUDI:4;     /*    HUDI      */
  #endif
                            }       BIT;                /*              */
                     }          IPRC;                   /*              */
               char             wk8[2];                 /*              */
               union {                                  /* IPRC         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short _IRL3:4;     /*    IRL3      */
                            unsigned short _IRL2:4;     /*    IRL2      */
                            unsigned short _IRL1:4;     /*    IRL1      */
                            unsigned short _IRL0:4;     /*    IRL0      */
  #else
                            unsigned short _IRL0:4;     /*    IRL0      */
                            unsigned short _IRL1:4;     /*    IRL1      */
                            unsigned short _IRL2:4;     /*    IRL2      */
                            unsigned short _IRL3:4;     /*    IRL3      */
  #endif
                            }       BIT;                /*              */
                     }          IPRD;                   /*              */
};                                                      /*              */
struct st_bsc {                                         /* struct BSC   */
              union {                                   /* WCR4         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned int CSH: 2;         /*    CSH       */
                           unsigned int    :30;         /*              */
  #else
                           unsigned int    :30;         /*              */
                           unsigned int CSH: 2;         /*    CSH       */
  #endif
                           }     BIT;                   /*              */
                    }           WCR4;                   /*              */
              char              wk1[196];               /*              */
              union {                                   /* BCR4         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned int ASYNC0: 1;      /*    ASYNC0    */
                           unsigned int ASYNC1: 1;      /*    ASYNC1    */
                           unsigned int ASYNC2: 1;      /*    ASYNC2    */
                           unsigned int ASYNC3: 1;      /*    ASYNC3    */
                           unsigned int ASYNC4: 1;      /*    ASYNC4    */
                           unsigned int ASYNC5: 1;      /*    ASYNC5    */
                           unsigned int ASYNC6: 1;      /*    ASYNC6    */
                           unsigned int       :25;      /*              */
  #else
                           unsigned int       :25;      /*              */
                           unsigned int ASYNC6: 1;      /*    ASYNC6    */
                           unsigned int ASYNC5: 1;      /*    ASYNC5    */
                           unsigned int ASYNC4: 1;      /*    ASYNC4    */
                           unsigned int ASYNC3: 1;      /*    ASYNC3    */
                           unsigned int ASYNC2: 1;      /*    ASYNC2    */
                           unsigned int ASYNC1: 1;      /*    ASYNC1    */
                           unsigned int ASYNC0: 1;      /*    ASYNC0    */
  #endif
                           }     BIT;                   /*              */
                    }           BCR4;                   /*              */
              char              wk2[24510220];          /*              */
              union {                                   /* BCR1         */
                    unsigned int LONG ;                 /*  Long Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned int A56PCM:1;       /*    A56PCM    */
                           unsigned int       :1;       /*              */
                           unsigned int DRAMTP:3;       /*    DRAMTP    */
                           unsigned int A6BST :3;       /*    A6BST     */
                           unsigned int A5BST :3;       /*    A5BST     */
                           unsigned int A0BST :3;       /*    A0BST     */
                           unsigned int HIZCNT:1;       /*    HIZCNT    */
                           unsigned int HIZMEM:1;       /*    HIZMEM    */
                           unsigned int DMABST:1;       /*    DMABST    */
                           unsigned int MEMMPX:1;       /*    MEMMPX    */
                           unsigned int PSHR  :1;       /*    PSHR      */
                           unsigned int BREQEN:1;       /*    BREQEN    */
                           unsigned int A4MBC :1;       /*    A4MBC     */
                           unsigned int A1MBC :1;       /*    A1MBC     */
                           unsigned int       :2;       /*              */
                           unsigned int OPUP  :1;       /*    OPUP      */
                           unsigned int       :1;       /*              */
                           unsigned int DPUP  :1;       /*    DPUP      */
                           unsigned int       :2;       /*              */
                           unsigned int A0MPX :1;       /*    A0MPX     */
                           unsigned int       :1;       /*              */
                           unsigned int ENDIAN:1;       /*    ENDIAN    */
  #else
                           unsigned int ENDIAN:1;       /*    ENDIAN    */
                           unsigned int       :1;       /*              */
                           unsigned int A0MPX :1;       /*    A0MPX     */
                           unsigned int       :2;       /*              */
                           unsigned int DPUP  :1;       /*    DPUP      */
                           unsigned int       :1;       /*              */
                           unsigned int OPUP  :1;       /*    OPUP      */
                           unsigned int       :2;       /*              */
                           unsigned int A1MBC :1;       /*    A1MBC     */
                           unsigned int A4MBC :1;       /*    A4MBC     */
                           unsigned int BREQEN:1;       /*    BREQEN    */
                           unsigned int PSHR  :1;       /*    PSHR      */
                           unsigned int MEMMPX:1;       /*    MEMMPX    */
                           unsigned int DMABST:1;       /*    DMABST    */
                           unsigned int HIZMEM:1;       /*    HIZMEM    */
                           unsigned int HIZCNT:1;       /*    HIZCNT    */
                           unsigned int A0BST :3;       /*    A0BST     */
                           unsigned int A5BST :3;       /*    A5BST     */
                           unsigned int A6BST :3;       /*    A6BST     */
                           unsigned int DRAMTP:3;       /*    DRAMTP    */
                           unsigned int       :1;       /*              */
                           unsigned int A56PCM:1;       /*    A56PCM    */
  #endif
                           }     BIT;                   /*              */
                    }           BCR1;                   /*              */
              union {                                   /* BCR2         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned short STBIRLEN:1;   /*    STBIRLEN  */
                           unsigned short         :1;   /*              */
                           unsigned short A1SZ    :2;   /*    A1SZ      */
                           unsigned short A2SZ    :2;   /*    A2SZ      */
                           unsigned short A3SZ    :2;   /*    A3SZ      */
                           unsigned short A4SZ    :2;   /*    A4SZ      */
                           unsigned short A5SZ    :2;   /*    A5SZ      */
                           unsigned short A6SZ    :2;   /*    A6SZ      */
                           unsigned short A0SZ    :2;   /*    A0SZ      */
  #else
                           unsigned short A0SZ    :2;   /*    A0SZ      */
                           unsigned short A6SZ    :2;   /*    A6SZ      */
                           unsigned short A5SZ    :2;   /*    A5SZ      */
                           unsigned short A4SZ    :2;   /*    A4SZ      */
                           unsigned short A3SZ    :2;   /*    A3SZ      */
                           unsigned short A2SZ    :2;   /*    A2SZ      */
                           unsigned short A1SZ    :2;   /*    A1SZ      */
                           unsigned short         :1;   /*              */
                           unsigned short STBIRLEN:1;   /*    STBIRLEN  */
  #endif
                           }       BIT;                 /*              */
                    }           BCR2;                   /*              */
              char              wk3[2];                 /*              */
              union {                                   /* WCR1         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned int A0IW :3;        /*    A0IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A1IW :3;        /*    A1IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A2IW :3;        /*    A2IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A3IW :3;        /*    A3IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A4IW :3;        /*    A4IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A5IW :3;        /*    A5IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A6IW :3;        /*    A6IW      */
                           unsigned int      :1;        /*              */
                           unsigned int DMAIW:3;        /*    DMAIW     */
                           unsigned int      :1;        /*              */
  #else
                           unsigned int      :1;        /*              */
                           unsigned int DMAIW:3;        /*    DMAIW     */
                           unsigned int      :1;        /*              */
                           unsigned int A6IW :3;        /*    A6IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A5IW :3;        /*    A5IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A4IW :3;        /*    A4IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A3IW :3;        /*    A3IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A2IW :3;        /*    A2IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A1IW :3;        /*    A1IW      */
                           unsigned int      :1;        /*              */
                           unsigned int A0IW :3;        /*    A0IW      */
  #endif
                           }     BIT;                   /*              */
                    }           WCR1;                   /*              */
              union {                                   /* WCR2         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned int A0B:3;          /*    A0B       */
                           unsigned int A0W:3;          /*    A0W       */
                           unsigned int A1W:3;          /*    A1W       */
                           unsigned int A2W:3;          /*    A2W       */
                           unsigned int    :1;          /*              */
                           unsigned int A3W:3;          /*    A3W       */
                           unsigned int    :1;          /*              */
                           unsigned int A4W:3;          /*    A4W       */
                           unsigned int A5B:3;          /*    A5B       */
                           unsigned int A5W:3;          /*    A5W       */
                           unsigned int A6B:3;          /*    A6B       */
                           unsigned int A6W:3;          /*    A6W       */
  #else
                           unsigned int A6W:3;          /*    A6W       */
                           unsigned int A6B:3;          /*    A6B       */
                           unsigned int A5W:3;          /*    A5W       */
                           unsigned int A5B:3;          /*    A5B       */
                           unsigned int A4W:3;          /*    A4W       */
                           unsigned int    :1;          /*              */
                           unsigned int A3W:3;          /*    A3W       */
                           unsigned int    :1;          /*              */
                           unsigned int A2W:3;          /*    A2W       */
                           unsigned int A1W:3;          /*    A1W       */
                           unsigned int A0W:3;          /*    A0W       */
                           unsigned int A0B:3;          /*    A0B       */
  #endif
                           }     BIT;                   /*              */
                    }           WCR2;                   /*              */
              union {                                   /* WCR3         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned int A0H  :2;        /*    A0H       */
                           unsigned int A0S0 :1;        /*    A0S0      */
                           unsigned int      :1;        /*              */
                           unsigned int A1H  :2;        /*    A1H       */
                           unsigned int A1S0 :1;        /*    A1S0      */
                           unsigned int A1RDH:1;        /*    A1RDH     */
                           unsigned int A2H  :2;        /*    A2H       */
                           unsigned int A2S0 :1;        /*    A2S0      */
                           unsigned int      :1;        /*              */
                           unsigned int A3H  :2;        /*    A3H       */
                           unsigned int A3S0 :1;        /*    A3S0      */
                           unsigned int      :1;        /*              */
                           unsigned int A4H  :2;        /*    A4H       */
                           unsigned int A4S0 :1;        /*    A4S0      */
                           unsigned int A4RDH:1;        /*    A4RDH     */
                           unsigned int A5H  :2;        /*    A5H       */
                           unsigned int A5S0 :1;        /*    A5S0      */
                           unsigned int      :1;        /*              */
                           unsigned int A6H  :2;        /*    A6H       */
                           unsigned int A6S0 :1;        /*    A6S0      */
                           unsigned int      :5;        /*              */
  #else
                           unsigned int      :5;        /*              */
                           unsigned int A6S0 :1;        /*    A6S0      */
                           unsigned int A6H  :2;        /*    A6H       */
                           unsigned int      :1;        /*              */
                           unsigned int A5S0 :1;        /*    A5S0      */
                           unsigned int A5H  :2;        /*    A5H       */
                           unsigned int A4RDH:1;        /*    A4RDH     */
                           unsigned int A4S0 :1;        /*    A4S0      */
                           unsigned int A4H  :2;        /*    A4H       */
                           unsigned int      :1;        /*              */
                           unsigned int A3S0 :1;        /*    A3S0      */
                           unsigned int A3H  :2;        /*    A3H       */
                           unsigned int      :1;        /*              */
                           unsigned int A2S0 :1;        /*    A2S0      */
                           unsigned int A2H  :2;        /*    A2H       */
                           unsigned int A1RDH:1;        /*    A1RDH     */
                           unsigned int A1S0 :1;        /*    A1S0      */
                           unsigned int A1H  :2;        /*    A1H       */
                           unsigned int      :1;        /*              */
                           unsigned int A0S0 :1;        /*    A0S0      */
                           unsigned int A0H  :2;        /*    A0H       */
  #endif
                           }     BIT;                   /*              */
                    }           WCR3;                   /*              */
              union {                                   /* MCR          */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned int       :1;       /*              */
                           unsigned int RMODE :1;       /*    RMODE     */
                           unsigned int RFSH  :1;       /*    RFSH      */
                           unsigned int AMX   :3;       /*    AMX       */
                           unsigned int AMXEXT:1;       /*    AMXEXT    */
                           unsigned int SZ    :2;       /*    SZ        */
                           unsigned int       :1;       /*              */
                           unsigned int TRAS  :3;       /*    TRAS      */
                           unsigned int TRWL  :3;       /*    TRWL      */
                           unsigned int RCD   :2;       /*    RCD       */
                           unsigned int       :1;       /*              */
                           unsigned int TPC   :3;       /*    TPC       */
                           unsigned int       :5;       /*              */
                           unsigned int TRC   :3;       /*    TRC       */
                           unsigned int MRSET :1;       /*    MRSET     */
                           unsigned int RASD  :1;       /*    RASD      */
  #else
                           unsigned int RASD  :1;       /*    RASD      */
                           unsigned int MRSET :1;       /*    MRSET     */
                           unsigned int TRC   :3;       /*    TRC       */
                           unsigned int       :5;       /*              */
                           unsigned int TPC   :3;       /*    TPC       */
                           unsigned int       :1;       /*              */
                           unsigned int RCD   :2;       /*    RCD       */
                           unsigned int TRWL  :3;       /*    TRWL      */
                           unsigned int TRAS  :3;       /*    TRAS      */
                           unsigned int       :1;       /*              */
                           unsigned int SZ    :2;       /*    SZ        */
                           unsigned int AMXEXT:1;       /*    AMXEXT    */
                           unsigned int AMX   :3;       /*    AMX       */
                           unsigned int RFSH  :1;       /*    RFSH      */
                           unsigned int RMODE :1;       /*    RMODE     */
  #endif
                           }     BIT;                   /*              */
                    }           MCR;                    /*              */
              union {                                   /* PCR          */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned short A6TEH:3;      /*    A6TEH     */
                           unsigned short A5TEH:3;      /*    A5TEH     */
                           unsigned short A6TED:3;      /*    A6TED     */
                           unsigned short A5TED:3;      /*    A5TED     */
                           unsigned short A6PCW:2;      /*    A6PCW     */
                           unsigned short A5PCW:2;      /*    A5PCW     */
  #else
                           unsigned short A5PCW:2;      /*    A5PCW     */
                           unsigned short A6PCW:2;      /*    A6PCW     */
                           unsigned short A5TED:3;      /*    A5TED     */
                           unsigned short A6TED:3;      /*    A6TED     */
                           unsigned short A5TEH:3;      /*    A5TEH     */
                           unsigned short A6TEH:3;      /*    A6TEH     */
  #endif
                           }       BIT;                 /*              */
                    }           PCR;                    /*              */
              char              wk4[2];                 /*              */
              union {                                   /* RTCSR        */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned short LMTS:1;       /*    LMTS      */
                           unsigned short OVIE:1;       /*    OVIE      */
                           unsigned short OVF :1;       /*    OVF       */
                           unsigned short CKS :3;       /*    CKS       */
                           unsigned short CMIE:1;       /*    CMIE      */
                           unsigned short CMF :1;       /*    CMF       */
                           unsigned short     :8;       /*              */
  #else
                           unsigned short     :8;       /*              */
                           unsigned short CMF :1;       /*    CMF       */
                           unsigned short CMIE:1;       /*    CMIE      */
                           unsigned short CKS :3;       /*    CKS       */
                           unsigned short OVF :1;       /*    OVF       */
                           unsigned short OVIE:1;       /*    OVIE      */
                           unsigned short LMTS:1;       /*    LMTS      */
  #endif
                           }       BIT;                 /*              */
                    }           RTCSR;                  /*              */
              char              wk5[2];                 /*              */
              unsigned short    RTCNT;                  /* RTCNT        */
              char              wk6[2];                 /*              */
              unsigned short    RTCOR;                  /* RTCOR        */
              char              wk7[2];                 /*              */
              unsigned short    RFCR;                   /* RFCR         */
              char              wk8[38];                /*              */
              union {                                   /* BCR3         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned short SDBL   : 1;   /*    SDBL      */
                           unsigned short        :12;   /*              */
                           unsigned short A4MPX  : 1;   /*    A4MPX     */
                           unsigned short A1MPX  : 1;   /*    A1MPX     */
                           unsigned short MEMMODE: 1;   /*    MEMMODE   */
  #else
                           unsigned short MEMMODE: 1;   /*    MEMMODE   */
                           unsigned short A1MPX  : 1;   /*    A1MPX     */
                           unsigned short A4MPX  : 1;   /*    A4MPX     */
                           unsigned short        :12;   /*              */
                           unsigned short SDBL   : 1;   /*    SDBL      */
  #endif
                           }       BIT;                 /*              */
                    }           BCR3;                   /*              */
};                                                      /*              */
struct st_dmac {                                        /* struct DMAC  */
               unsigned int     DMARSRA;                /* DMARSRA      */
               unsigned int     DMARSRB;                /* DMARSRB      */
               union {                                  /* DMARCR       */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int REX7:1;        /*    REX7      */
                            unsigned int REX6:1;        /*    REX6      */
                            unsigned int REX5:1;        /*    REX5      */
                            unsigned int REX4:1;        /*    REX4      */
                            unsigned int REX3:1;        /*    REX3      */
                            unsigned int REX2:1;        /*    REX2      */
                            unsigned int REX1:1;        /*    REX1      */
                            unsigned int REX0:1;        /*    REX0      */
                            unsigned int RA3 :1;        /*    R/A3      */
                            unsigned int RA2 :1;        /*    R/A2      */
                            unsigned int     :4;        /*              */
                            unsigned int RPR :2;        /*    RPR       */
                            unsigned int     :1;        /*              */
                            unsigned int DS3 :1;        /*    DS3       */
                            unsigned int RL3 :1;        /*    RL3       */
                            unsigned int AL3 :1;        /*    AL3       */
                            unsigned int     :1;        /*              */
                            unsigned int DS2 :1;        /*    DS2       */
                            unsigned int RL2 :1;        /*    RL2       */
                            unsigned int AL2 :1;        /*    AL2       */
                            unsigned int     :1;        /*              */
                            unsigned int DS1 :1;        /*    DS1       */
                            unsigned int RL1 :1;        /*    RL1       */
                            unsigned int AL1 :1;        /*    AL1       */
                            unsigned int     :1;        /*              */
                            unsigned int DS0 :1;        /*    DS0       */
                            unsigned int RL0 :1;        /*    RL0       */
                            unsigned int AL0 :1;        /*    AL0       */
                            }     BIT;                  /*              */
                     }          DMARCR;                 /*              */
               char             wk1[3342324];           /*              */
               union {                                  /* DMABRGCR     */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int A1RXHE:1;      /*    A1RXHE    */
                            unsigned int A1RXEE:1;      /*    A1RXEE    */
                            unsigned int A1TXHE:1;      /*    A1TXHE    */
                            unsigned int A1TXEE:1;      /*    A1TXEE    */
                            unsigned int A0RXHE:1;      /*    A0RXHE    */
                            unsigned int A0RXEE:1;      /*    A0RXEE    */
                            unsigned int A0TXHE:1;      /*    A0TXHE    */
                            unsigned int A0TXEE:1;      /*    A0TXEE    */
                            unsigned int A1RXHF:1;      /*    A1RXHF    */
                            unsigned int A1RXEF:1;      /*    A1RXEF    */
                            unsigned int A1TXHF:1;      /*    A1TXHF    */
                            unsigned int A1TXEF:1;      /*    A1TXEF    */
                            unsigned int A0RXHF:1;      /*    A0RXHF    */
                            unsigned int A0RXEF:1;      /*    A0RXEF    */
                            unsigned int A0TXHF:1;      /*    A0TXHF    */
                            unsigned int A0TXEF:1;      /*    A0TXEF    */
                            unsigned int       :6;      /*              */
                            unsigned int UAE   :1;      /*    UAE       */
                            unsigned int UTE   :1;      /*    UTE       */
                            unsigned int       :6;      /*              */
                            unsigned int UAF   :1;      /*    UAF       */
                            unsigned int UTF   :1;      /*    UTF       */
                            }     BIT;                  /*              */
                     }          DMABRGCR;               /*              */
               char             wk2[60];                /*              */
               void            *DMAATXSAR0;             /* DMAATXSAR0   */
               void            *DMAARXDAR0;             /* DMAARXDAR0   */
               unsigned int     DMAATXTCR0;             /* DMAATXTCR0   */
               unsigned int     DMAARXTCR0;             /* DMAARXTCR0   */
               union {                                  /* DMAACR0      */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int    :6;         /*              */
                            unsigned int RAM:2;         /*    RAM       */
                            unsigned int    :5;         /*              */
                            unsigned int RAR:1;         /*    RAR       */
                            unsigned int RDS:1;         /*    RDS       */
                            unsigned int RDE:1;         /*    RDE       */
                            unsigned int    :6;         /*              */
                            unsigned int TAM:2;         /*    TAM1      */
                            unsigned int    :5;         /*              */
                            unsigned int TAR:1;         /*    TAR       */
                            unsigned int TDS:1;         /*    TDS       */
                            unsigned int TDE:1;         /*    TDE       */
                            }     BIT;                  /*              */
                     }          DMAACR0;                /*              */
               unsigned int     DMAATXTCNT0;            /* DMAATXTCNT0  */
               unsigned int     DMAARXTCNT0;            /* DMAARXTCNT0  */
               char             wk3[4];                 /*              */
               void            *DMAATXSAR1;             /* DMAATXSAR1   */
               void            *DMAARXDAR1;             /* DMAARXDAR1   */
               unsigned int     DMAATXTCR1;             /* DMAATXTCR1   */
               unsigned int     DMAARXTCR1;             /* DMAARXTCR1   */
               union {                                  /* DMAACR1      */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int    :6;         /*              */
                            unsigned int RAM:2;         /*    RAM       */
                            unsigned int    :5;         /*              */
                            unsigned int RAR:1;         /*    RAR       */
                            unsigned int RDS:1;         /*    RDS       */
                            unsigned int RDE:1;         /*    RDE       */
                            unsigned int    :6;         /*              */
                            unsigned int TAM:2;         /*    TAM       */
                            unsigned int    :5;         /*              */
                            unsigned int TAR:1;         /*    TAR       */
                            unsigned int TDS:1;         /*    TDS       */
                            unsigned int TDE:1;         /*    TDE       */
                            }     BIT;                  /*              */
                     }          DMAACR1;                /*              */
               unsigned int     DMAATXTCNT1;            /* DMAATXTCNT1  */
               unsigned int     DMAARXTCNT1;            /* DMAARXTCNT1  */
               char             wk4[4];                 /*              */
               void            *DMAUSAR;                /* DMAUSAR      */
               void            *DMAUDAR;                /* DMAUDAR      */
               union {                                  /* DMAURWSZ     */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int   :15;         /*              */
                            unsigned int RW: 1;         /*    RW        */
                            unsigned int   : 3;         /*              */
                            unsigned int SZ:13;         /*    SZ        */
                            }     BIT;                  /*              */
                     }          DMAURWSZ;               /*              */
               union {                                  /* DMAUCR       */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int      :14;      /*              */
                            unsigned int CVRT : 2;      /*   CVRT       */
                            unsigned int      :14;      /*              */
                            unsigned int START: 1;      /*   START      */
                            }     BIT;                  /*              */
                     }          DMAUCR;                 /*              */
               char             wk5[23330736];          /*              */
               union {                                  /* DMAOR        */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int     :16;       /*              */
                            unsigned int DMS : 2;       /*    DMS       */
                            unsigned int     : 4;       /*              */
                            unsigned int PR  : 2;       /*    PR        */
                            unsigned int     : 5;       /*              */
                            unsigned int AE  : 1;       /*    AE        */
                            unsigned int NMIF: 1;       /*    NMIF      */
                            unsigned int DME : 1;       /*    DME       */
                            }     BIT;                  /*              */
                     }          DMAOR;                  /*              */
};                                                      /*              */
struct st_dmac0 {                                       /* struct DMAC0 */
                void           *SAR;                    /* SAR          */
                void           *DAR;                    /* DAR          */
                unsigned int    DMATCR;                 /* DMATCR       */
                union {                                 /* CHCR         */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned int SSA  :3;      /*    SSA       */
                             unsigned int STC  :1;      /*    STC       */
                             unsigned int DSA  :3;      /*    DSA       */
                             unsigned int DTC  :1;      /*    DTC       */
                             unsigned int      :4;      /*              */
                             unsigned int DS   :1;      /*    DS        */
                             unsigned int RL   :1;      /*    RL        */
                             unsigned int AM   :1;      /*    AM        */
                             unsigned int AL   :1;      /*    AL        */
                             unsigned int DM   :2;      /*    DM        */
                             unsigned int SM   :2;      /*    SM        */
                             unsigned int RS   :4;      /*    RS        */
                             unsigned int TM   :1;      /*    TM        */
                             unsigned int TS   :3;      /*    TS        */
                             unsigned int CHSET:1;      /*    CHSET     */
                             unsigned int IE   :1;      /*    IE        */
                             unsigned int TE   :1;      /*    TE        */
                             unsigned int DE   :1;      /*    DE        */
                             }     BIT;                 /*              */
                      }         CHCR;                   /*              */
};                                                      /*              */
struct st_cpg {                                         /* struct CPG   */
              union {                                   /* DCKDR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int       :24;      /*              */
                           unsigned int DCKEN : 1;      /*    DCKEN     */
                           unsigned int       : 3;      /*              */
                           unsigned int PLL3EN: 1;      /*    PLL3EN    */
                           unsigned int DCKOUT: 1;      /*    DCKOUT    */
                           unsigned int DIV   : 2;      /*    DIV       */
                           }     BIT;                   /*              */
                    }           DCKDR;                  /*              */
              union {                                   /* MCKCR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int      :28;       /*              */
                           unsigned int FLMCK: 4;       /*    FLMCK     */
                           }     BIT;                   /*              */
                    }           MCKCR;                  /*              */
              char              wk1[28704728];          /*              */
              union {                                   /* FRQCR        */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short       :4;     /*              */
                           unsigned short CKOEN :1;     /*    CKOEN     */
                           unsigned short PLL1EN:1;     /*    PLL1EN    */
                           unsigned short PLL2EN:1;     /*    PLL2EN    */
                           unsigned short IFC   :3;     /*    IFC       */
                           unsigned short BFC   :3;     /*    BFC       */
                           unsigned short _PFC  :3;     /*    PFC       */
                           }       BIT;                 /*              */
                    }           FRQCR;                  /*              */
              char              wk2[6];                 /*              */
              union {                                   /* WTCNT        */
                    unsigned char  READ;                /*  Read  Access*/
                    unsigned short WRITE;               /*  Write Access*/
                    }           WTCNT;                  /*              */
              char              wk3[2];                 /*              */
              union {                                   /* WTCSR        */
                    union {                             /*  Read  Access*/
                          unsigned char BYTE;           /*   Byte Access*/
                          struct {                      /*   Bit  Access*/
                                 unsigned char TME :1;  /*    TME       */
                                 unsigned char WTIT:1;  /*    WT/IT     */
                                 unsigned char RSTS:1;  /*    RSTS      */
                                 unsigned char WOVF:1;  /*    WOVF      */
                                 unsigned char IOVF:1;  /*    IOVF      */
                                 unsigned char CKS :3;  /*    CKS       */
                                 }      BIT;            /*              */
                         }         READ;                /*              */
                    unsigned short WRITE;               /*  Write Access*/
                    }           WTCSR;                  /*              */
};                                                      /*              */
union un_wdt {                                          /* union WDT    */
             struct {                                   /* Read  Access */
                    unsigned char WTCNT;                /* WTCNT        */
                    char          wk[3];                /*              */
                    union {                             /* WTCSR        */
                          unsigned char BYTE;           /*  Byte Access */
                          struct {                      /*  Bit  Access */
                                 unsigned char TME :1;  /*    TME       */
                                 unsigned char WTIT:1;  /*    WT/IT     */
                                 unsigned char RSTS:1;  /*    RSTS      */
                                 unsigned char WOVF:1;  /*    WOVF      */
                                 unsigned char IOVF:1;  /*    IOVF      */
                                 unsigned char CKS :3;  /*    CKS       */
                                 }      BIT;            /*              */
                          }       WTCSR;                /*              */
                    } READ;                             /*              */
             struct {                                   /* Write Access */
                    unsigned short WTCNT;               /* WTCNT        */
                    char           wk[2];               /*              */
                    unsigned short WTCSR;               /* WTCSR        */
                    } WRITE;                            /*              */
};                                                      /*              */
struct st_stb {                                         /* struct STB   */
              union {                                   /* STBCR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char STBY :1;       /*    STBY      */
                           unsigned char      :2;       /*              */
                           unsigned char _DMAC:1;       /*    DMAC      */
                           unsigned char      :1;       /*              */
                           unsigned char _TMU :1;       /*    TMU       */
                           }      BIT;                  /*              */
                    }           CR;                     /*              */
              char              wk[11];                 /*              */
              union {                                   /* STBCR2       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char DSLP:1;        /*    DSLP      */
                           unsigned char STHZ:1;        /*    STHZ      */
                           unsigned char     :5;        /*              */
                           unsigned char _SQ :1;        /*    SQ        */
                           unsigned char _UBC:1;        /*    UBC       */
                           }      BIT;                  /*              */
                    }           CR2;                    /*              */
};                                                      /*              */
struct st_clk {                                         /* struct CLK   */
              union {                                   /* CLKSTP00     */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int _SSI1  :1;      /*    SSI1      */
                           unsigned int _SSI0  :1;      /*    SSI0      */
                           unsigned int _SCIF2 :1;      /*    SCIF2     */
                           unsigned int _SCIF1 :1;      /*    SCIF1     */
                           unsigned int _SCIF0 :1;      /*    SCIF0     */
                           unsigned int _IIC1  :1;      /*    IIC1      */
                           unsigned int _IIC0  :1;      /*    IIC0      */
                           unsigned int _SIM   :1;      /*    SIM       */
                           unsigned int _MFI   :1;      /*    MFI       */
                           unsigned int _HSPI  :1;      /*    HSPI      */
                           unsigned int        :1;      /*              */
                           unsigned int _LCDC  :1;      /*    LCDC      */
                           unsigned int _USB   :1;      /*    USB       */
                           unsigned int        :1;      /*              */
                           unsigned int _CMT   :1;      /*    CMT       */
                           unsigned int _HAC1  :1;      /*    HAC1      */
                           unsigned int _HAC0  :1;      /*    HAC0      */
                           unsigned int _ADC   :1;      /*    ADC       */
                           unsigned int _HCAN21:1;      /*    HCAN21    */
                           unsigned int _HCAN20:1;      /*    HCAN20    */
                           unsigned int _DMABRG:1;      /*    DMABRG    */
                           unsigned int _GPIO  :1;      /*    GPIO      */
                           unsigned int _MMCIF :1;      /*    MMCIF     */
                           unsigned int _DMAC  :1;      /*    DMAC      */
                           unsigned int        :7;      /*              */
                           unsigned int _INTC  :1;      /*    INTC      */
                           }      BIT;                  /*              */
                    }           STP00;                  /*              */
              char              wk[12];                 /*              */
              union {                                   /* CLKSTPCLR00  */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int _SSI1  :1;      /*    SSI1      */
                           unsigned int _SSI0  :1;      /*    SSI0      */
                           unsigned int _SCIF2 :1;      /*    SCIF2     */
                           unsigned int _SCIF1 :1;      /*    SCIF1     */
                           unsigned int _SCIF0 :1;      /*    SCIF0     */
                           unsigned int _IIC1  :1;      /*    IIC1      */
                           unsigned int _IIC0  :1;      /*    IIC0      */
                           unsigned int _SIM   :1;      /*    SIM       */
                           unsigned int _MFI   :1;      /*    MFI       */
                           unsigned int _HSPI  :1;      /*    HSPI      */
                           unsigned int        :1;      /*              */
                           unsigned int _LCDC  :1;      /*    LCDC      */
                           unsigned int _USB   :1;      /*    USB       */
                           unsigned int        :1;      /*              */
                           unsigned int _CMT   :1;      /*    CMT       */
                           unsigned int _HAC1  :1;      /*    HAC1      */
                           unsigned int _HAC0  :1;      /*    HAC0      */
                           unsigned int _ADC   :1;      /*    ADC       */
                           unsigned int _HCAN21:1;      /*    HCAN21    */
                           unsigned int _HCAN20:1;      /*    HCAN20    */
                           unsigned int _DMABRG:1;      /*    DMABRG    */
                           unsigned int _GPIO  :1;      /*    GPIO      */
                           unsigned int _MMCIF :1;      /*    MMCIF     */
                           unsigned int _DMAC  :1;      /*    DMAC      */
                           unsigned int        :7;      /*              */
                           unsigned int _INTC  :1;      /*    INTC      */
                           }      BIT;                  /*              */
                    }           STPCLR00;               /*              */
};                                                      /*              */
struct st_tmu  {                                        /* struct TMU   */
               union {                                  /* TSTR         */
                     unsigned char  BYTE;               /*  Byte Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned char STR0:1;       /*    STR0      */
                            unsigned char STR1:1;       /*    STR1      */
                            unsigned char STR2:1;       /*    STR2      */
                            unsigned char     :5;       /*              */
  #else
                            unsigned char     :5;       /*              */
                            unsigned char STR2:1;       /*    STR2      */
                            unsigned char STR1:1;       /*    STR1      */
                            unsigned char STR0:1;       /*    STR0      */
  #endif
                            }       BIT;                /*              */
                     }          TSTR;                   /*              */
};                                                      /*              */
struct st_tmu0 {                                        /* struct TMU0  */
               unsigned int     TCOR;                   /* TCOR         */
               unsigned int     TCNT;                   /* TCNT         */
               union {                                  /* TCR          */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short TPSC:3;      /*    TPSC      */
                            unsigned short CKEG:2;      /*    CKEG      */
                            unsigned short UNIE:1;      /*    UNIE      */
                            unsigned short     :2;      /*              */
                            unsigned short UNF :1;      /*    UNF       */
                            unsigned short     :7;      /*              */
  #else
                            unsigned short     :7;      /*              */
                            unsigned short UNF :1;      /*    UNF       */
                            unsigned short     :2;      /*              */
                            unsigned short UNIE:1;      /*    UNIE      */
                            unsigned short CKEG:2;      /*    CKEG      */
                            unsigned short TPSC:3;      /*    TPSC      */
  #endif
                            }       BIT;                /*              */
                     }          TCR;                    /*              */
};                                                      /*              */
struct st_tmu2 {                                        /* struct TMU2  */
               unsigned int     TCOR;                   /* TCOR         */
               unsigned int     TCNT;                   /* TCNT         */
               union {                                  /* TCR          */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short TPSC:3;      /*    TPSC      */
                            unsigned short CKEG:2;      /*    CKEG      */
                            unsigned short UNIE:1;      /*    UNIE      */
                            unsigned short ICPE:2;      /*    ICPE      */
                            unsigned short UNF :1;      /*    UNF       */
                            unsigned short ICPF:1;      /*    ICPF      */
                            unsigned short     :6;      /*              */
  #else
                            unsigned short     :6;      /*              */
                            unsigned short ICPF:1;      /*    ICPF      */
                            unsigned short UNF :1;      /*    UNF       */
                            unsigned short ICPE:2;      /*    ICPE      */
                            unsigned short UNIE:1;      /*    UNIE      */
                            unsigned short CKEG:2;      /*    CKEG      */
                            unsigned short TPSC:3;      /*    TPSC      */
  #endif
                            }       BIT;                /*              */
                     }          TCR;                    /*              */
               unsigned int     TCPR;                   /* TCPR         */
};                                                      /*              */
struct st_cmt {                                         /* struct CMT   */
              union {                                   /* CMTCFG       */
                    unsigned int LONG;                 /*  Long Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned int T01 : 2;       /*    T01       */
                           unsigned int T23 : 3;       /*    T23       */
                           unsigned int FRTM: 1;       /*    FRTM      */
                           unsigned int FRCM: 1;       /*    FRCM      */
                           unsigned int     : 1;       /*              */
                           unsigned int ED0 : 2;       /*    ED0       */
                           unsigned int ED1 : 2;       /*    ED1       */
                           unsigned int ED2 : 2;       /*    ED2       */
                           unsigned int ED3 : 2;       /*    ED3       */
                           unsigned int ROT0: 1;       /*    ROT0      */
                           unsigned int ROT2: 1;       /*    ROT2      */
                           unsigned int     :14;       /*              */
  #else
                           unsigned int     :14;       /*              */
                           unsigned int ROT2: 1;       /*    ROT2      */
                           unsigned int ROT0: 1;       /*    ROT0      */
                           unsigned int ED3 : 2;       /*    ED3       */
                           unsigned int ED2 : 2;       /*    ED2       */
                           unsigned int ED1 : 2;       /*    ED1       */
                           unsigned int ED0 : 2;       /*    ED0       */
                           unsigned int     : 1;       /*              */
                           unsigned int FRCM: 1;       /*    FRCM      */
                           unsigned int FRTM: 1;       /*    FRTM      */
                           unsigned int T23 : 3;       /*    T23       */
                           unsigned int T01 : 2;       /*    T01       */
  #endif
                           }       BIT;                 /*              */
                    }           CMTCFG;                 /*              */
              unsigned int     CMTFRT;                 /* CMTFRT       */
              union {                                   /* CMTCTL       */
                    unsigned int LONG;                 /*  Long Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned int OP0 :1;        /*    OP0       */
                           unsigned int OP1 :1;        /*    OP1       */
                           unsigned int OP2 :1;        /*    OP2       */
                           unsigned int OP3 :1;        /*    OP3       */
                           unsigned int SI0 :1;        /*    SI0       */
                           unsigned int SI1 :1;        /*    SI1       */
                           unsigned int SI2 :1;        /*    SI2       */
                           unsigned int SI3 :1;        /*    SI3       */
                           unsigned int CC0 :2;        /*    CC0       */
                           unsigned int CC1 :2;        /*    CC1       */
                           unsigned int CC2 :2;        /*    CC2       */
                           unsigned int CC3 :2;        /*    CC3       */
                           unsigned int IEE0:1;        /*    IEE0      */
                           unsigned int IEE1:1;        /*    IEE1      */
                           unsigned int IEE2:1;        /*    IEE2      */
                           unsigned int IEE3:1;        /*    IEE3      */
                           unsigned int ICE0:1;        /*    ICE0      */
                           unsigned int ICE1:1;        /*    ICE1      */
                           unsigned int ICE2:1;        /*    ICE2      */
                           unsigned int ICE3:1;        /*    ICE3      */
                           unsigned int IOE0:1;        /*    IOE0      */
                           unsigned int IOE1:1;        /*    IOE1      */
                           unsigned int IOE2:1;        /*    IOE2      */
                           unsigned int IOE3:1;        /*    IOE3      */
                           unsigned int TE0 :1;        /*    TE0       */
                           unsigned int TE1 :1;        /*    TE1       */
                           unsigned int TE2 :1;        /*    TE2       */
                           unsigned int TE3 :1;        /*    TE3       */
  #else
                           unsigned int TE3 :1;        /*    TE3       */
                           unsigned int TE2 :1;        /*    TE2       */
                           unsigned int TE1 :1;        /*    TE1       */
                           unsigned int TE0 :1;        /*    TE0       */
                           unsigned int IOE3:1;        /*    IOE3      */
                           unsigned int IOE2:1;        /*    IOE2      */
                           unsigned int IOE1:1;        /*    IOE1      */
                           unsigned int IOE0:1;        /*    IOE0      */
                           unsigned int ICE3:1;        /*    ICE3      */
                           unsigned int ICE2:1;        /*    ICE2      */
                           unsigned int ICE1:1;        /*    ICE1      */
                           unsigned int ICE0:1;        /*    ICE0      */
                           unsigned int IEE3:1;        /*    IEE3      */
                           unsigned int IEE2:1;        /*    IEE2      */
                           unsigned int IEE1:1;        /*    IEE1      */
                           unsigned int IEE0:1;        /*    IEE0      */
                           unsigned int CC3 :2;        /*    CC3       */
                           unsigned int CC2 :2;        /*    CC2       */
                           unsigned int CC1 :2;        /*    CC1       */
                           unsigned int CC0 :2;        /*    CC0       */
                           unsigned int SI3 :1;        /*    SI3       */
                           unsigned int SI2 :1;        /*    SI2       */
                           unsigned int SI1 :1;        /*    SI1       */
                           unsigned int SI0 :1;        /*    SI0       */
                           unsigned int OP3 :1;        /*    OP3       */
                           unsigned int OP2 :1;        /*    OP2       */
                           unsigned int OP1 :1;        /*    OP1       */
                           unsigned int OP0 :1;        /*    OP0       */
  #endif
                           }       BIT;                 /*              */
                    }           CMTCTL;                 /*              */
              union {                                   /* CMTIRQS      */
                    unsigned int LONG;                 /*  Long Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned int IE0: 1;        /*    IE0       */
                           unsigned int IE1: 1;        /*    IE1       */
                           unsigned int IE2: 1;        /*    IE2       */
                           unsigned int IE3: 1;        /*    IE3       */
                           unsigned int IC0: 1;        /*    IC0       */
                           unsigned int IC1: 1;        /*    IC1       */
                           unsigned int IC2: 1;        /*    IC2       */
                           unsigned int IC3: 1;        /*    IC3       */
                           unsigned int IO0: 1;        /*    IO0       */
                           unsigned int IO1: 1;        /*    IO1       */
                           unsigned int IO2: 1;        /*    IO2       */
                           unsigned int IO3: 1;        /*    IO3       */
                           unsigned int    :20;        /*              */
  #else
                           unsigned int    :20;        /*              */
                           unsigned int IO3: 1;        /*    IO3       */
                           unsigned int IO2: 1;        /*    IO2       */
                           unsigned int IO1: 1;        /*    IO1       */
                           unsigned int IO0: 1;        /*    IO0       */
                           unsigned int IC3: 1;        /*    IC3       */
                           unsigned int IC2: 1;        /*    IC2       */
                           unsigned int IC1: 1;        /*    IC1       */
                           unsigned int IC0: 1;        /*    IC0       */
                           unsigned int IE3: 1;        /*    IE3       */
                           unsigned int IE2: 1;        /*    IE2       */
                           unsigned int IE1: 1;        /*    IE1       */
                           unsigned int IE0: 1;        /*    IE0       */
  #endif
                           }       BIT;                 /*              */
                    }           CMTIRQS;                /*              */
};                                                      /*              */
struct st_cmt0 {                                        /* struct CMT0  */
               unsigned int     CMTCHT;                 /* CMTCHT       */
               char             wk1[12];                /*              */
               unsigned int     CMTCHST;                /* CMTCHST      */
               char             wk2[12];                /*              */
               unsigned int     CMTCHC;                 /* CMTCHC       */
};                                                      /*              */
struct st_scif {                                        /* struct SCIF  */
               union {                                  /* SCSMR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short CKS :2;      /*    CKS       */
                            unsigned short     :1;      /*              */
                            unsigned short STOP:1;      /*    STOP      */
                            unsigned short OE  :1;      /*    O/E       */
                            unsigned short PE  :1;      /*    PE        */
                            unsigned short CHR :1;      /*    CHR       */
                            unsigned short CA  :1;      /*    C/A       */
                            unsigned short     :8;      /*              */
  #else
                            unsigned short     :8;      /*              */
                            unsigned short CA  :1;      /*    C/A       */
                            unsigned short CHR :1;      /*    CHR       */
                            unsigned short PE  :1;      /*    PE        */
                            unsigned short OE  :1;      /*    O/E       */
                            unsigned short STOP:1;      /*    STOP      */
                            unsigned short     :1;      /*              */
                            unsigned short CKS :2;      /*    CKS       */
  #endif
                            }       BIT;                /*              */
                     }          SCSMR;                  /*              */
               char             wk1[2];                 /*              */
               unsigned char    SCBRR;                  /* SCBRR        */
               char             wk2[3];                 /*              */
               union {                                  /* SCSCR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short CKE :1;      /*    CKE       */
                            unsigned short     :1;      /*              */
                            unsigned short REIE:1;      /*    REIE      */
                            unsigned short RE  :1;      /*    RE        */
                            unsigned short TE  :1;      /*    TE        */
                            unsigned short RIE :1;      /*    RIE       */
                            unsigned short TIE :1;      /*    TIE       */
                            unsigned short     :8;      /*              */
  #else
                            unsigned short     :8;      /*              */
                            unsigned short TIE :1;      /*    TIE       */
                            unsigned short RIE :1;      /*    RIE       */
                            unsigned short TE  :1;      /*    TE        */
                            unsigned short RE  :1;      /*    RE        */
                            unsigned short REIE:1;      /*    REIE      */
                            unsigned short     :1;      /*              */
                            unsigned short CKE :1;      /*    CKE       */
  #endif
                            }      BIT;                 /*              */
                     }          SCSCR;                  /*              */
               char             wk3[2];                 /*              */
               unsigned char    SCFTDR;                 /* SCFTDR       */
               char             wk4[3];                 /*              */
               union {                                  /* SCFSR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short DR  :1;      /*    DR        */
                            unsigned short RDF :1;      /*    RDF       */
                            unsigned short PER :1;      /*    PER       */
                            unsigned short FER :1;      /*    FER       */
                            unsigned short BRK :1;      /*    BRK       */
                            unsigned short TDFE:1;      /*    TDFE      */
                            unsigned short TEND:1;      /*    TEND      */
                            unsigned short ER  :1;      /*    ER        */
                            unsigned short     :8;      /*    PERC      */
  #else
                            unsigned short     :8;      /*    PERC      */
                            unsigned short ER  :1;      /*    ER        */
                            unsigned short TEND:1;      /*    TEND      */
                            unsigned short TDFE:1;      /*    TDFE      */
                            unsigned short BRK :1;      /*    BRK       */
                            unsigned short FER :1;      /*    FER       */
                            unsigned short PER :1;      /*    PER       */
                            unsigned short RDF :1;      /*    RDF       */
                            unsigned short DR  :1;      /*    DR        */
  #endif
                            }       BIT;                /*              */
                     }          SCFSR;                  /*              */
               char             wk5[2];                 /*              */
               unsigned char    SCFRDR;                 /* SCFRDR       */
               char             wk6[3];                 /*              */
               union {                                  /* SCFCR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short LOOP :1;     /*    LOOP      */
                            unsigned short RFRST:1;     /*    RFRST     */
                            unsigned short TFRST:1;     /*    TFRST     */
                            unsigned short MCE  :1;     /*    MCE       */
                            unsigned short TTRG :2;     /*    TTRG      */
                            unsigned short RTRG :2;     /*    RTRG      */
                            unsigned short RSTRG:3;     /*    RSTRG     */
                            unsigned short      :5;     /*              */
  #else
                            unsigned short      :5;     /*              */
                            unsigned short RSTRG:3;     /*    RSTRG     */
                            unsigned short RTRG :2;     /*    RTRG      */
                            unsigned short TTRG :2;     /*    TTRG      */
                            unsigned short MCE  :1;     /*    MCE       */
                            unsigned short TFRST:1;     /*    TFRST     */
                            unsigned short RFRST:1;     /*    RFRST     */
                            unsigned short LOOP :1;     /*    LOOP      */
  #endif
                            }       BIT;                /*              */
                     }          SCFCR;                  /*              */
               char             wk7[2];                 /*              */
               union {                                  /* SCTFDR       */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short T:8;         /*    T         */
                            unsigned short  :8;         /*              */
  #else
                            unsigned short  :8;         /*              */
                            unsigned short T:8;         /*    T         */
  #endif
                            }       BIT;                /*              */
                     }          SCTFDR;                 /*              */
               char             wk8[2];                 /*              */
               union {                                  /* SCRFDR       */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short R:8;         /*    R         */
                            unsigned short  :8;         /*              */
  #else
                            unsigned short  :8;         /*              */
                            unsigned short R:8;         /*    R         */
  #endif
                            }       BIT;                /*              */
                     }          SCRFDR;                 /*              */
               char             wk9[2];                 /*              */
               union {                                  /* SCSPTR       */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short SPB2DT:1;    /*    SPB2DT    */
                            unsigned short SPB2IO:1;    /*    SPB2IO    */
                            unsigned short SCKDT :1;    /*    SCKDT     */
                            unsigned short SCKIO :1;    /*    SCKIO     */
                            unsigned short CTSDT :1;    /*    CTSDT     */
                            unsigned short CTSIO :1;    /*    CTSIO     */
                            unsigned short RTSDT :1;    /*    RTSDT     */
                            unsigned short RTSIO :1;    /*    RTSIO     */
                            unsigned short       :8;    /*              */
  #else
                            unsigned short       :8;    /*              */
                            unsigned short RTSIO :1;    /*    RTSIO     */
                            unsigned short RTSDT :1;    /*    RTSDT     */
                            unsigned short CTSIO :1;    /*    CTSIO     */
                            unsigned short CTSDT :1;    /*    CTSDT     */
                            unsigned short SCKIO :1;    /*    SCKIO     */
                            unsigned short SCKDT :1;    /*    SCKDT     */
                            unsigned short SPB2IO:1;    /*    SPB2IO    */
                            unsigned short SPB2DT:1;    /*    SPB2DT    */
  #endif
                            }       BIT;                /*              */
                     }          SCSPTR;                 /*              */
               char             wk10[2];                /*              */
               union {                                  /* SCLSR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short ORER: 1;     /*    ORER      */
                            unsigned short     :15;     /*              */
  #else
                            unsigned short     :15;     /*              */
                            unsigned short ORER: 1;     /*    ORER      */
  #endif
                            }       BIT;                /*              */
                     }          SCLSR;                  /*              */
               char             wk11[2];                /*              */
               union {                                  /* SCRER        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                            unsigned short FER:8;       /*    FER       */
                            unsigned short PER:8;       /*    PER       */
  #else
                            unsigned short PER:8;       /*    PER       */
                            unsigned short FER:8;       /*    FER       */
  #endif
                            }       BIT;                /*              */
                     }          SCRER;                  /*              */
};                                                      /*              */
struct st_sim {                                         /* struct SIM   */
              union {                                   /* SISMR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char   :2;          /*              */
                           unsigned char PE:1;          /*    PE        */
                           unsigned char OE:1;          /*    O/E       */
                           }      BIT;                  /*              */
                    }           SISMR;                  /*              */
              char              wk1;                    /*              */
              unsigned char     SIBRR;                  /* SIBRR        */
              unsigned char     wk2;                    /*              */
              union {                                   /* SISCR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char TIE    :1;     /*    TIE       */
                           unsigned char RIE    :1;     /*    RIE       */
                           unsigned char TE     :1;     /*    TE        */
                           unsigned char RE     :1;     /*    RE        */
                           unsigned char WAIT_IE:1;     /*    WAIT_IE   */
                           unsigned char TEIE   :1;     /*    TEIE      */
                           unsigned char CKE    :2;     /*    CKE       */
                           }      BIT;                  /*              */
                    }           SISCR;                  /*              */
              char              wk3;                    /*              */
              unsigned char     SITDR;                  /* SITDR        */
              char              wk4;                    /*              */
              union {                                   /* SISSR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char TDRE   :1;     /*    TDRE      */
                           unsigned char RDRF   :1;     /*    RDRF      */
                           unsigned char ORER   :1;     /*    ORER      */
                           unsigned char ERS    :1;     /*    ERS       */
                           unsigned char PER    :1;     /*    PER       */
                           unsigned char TEND   :1;     /*    TEND      */
                           unsigned char WAIT_ER:1;     /*    WAIT_ER   */
                           }      BIT;                  /*              */
                    }           SISSR;                  /*              */
              char              wk5;                    /*              */
              unsigned char     SIRDR;                  /* SIRDR        */
              char              wk6;                    /*              */
              union {                                   /* SISCMR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char     :1;        /*              */
                           unsigned char LCB :1;        /*    LCB       */
                           unsigned char PB  :1;        /*    PB        */
                           unsigned char     :1;        /*              */
                           unsigned char SDIR:1;        /*    SDIR      */
                           unsigned char SINV:1;        /*    SINV      */
                           unsigned char RST :1;        /*    RST       */
                           unsigned char SMIF:1;        /*    SMIF      */
                           }      BIT;                  /*              */
                    }           SISCMR;                 /*              */
              char              wk7;                    /*              */
              union {                                   /* SISC2R       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char EIO:1;         /*    EIO       */
                           }      BIT;                  /*              */
                    }           SISC2R;                 /*              */
              char              wk8;                    /*              */
              unsigned short    SIWAIT;                 /* SIWAIT       */
              unsigned char     SIGRD;                  /* SIGRD        */
              char              wk9;                    /*              */
              unsigned short    SISMPL;                 /* SISMPL       */
};                                                      /*              */
struct st_iic {                                         /* struct IIC   */
              union {                                   /* ICSCR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :28;        /*              */
                           unsigned int SDBS: 1;        /*    SDBS      */
                           unsigned int SIE : 1;        /*    SIE       */
                           unsigned int GCAE: 1;        /*    GCAE      */
                           unsigned int FNA : 1;        /*    FNA       */
                           }     BIT;                   /*              */
                    }           ICSCR;                  /*              */
              union {                                   /* ICMCR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :24;        /*              */
                           unsigned int MDBS: 1;        /*    MDBS      */
                           unsigned int FSCL: 1;        /*    FSCL      */
                           unsigned int FSDA: 1;        /*    FSDA      */
                           unsigned int OBPC: 1;        /*    OBPC      */
                           unsigned int MIE : 1;        /*    MIE       */
                           unsigned int TSBE: 1;        /*    TSBE      */
                           unsigned int FSB : 1;        /*    FSB       */
                           unsigned int ESG : 1;        /*    ESG       */
                           }     BIT;                   /*              */
                    }           ICMCR;                  /*              */
              union {                                   /* ICSSR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :25;        /*              */
                           unsigned int GCAR: 1;        /*    GCAR      */
                           unsigned int STM : 1;        /*    STM       */
                           unsigned int SSR : 1;        /*    SSR       */
                           unsigned int SDE : 1;        /*    SDE       */
                           unsigned int SDT : 1;        /*    SDT       */
                           unsigned int SDR : 1;        /*    SDR       */
                           unsigned int SAR : 1;        /*    SAR       */
                           }     BIT;                   /*              */
                    }           ICSSR;                  /*              */
              union {                                   /* ICMSR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int    :25;         /*              */
                           unsigned int MNR: 1;         /*    MNR       */
                           unsigned int MAL: 1;         /*    MAL       */
                           unsigned int MST: 1;         /*    MST       */
                           unsigned int MDE: 1;         /*    MDE       */
                           unsigned int MDT: 1;         /*    MDT       */
                           unsigned int MDR: 1;         /*    MDR       */
                           unsigned int MAT: 1;         /*    MAT       */
                           }     BIT;                   /*              */
                    }           ICMSR;                  /*              */
              union {                                   /* ICSIER       */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :27;        /*              */
                           unsigned int SSRE: 1;        /*    SSRE      */
                           unsigned int SDEE: 1;        /*    SDEE      */
                           unsigned int SDTE: 1;        /*    SDTE      */
                           unsigned int SDRE: 1;        /*    SDRE      */
                           unsigned int SARE: 1;        /*    SARE      */
                           }     BIT;                   /*              */
                    }           ICSIER;                 /*              */
              union {                                   /* ICMIER       */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :25;        /*              */
                           unsigned int MNRE: 1;        /*    MNRE      */
                           unsigned int MALE: 1;        /*    MALE      */
                           unsigned int MSTE: 1;        /*    MSTE      */
                           unsigned int MDEE: 1;        /*    MDEE      */
                           unsigned int MDTE: 1;        /*    MDTE      */
                           unsigned int MDRE: 1;        /*    MDRE      */
                           unsigned int MATE: 1;        /*    MATE      */
                           }     BIT;                   /*              */
                    }           ICMIER;                 /*              */
              union {                                   /* ICCCR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :24;        /*              */
                           unsigned int SCGD: 6;        /*    SCGD      */
                           unsigned int CDF : 2;        /*    CDF       */
                           }     BIT;                   /*              */
                    }           ICCCR;                  /*              */
              union {                                   /* ICSAR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int      :25;       /*              */
                           unsigned int SADD0: 7;       /*    SADD0     */
                           }     BIT;                   /*              */
                    }           ICSAR;                  /*              */
              union {                                   /* ICMAR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int      :24;       /*              */
                           unsigned int SADD1: 7;       /*   SADD1      */
                           unsigned int STM1 : 1;       /*   STM1       */
                           }     BIT;                   /*              */
                    }           ICMAR;                  /*              */
              union {                                   /*              */
                    union {                             /* ICRXD        */
                          unsigned int LONG;            /*  Long Access */
                          struct {                      /*  Bit  Access */
                                 unsigned int    :24;   /*              */
                                 unsigned int RXD: 8;   /*    RXD       */
                                 }     BIT;             /*              */
                          }         REG_ICRXD;          /*              */
                    union {                             /* ICTXD        */
                          unsigned int LONG;            /*  Long Access */
                          struct {                      /*  Bit  Access */
                                 unsigned int    :24;   /*              */
                                 unsigned int TXD: 8;   /*    TXD       */
                                 }     BIT;             /*              */
                          }         REG_ICTXD;          /*              */
                    }           UN_IIC;                 /*              */
              union {                                   /* ICFCR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int      :24;       /*              */
                           unsigned int RTRG : 4;       /*    RTRG      */
                           unsigned int TTRG : 2;       /*    TTRG      */
                           unsigned int RFRST: 1;       /*    RFRST     */
                           unsigned int TFRST: 1;       /*    TFRST     */
                           }     BIT;                   /*              */
                    }           ICFCR;                  /*              */
              union {                                   /* ICFSR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :29;        /*              */
                           unsigned int TEND: 1;        /*    TEND      */
                           unsigned int RDF : 1;        /*    RDF       */
                           unsigned int TDFE: 1;        /*    TDFE      */
                           }     BIT;                   /*              */
                    }           ICFSR;                  /*              */
              union {                                   /* ICFIER       */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :29;        /*              */
                           unsigned int TEIE: 1;        /*   TEIE       */
                           unsigned int RXIE: 1;        /*   RXIE       */
                           unsigned int TXIE: 1;        /*   TXIE       */
                           }     BIT;                   /*              */
                    }           ICFIER;                 /*              */
              union {                                   /* ICRFDR       */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int  :27;           /*              */
                           unsigned int R: 5;           /*   R          */
                           }     BIT;                   /*              */
                    }           ICRFDR;                 /*              */
              union {                                   /* ICTFDR       */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int  :27;           /*              */
                           unsigned int T: 5;           /*   T          */
                           }     BIT;                   /*              */
                    }           ICTFDR;                 /*              */
};                                                      /*              */
struct st_ssi {                                         /* struct SSI   */
              union {                                   /* SSICR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :3;         /*              */
                           unsigned int DMEN:1;         /*    DMEN      */
                           unsigned int UIEN:1;         /*    UIEN      */
                           unsigned int OIEN:1;         /*    OIEN      */
                           unsigned int IIEN:1;         /*    IIEN      */
                           unsigned int DIEN:1;         /*    DIEN      */
                           unsigned int CHNL:2;         /*    CHNL      */
                           unsigned int DWL :3;         /*    DWL       */
                           unsigned int SWL :3;         /*    SWL       */
                           unsigned int SCKD:1;         /*    SCKD      */
                           unsigned int SWSD:1;         /*    SWSD      */
                           unsigned int SCKP:1;         /*    SCKP      */
                           unsigned int SWSP:1;         /*    SWSP      */
                           unsigned int SPDP:1;         /*    SPDP      */
                           unsigned int SDTA:1;         /*    SDTA      */
                           unsigned int PDTA:1;         /*    PDTA      */
                           unsigned int DEL :1;         /*    DEL       */
                           unsigned int BREN:1;         /*    BREN      */
                           unsigned int CKDV:3;         /*    CKDV      */
                           unsigned int MUEN:1;         /*    MUEN      */
                           unsigned int CPEN:1;         /*    CPEN      */
                           unsigned int TRMD:1;         /*    TRMD      */
                           unsigned int EN  :1;         /*    EN        */
                           }     BIT;                   /*              */
                    }           SSICR;                  /*              */
              union {                                   /* SSISR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     : 3;        /*              */
                           unsigned int DMRQ: 1;        /*    DMRQ      */
                           unsigned int UIRQ: 1;        /*    UIRQ      */
                           unsigned int OIRQ: 1;        /*    OIRQ      */
                           unsigned int IIRQ: 1;        /*    IIRQ      */
                           unsigned int DIRQ: 1;        /*    DIRQ      */
                           unsigned int     :20;        /*              */
                           unsigned int CHNO: 2;        /*    CHNO      */
                           unsigned int SWNO: 1;        /*    SWNO      */
                           unsigned int IDST: 1;        /*    IDST      */
                           }     BIT;                   /*              */
                    }           SSISR;                  /*              */
              unsigned int      SSITDR;                 /* SSITDR       */
              unsigned int      SSIRDR;                 /* SSIRDR       */
};                                                      /*              */
struct st_usb {                                         /* struct USB   */
              union {                                   /* HR           */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int    :24;         /*              */
                           unsigned int REV: 8;         /*    REV       */
                           }     BIT;                   /*              */
                    }           HR;                     /*              */
              union {                                   /* HC           */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :21;        /*              */
                           unsigned int RWE : 1;        /*    RWE       */
                           unsigned int RWC : 1;        /*    RWC       */
                           unsigned int IR  : 1;        /*    IR        */
                           unsigned int HCFS: 2;        /*    HCFS      */
                           unsigned int BLE : 1;        /*    BLE       */
                           unsigned int CLE : 1;        /*    CLE       */
                           unsigned int IE  : 1;        /*    IE        */
                           unsigned int PLE : 1;        /*    PLE       */
                           unsigned int CBSR: 2;        /*    CBSR      */
                           }     BIT;                   /*              */
                    }           HC;                     /*              */
              union {                                   /* HCS          */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int    :14;         /*              */
                           unsigned int SOC: 2;         /*    SOC       */
                           unsigned int    :12;         /*              */
                           unsigned int OCR: 1;         /*    OCR       */
                           unsigned int BLF: 1;         /*    BLF       */
                           unsigned int CLF: 1;         /*    CLF       */
                           unsigned int HCR: 1;         /*    HCR       */
                           }     BIT;                   /*              */
                    }           HCS;                    /*              */
              union {                                   /* HIS          */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     : 1;        /*              */
                           unsigned int OC  : 1;        /*   OC         */
                           unsigned int     :23;        /*              */
                           unsigned int RHSC: 1;        /*   RHSC       */
                           unsigned int FNO : 1;        /*   FNO        */
                           unsigned int UE  : 1;        /*   UE         */
                           unsigned int RD  : 1;        /*   RD         */
                           unsigned int SF  : 1;        /*   SF         */
                           unsigned int WDH : 1;        /*   WDH        */
                           unsigned int SO  : 1;        /*   SO         */
                           }     BIT;                   /*              */
                    }           HIS;                    /*              */
              union {                                   /* HIE          */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int MIE : 1;        /*    MIE       */
                           unsigned int OC  : 1;        /*    OC        */
                           unsigned int     :23;        /*              */
                           unsigned int RHSC: 1;        /*    RHSC      */
                           unsigned int FNO : 1;        /*    FNO       */
                           unsigned int UE  : 1;        /*    UE        */
                           unsigned int RD  : 1;        /*    RD        */
                           unsigned int SF  : 1;        /*    SF        */
                           unsigned int WDH : 1;        /*    WDH       */
                           unsigned int SO  : 1;        /*    SO        */
                           }     BIT;                   /*              */
                    }           HIE;                    /*              */
              union {                                   /* HID          */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int MIE : 1;        /*    MIE       */
                           unsigned int OC  : 1;        /*    OC        */
                           unsigned int     :23;        /*              */
                           unsigned int RHSC: 1;        /*    RHSC      */
                           unsigned int FNO : 1;        /*    FNO       */
                           unsigned int UE  : 1;        /*    UE        */
                           unsigned int RD  : 1;        /*    RD        */
                           unsigned int SF  : 1;        /*    SF        */
                           unsigned int WDH : 1;        /*    WDH       */
                           unsigned int SO  : 1;        /*    SO        */
                           }     BIT;                   /*              */
                    }           HID;                    /*              */
              union {                                   /* HHCCA        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int HCCA:24;        /*    HCCA      */
                           }     BIT;                   /*              */
                    }           HHCCA;                  /*              */
              union {                                   /* HPCED        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int PCED:28;        /*    PCED      */
                           }     BIT;                   /*              */
                    }           HPCED;                  /*              */
              union {                                   /* HCHED        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int CHED:28;        /*    CHED      */
                           }     BIT;                   /*              */
                    }           HCHED;                  /*              */
              union {                                   /* HCCED        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int CCED:28;        /*    CCED      */
                           }     BIT;                   /*              */
                    }           HCCED;                  /*              */
              union {                                   /* HBHED        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int BHED:28;        /*    BHED      */
                           }     BIT;                   /*              */
                    }           HBHED;                  /*              */
              union {                                   /* HBCED        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int BCED:28;        /*    BCED      */
                           }     BIT;                   /*              */
                    }           HBCED;                  /*              */
              union {                                   /* HDH          */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int DH:28;          /*    DH        */
                           }     BIT;                   /*              */
                    }           HDH;                    /*              */
              union {                                   /* HFI          */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int FIT  : 1;       /*    FIT       */
                           unsigned int FSMPS:15;       /*    FSMPS     */
                           unsigned int      : 2;       /*              */
                           unsigned int FI   :14;       /*    FI        */
                           }     BIT;                   /*              */
                    }           HFI;                    /*              */
              union {                                   /* HFR          */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int FRT: 1;         /*    FRT       */
                           unsigned int    :17;         /*              */
                           unsigned int FR :14;         /*    FR        */
                           }     BIT;                   /*              */
                    }           HFR;                    /*              */
              union {                                   /* HFN          */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int   :16;          /*              */
                           unsigned int FN:16;          /*    FN        */
                           }     BIT;                   /*              */
                    }           HFN;                    /*              */
              union {                                   /* HPS          */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int   :18;          /*              */
                           unsigned int PS:14;          /*    PS        */
                           }     BIT;                   /*              */
                    }           HPS;                    /*              */
              union {                                   /* HLST         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int    :20;         /*              */
                           unsigned int LST:12;         /*    LST       */
                           }     BIT;                   /*              */
                    }           HLST;                   /*              */
              union {                                   /* HRDA         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int POTPGT: 8;      /*    POTPGT    */
                           unsigned int       :11;      /*              */
                           unsigned int NOCP  : 1;      /*    NOCP      */
                           unsigned int OCPM  : 1;      /*    OCPM      */
                           unsigned int DT    : 1;      /*    DT        */
                           unsigned int NPS   : 1;      /*    NPS       */
                           unsigned int PSM   : 1;      /*    PSM       */
                           unsigned int NDP   : 8;      /*    NDP       */
                           }     BIT;                   /*              */
                    }           HRDA;                   /*              */
              union {                                   /* HRDB         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int PPCM:16;        /*    PPCM      */
                           unsigned int DR  :16;        /*    DR        */
                           }     BIT;                   /*              */
                    }           HRDB;                   /*              */
              union {                                   /* HRS          */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int CRWE: 1;        /*    CRWE      */
                           unsigned int     :13;        /*              */
                           unsigned int OCIC: 1;        /*    OCIC      */
                           unsigned int LPSC: 1;        /*    LPSC      */
                           unsigned int DRWE: 1;        /*    DRWE      */
                           unsigned int     :13;        /*              */
                           unsigned int OCI :1;         /*    OCI       */
                           unsigned int LPS :1;         /*    LPS       */
                           }     BIT;                   /*              */
                    }           HRS;                    /*              */
              union {                                   /* HRPS1        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :11;        /*              */
                           unsigned int PRSC: 1;        /*    PRSC      */
                           unsigned int OCIC: 1;        /*    OCIC      */
                           unsigned int PSSC: 1;        /*    PSSC      */
                           unsigned int PESC: 1;        /*    PESC      */
                           unsigned int CSC : 1;        /*    CSC       */
                           unsigned int     : 6;        /*              */
                           unsigned int LSDA: 1;        /*    LSDA      */
                           unsigned int PPS : 1;        /*    PPS       */
                           unsigned int     : 3;        /*              */
                           unsigned int PRS : 1;        /*    PRS       */
                           unsigned int POCI: 1;        /*    POCI      */
                           unsigned int PSS : 1;        /*    PSS       */
                           unsigned int PES : 1;        /*    PES       */
                           unsigned int CCS : 1;        /*    CCS       */
                           }     BIT;                   /*              */
                    }           HRPS1;                  /*              */
};                                                      /*              */
union un_mb3116 {                                       /* MB31-MB16    */
                unsigned short WORD;                    /*  Word Access */
                struct {                                /*  Bit  Access */
                       unsigned short MB31:1;           /*    MB31      */
                       unsigned short MB30:1;           /*    MB30      */
                       unsigned short MB29:1;           /*    MB29      */
                       unsigned short MB28:1;           /*    MB28      */
                       unsigned short MB27:1;           /*    MB27      */
                       unsigned short MB26:1;           /*    MB26      */
                       unsigned short MB25:1;           /*    MB25      */
                       unsigned short MB24:1;           /*    MB24      */
                       unsigned short MB23:1;           /*    MB23      */
                       unsigned short MB22:1;           /*    MB22      */
                       unsigned short MB21:1;           /*    MB21      */
                       unsigned short MB20:1;           /*    MB20      */
                       unsigned short MB19:1;           /*    MB19      */
                       unsigned short MB18:1;           /*    MB18      */
                       unsigned short MB17:1;           /*    MB17      */
                       unsigned short MB16:1;           /*    MB16      */
                      }     BIT;                        /*              */
};                                                      /*              */
union un_mb15_0 {                                       /* MB15-MB0     */
                unsigned short WORD;                    /*  Word Access */
                struct {                                /*  Bit  Access */
                       unsigned short MB15:1;           /*    MB15      */
                       unsigned short MB14:1;           /*    MB14      */
                       unsigned short MB13:1;           /*    MB13      */
                       unsigned short MB12:1;           /*    MB12      */
                       unsigned short MB11:1;           /*    MB11      */
                       unsigned short MB10:1;           /*    MB10      */
                       unsigned short MB9 :1;           /*    MB9       */
                       unsigned short MB8 :1;           /*    MB8       */
                       unsigned short MB7 :1;           /*    MB7       */
                       unsigned short MB6 :1;           /*    MB6       */
                       unsigned short MB5 :1;           /*    MB5       */
                       unsigned short MB4 :1;           /*    MB4       */
                       unsigned short MB3 :1;           /*    MB3       */
                       unsigned short MB2 :1;           /*    MB2       */
                       unsigned short MB1 :1;           /*    MB1       */
                       unsigned short MB0 :1;           /*    MB0       */
                       }     BIT;                       /*              */
};                                                      /*              */
union un_mb15_1 {                                       /* MB15-MB1     */
                unsigned short WORD;                    /*  Word Access */
                struct {                                /*  Bit  Access */
                       unsigned short MB15:1;           /*    MB15      */
                       unsigned short MB14:1;           /*    MB14      */
                       unsigned short MB13:1;           /*    MB13      */
                       unsigned short MB12:1;           /*    MB12      */
                       unsigned short MB11:1;           /*    MB11      */
                       unsigned short MB10:1;           /*    MB10      */
                       unsigned short MB9 :1;           /*    MB9       */
                       unsigned short MB8 :1;           /*    MB8       */
                       unsigned short MB7 :1;           /*    MB7       */
                       unsigned short MB6 :1;           /*    MB6       */
                       unsigned short MB5 :1;           /*    MB5       */
                       unsigned short MB4 :1;           /*    MB4       */
                       unsigned short MB3 :1;           /*    MB3       */
                       unsigned short MB2 :1;           /*    MB2       */
                       unsigned short MB1 :1;           /*    MB1       */
                       }     BIT;                       /*              */
};                                                      /*              */
struct st_hcan2 {                                       /* struct HCAN2 */
                union {                                 /* CANMCR       */
                      unsigned short WORD;              /*  Word Access */
                      struct {                          /*  Bit  Access */
                             unsigned short TSTM  :1;   /*    TSTM      */
                             unsigned short WCANEC:1;   /*    WCANEC    */
                             unsigned short FEP   :1;   /*    FEP       */
                             unsigned short AAM   :1;   /*    AAM       */
                             unsigned short DEC   :1;   /*    DEC       */
                             unsigned short DRXI  :1;   /*    DRXI      */
                             unsigned short DTXO  :1;   /*    DTXO      */
                             unsigned short EIL   :1;   /*    EIL       */
                             unsigned short AWM   :1;   /*    AWM       */
                             unsigned short       :1;   /*              */
                             unsigned short SLPM  :1;   /*    SLPM      */
                             unsigned short       :2;   /*              */
                             unsigned short MTP   :1;   /*    MTP       */
                             unsigned short HLTRQ :1;   /*    HLTRQ     */
                             unsigned short RSTRQ :1;   /*    RSTRQ     */
                             }       BIT;               /*              */
                      }         CANMCR;                 /*              */
                union {                                 /* CANGSR       */
                      unsigned short WORD;              /*  Word Access */
                      struct {                          /*  Bit  Access */
                             unsigned short     :10;    /*              */
                             unsigned short EPS : 1;    /*    EPS       */
                             unsigned short HSS : 1;    /*    HSS       */
                             unsigned short RS  : 1;    /*    RS        */
                             unsigned short MTCF: 1;    /*    MTCF      */
                             unsigned short TRWF: 1;    /*    TRWF      */
                             unsigned short BOF : 1;    /*    BOF       */
                             }       BIT;               /*              */
                      }         CANGSR;                 /*              */
                union {                                 /* CANBCR1      */
                      unsigned short WORD;              /*  Word Access */
                      struct {                          /*  Bit  Access */
                             unsigned short TSEG1:4;    /*    TSEG1     */
                             unsigned short      :1;    /*              */
                             unsigned short TSEG2:3;    /*    TSEG2     */
                             unsigned short      :2;    /*              */
                             unsigned short SJW  :2;    /*    SJW       */
                             unsigned short      :2;    /*              */
                             unsigned short EG   :1;    /*    EG        */
                             unsigned short BSP  :1;    /*    BSP       */
                             }       BIT;               /*              */
                      }         CANBCR1;                /*              */
                union {                                 /* CANBCR0      */
                      unsigned short WORD;              /*  Word Access */
                      struct {                          /*  Bit  Access */
                             unsigned short    :8;      /*              */
                             unsigned short BRP:8;      /*    BRP       */
                             }       BIT;               /*              */
                      }         CANBCR0;                /*              */
                union {                                 /* CANIRR       */
                      unsigned short WORD;              /*  Word Access */
                      struct {                          /*  Bit  Access */
                             unsigned short      :1;    /*              */
                             unsigned short TCMIF:1;    /*    TCMIF     */
                             unsigned short TOVIF:1;    /*    TOVIF     */
                             unsigned short WBAIF:1;    /*    WBAIF     */
                             unsigned short      :2;    /*              */
                             unsigned short MOVIF:1;    /*    MOVIF     */
                             unsigned short MBEIF:1;    /*    MBEIF     */
                             unsigned short OLFIF:1;    /*    OLFIF     */
                             unsigned short BOFIF:1;    /*    BOFIF     */
                             unsigned short ERPIF:1;    /*    ERPIF     */
                             unsigned short ROWIF:1;    /*    ROWIF     */
                             unsigned short TOWIF:1;    /*    TOWIF     */
                             unsigned short RFRIF:1;    /*    RFRIF     */
                             unsigned short DFRIF:1;    /*    DFRIF     */
                             unsigned short RHSIF:1;    /*    RHSIF     */
                             }       BIT;               /*              */
                      }         CANIRR;                 /*              */
                union {                                 /* CANIMR       */
                      unsigned short WORD;              /*  Word Access */
                      struct {                          /*  Bit  Access */
                             unsigned short      :1;    /*              */
                             unsigned short TCMIM:1;    /*    TCMIM     */
                             unsigned short TOVIM:1;    /*    TOVIM     */
                             unsigned short WBAIM:1;    /*    WBAIM     */
                             unsigned short      :2;    /*              */
                             unsigned short MOVIM:1;    /*    MOVIM     */
                             unsigned short MBEIM:1;    /*    MBEIM     */
                             unsigned short OLFIM:1;    /*    OLFIM     */
                             unsigned short BOFIM:1;    /*    BOFIM     */
                             unsigned short ERPIM:1;    /*    ERPIM     */
                             unsigned short ROWIM:1;    /*    ROWIM     */
                             unsigned short TOWIM:1;    /*    TOWIM     */
                             unsigned short RFRIM:1;    /*    RFRIM     */
                             unsigned short DFRIM:1;    /*    DFRIM     */
                             unsigned short RHSIM:1;    /*    RHSIM     */
                             }       BIT;               /*              */
                      }         CANIMR;                 /*              */
                union {                                 /* CANTECREC    */
                      unsigned short WORD;              /*  Word Access */
                      struct {                          /*  Bit  Access */
                             unsigned short TEC:8;      /*    TEC       */
                             unsigned short REC:8;      /*    REC       */
                             }       BIT;               /*              */
                      }         CANTECREC;              /*              */
                char            wk1[18];                /*              */
                union un_mb3116 CANTXPR1;               /* CANTXPR1     */
                union un_mb15_1 CANTXPR0;               /* CANTXPR0     */
                char            wk2[4];                 /*              */
                union un_mb3116 CANTXCR1;               /* CANTXCR1     */
                union un_mb15_1 CANTXCR0;               /* CANTXCR0     */
                char            wk3[4];                 /*              */
                union un_mb3116 CANTXACK1;              /* CANTXACK1    */
                union un_mb15_1 CANTXACK0;              /* CANTXACK0    */
                char            wk4[4];                 /*              */
                union un_mb3116 CANABACK1;              /* CANABACK1    */
                union un_mb15_1 CANABACK0;              /* CANABACK0    */
                char            wk5[4];                 /*              */
                union un_mb3116 CANRXPR1;               /* CANRXPR1     */
                union un_mb15_0 CANRXPR0;               /* CANRXPR0     */
                char            wk6[4];                 /*              */
                union un_mb3116 CANRFPR1;               /* CANRFPR1     */
                union un_mb15_0 CANRFPR0;               /* CANRFPR0     */
                char            wk7[4];                 /*              */
                union un_mb3116 CANMBIMR1;              /* CANMBIMR1    */
                union un_mb15_0 CANMBIMR0;              /* CANMBIMR0    */
                char            wk8[4];                 /*              */
                union un_mb3116 CANUMSR1;               /* CANUMSR1     */
                union un_mb15_0 CANUMSR0;               /* CANUMSR0     */
                char            wk9[36];                /*              */
                unsigned short  CANTCNTR;               /* CANTCNTR     */
                union {                                 /* CANTCR       */
                      unsigned short WORD;              /*  Word Access */
                      struct {                          /*  Bit  Access */
                             unsigned short ET  :1;     /*    ET        */
                             unsigned short     :1;     /*              */
                             unsigned short TSCR:1;     /*    TSCR      */
                             unsigned short TSCT:1;     /*    TSCT      */
                             unsigned short TCSC:1;     /*    TCSC      */
                             unsigned short     :5;     /*              */
                             unsigned short TPSC:6;     /*    TPSC      */
                             }       BIT;               /*              */
                      }         CANTCR;                 /*              */
                char            wk10[12];               /*              */
                unsigned short  CANTCMR;                /* CANTCMR      */
                char            wk11[110];              /*              */
                struct {                                /* MB           */
                   union {                              /* Control(CTRL)*/
                         unsigned short WORD;           /*  Word Access */
                         struct {                       /*  Bit  Access */
                                unsigned short      : 1;/*              */
                                unsigned short STID :11;/*    STDID     */
                                unsigned short RTR  : 1;/*    RTR       */
                                unsigned short IDE  : 1;/*    IDE       */
                                unsigned short EXTID: 2;/*    EXTID     */
                                }       BIT;            /*              */
                         }         CTRLH;               /*              */
                   union {                              /* Control(CTRL)*/
                         unsigned short WORD;           /*  Word Access */
                         struct {                       /*  Bit  Access */
                                unsigned short EXTID:16;/*    EXTID     */
                                }       BIT;            /*              */
                         }         CTRLM;               /*              */
                   union {                              /* Control(CTRL)*/
                         unsigned short WORD;           /*  Word Access */
                         struct {                       /*  Bit  Access */
                                unsigned char     :1;   /*              */
                                unsigned char     :1;   /*              */
                                unsigned char NMC :1;   /*    NMC       */
                                unsigned char ATX :1;   /*    ATX       */
                                unsigned char DART:1;   /*    DART      */
                                unsigned char MBC :3;   /*    MBC       */
                                unsigned char     :2;   /*              */
                                unsigned char CBE :1;   /*    CBE       */
                                unsigned char     :1;   /*              */
                                unsigned char DLC :4;   /*    DLC       */
                                }       BIT;            /*              */
                         }         CTRLL;               /*              */
                   unsigned short  TMSTP;               /* TimeStamp    */
                   unsigned char   MSG_DATA[8];         /* MSG_DATA     */
                   union {                              /* LAFM0        */
                         unsigned short WORD;           /*  Word Access */
                         struct {                       /*  Bit  Access */
                                unsigned short      : 1;/*              */
                                unsigned short STDID:11;/*    STDID     */
                                unsigned short      : 2;/*              */
                                unsigned short EXTID: 2;/*    EXTID     */
                                }       BIT;            /*              */
                         }         LAFM0;               /*              */
                   union {                              /* LAFM1        */
                         unsigned short WORD;           /*  Word Access */
                         struct {                       /*  Bit  Access */
                                unsigned short EXTID:16;/*    EXTID     */
                                }       BIT;            /*              */
                         }         LAFM1;               /*              */
                   char            wk[12];              /*              */
                }               MB[32];                 /*              */
};                                                      /*              */
struct st_hspi {                                        /* struct HSPI  */
               union {                                  /* SPCR         */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int     :24;       /*              */
                            unsigned int FBS : 1;       /*    FBS       */
                            unsigned int CLKP: 1;       /*    CLKP      */
                            unsigned int IDIV: 1;       /*    IDIV      */
                            unsigned int CLKC: 5;       /*    CLKC      */
                            }     BIT;                  /*              */
                     }          SPCR;                   /*              */
               union {                                  /* SPSR         */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int     :21;       /*              */
                            unsigned int TXFU: 1;       /*    TXFU      */
                            unsigned int TXHA: 1;       /*    TXHA      */
                            unsigned int TXEM: 1;       /*    TXEM      */
                            unsigned int RXFU: 1;       /*    RXFU      */
                            unsigned int RXHA: 1;       /*    RXHA      */
                            unsigned int RXEM: 1;       /*    RXEM      */
                            unsigned int RXOO: 1;       /*    RXOO      */
                            unsigned int RXOW: 1;       /*    RXOW      */
                            unsigned int RXFL: 1;       /*    RXFL      */
                            unsigned int TXFN: 1;       /*    TXFN      */
                            unsigned int TXFL: 1;       /*    TXFL      */
                            }     BIT;                  /*              */
                     }          SPSR;                   /*              */
               union {                                  /* SPSCR        */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int     :18;       /*              */
                            unsigned int TEIE: 1;       /*    TEIE      */
                            unsigned int THIE: 1;       /*    THIE      */
                            unsigned int RNIE: 1;       /*    RNIE      */
                            unsigned int RHIE: 1;       /*    RHIE      */
                            unsigned int RFIE: 1;       /*    RFIE      */
                            unsigned int FFEN: 1;       /*    FFEN      */
                            unsigned int LMSB: 1;       /*    LMSB      */
                            unsigned int CSV : 1;       /*    CSV       */
                            unsigned int CSA : 1;       /*    CSA       */
                            unsigned int TFIE: 1;       /*    TFIE      */
                            unsigned int ROIE: 1;       /*    ROIE      */
                            unsigned int RXDE: 1;       /*    RXDE      */
                            unsigned int TXDE: 1;       /*    TXDE      */
                            unsigned int MASL: 1;       /*    MASL      */
                            }     BIT;                  /*              */
                     }          SPSCR;                  /*              */
               union {                                  /* SPTBR        */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int   :24;         /*              */
                            unsigned int TD: 8;         /*    TD        */
                            }     BIT;                  /*              */
                     }          SPTBR;                  /*              */
               union {                                  /* SPRBR        */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int   :24;         /*              */
                            unsigned int RD: 8;         /*    RD        */
                            }     BIT;                  /*              */
                     }          SPRBR;                  /*              */
};                                                      /*              */
struct st_pfc {                                         /* struct PFC   */
              union {                                   /* PACR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PA7MD:2;      /*    PA7MD     */
                           unsigned short PA6MD:2;      /*    PA6MD     */
                           unsigned short PA5MD:2;      /*    PA5MD     */
                           unsigned short PA4MD:2;      /*    PA4MD     */
                           unsigned short PA3MD:2;      /*    PA3MD     */
                           unsigned short PA2MD:2;      /*    PA2MD     */
                           }       BIT;                 /*              */
                    }           PACR;                   /*              */
              char              wk1[2];                 /*              */
              union {                                   /* PBCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PB7MD:2;      /*    PB7MD     */
                           unsigned short PB6MD:2;      /*    PB6MD     */
                           unsigned short PB5MD:2;      /*    PB5MD     */
                           unsigned short PB4MD:2;      /*    PB4MD     */
                           unsigned short PB3MD:2;      /*    PB3MD     */
                           unsigned short PB2MD:2;      /*    PB2MD     */
                           unsigned short PB1MD:2;      /*    PB1MD     */
                           }       BIT;                 /*              */
                    }           PBCR;                   /*              */
              char              wk2[2];                 /*              */
              union {                                   /* PCCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PC7MD:2;      /*    PC7MD     */
                           unsigned short PC6MD:2;      /*    PC6MD     */
                           unsigned short PC5MD:2;      /*    PC5MD     */
                           unsigned short PC4MD:2;      /*    PC4MD     */
                           unsigned short PC3MD:2;      /*    PC3MD     */
                           unsigned short PC2MD:2;      /*    PC2MD     */
                           unsigned short PC1MD:2;      /*    PC1MD     */
                           unsigned short PC0MD:2;      /*    PC0MD     */
                           }       BIT;                 /*              */
                    }           PCCR;                   /*              */
              char              wk3[2];                 /*              */
              union {                                   /* PDCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PD7MD:2;      /*    PD7MD     */
                           unsigned short PD6MD:2;      /*    PD6MD     */
                           unsigned short PD5MD:2;      /*    PD5MD     */
                           unsigned short PD4MD:2;      /*    PD4MD     */
                           unsigned short PD3MD:2;      /*    PD3MD     */
                           unsigned short PD2MD:2;      /*    PD2MD     */
                           unsigned short PD1MD:2;      /*    PD1MD     */
                           unsigned short PD0MD:2;      /*    PD0MD     */
                           }       BIT;                 /*              */
                    }           PDCR;                   /*              */
              char              wk4[2];                 /*              */
              union {                                   /* PECR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PE7MD:2;      /*    PE7MD     */
                           unsigned short PE6MD:2;      /*    PE6MD     */
                           unsigned short PE5MD:2;      /*    PE5MD     */
                           unsigned short PE4MD:2;      /*    PE4MD     */
                           unsigned short PE3MD:2;      /*    PE3MD     */
                           unsigned short PE2MD:2;      /*    PE2MD     */
                           unsigned short PE1MD:2;      /*    PE1MD     */
                           unsigned short PE0MD:2;      /*    PE0MD     */
                           }       BIT;                 /*              */
                    }           PECR;                   /*              */
              char              wk5[2];                 /*              */
              union {                                   /* PFCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short      :8;      /*              */
                           unsigned short PF3MD:2;      /*    PF3MD     */
                           unsigned short PF2MD:2;      /*    PF2MD     */
                           unsigned short PF1MD:2;      /*    PF1MD     */
                           unsigned short PF0MD:2;      /*    PF0MD     */
                           }       BIT;                 /*              */
                    }           PFCR;                   /*              */
              char              wk6[2];                 /*              */
              union {                                   /* PGCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PG7MD:2;      /*    PG7MD     */
                           unsigned short PG6MD:2;      /*    PG6MD     */
                           unsigned short PG5MD:2;      /*    PG5MD     */
                           unsigned short PG4MD:2;      /*    PG4MD     */
                           unsigned short PG3MD:2;      /*    PG3MD     */
                           unsigned short PG2MD:2;      /*    PG2MD     */
                           unsigned short PG1MD:2;      /*    PG1MD     */
                           unsigned short PG0MD:2;      /*    PG0MD     */
                           }       BIT;                 /*              */
                    }           PGCR;                   /*              */
              char              wk7[2];                 /*              */
              union {                                   /* PHCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PH7MD:2;      /*    PH7MD     */
                           unsigned short PH6MD:2;      /*    PH6MD     */
                           unsigned short PH5MD:2;      /*    PH5MD     */
                           unsigned short PH4MD:2;      /*    PH4MD     */
                           unsigned short PH3MD:2;      /*    PH3MD     */
                           unsigned short PH2MD:2;      /*    PH2MD     */
                           unsigned short PH1MD:2;      /*    PH1MD     */
                           unsigned short PH0MD:2;      /*    PH0MD     */
                           }       BIT;                 /*              */
                    }           PHCR;                   /*              */
              char              wk8[2];                 /*              */
              union {                                   /* PJCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PJ7MD:2;      /*    PJ7MD     */
                           unsigned short PJ6MD:2;      /*    PJ6MD     */
                           unsigned short PJ5MD:2;      /*    PJ5MD     */
                           unsigned short PJ4MD:2;      /*    PJ4MD     */
                           unsigned short PJ3MD:2;      /*    PJ3MD     */
                           unsigned short PJ2MD:2;      /*    PJ2MD     */
                           unsigned short PJ1MD:2;      /*    PJ1MD     */
                           }       BIT;                 /*              */
                    }           PJCR;                   /*              */
              char              wk9[2];                 /*              */
              union {                                   /* PKCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PK7MD:2;      /*    PK7MD1    */
                           unsigned short PK6MD:2;      /*    PK6MD1    */
                           unsigned short PK5MD:2;      /*    PK5MD1    */
                           unsigned short PK4MD:2;      /*    PK4MD1    */
                           unsigned short PK3MD:2;      /*    PK3MD1    */
                           unsigned short PK2MD:2;      /*    PK2MD1    */
                           }       BIT;                 /*              */
                    }           PKCR;                   /*              */
              char              wk10[2];                /*              */
              union {                                   /* INPUPA       */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short MD6PUP :1;    /*    MD6PUP    */
                           unsigned short RDYPUP :1;    /*    RDYPUP    */
                           unsigned short BREQPUP:1;    /*    BREQPUP   */
                           unsigned short IRL0PUP:1;    /*    IRL0PUP   */
                           unsigned short IRL1PUP:1;    /*    IRL1PUP   */
                           unsigned short IRL2PUP:1;    /*    IRL2PUP   */
                           unsigned short IRL3PUP:1;    /*    IRL3PUP   */
                           unsigned short NMIPUP :1;    /*    NMIPUP    */
                           }       BIT;                 /*              */
                    }           INPUPA;                 /*              */
              char              wk11[2];                /*              */
              union {                                   /* DMAPCR       */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short DREQP0:1;     /*    DREQP0    */
                           unsigned short       :1;     /*              */
                           unsigned short DREQP1:1;     /*    DREQP1    */
                           unsigned short       :1;     /*              */
                           unsigned short DACK0 :2;     /*    DACKP     */
                           unsigned short DACK1 :2;     /*    DACKP     */
                           unsigned short DRAK0 :2;     /*    DRAKP     */
                           unsigned short DRAK1 :2;     /*    DRAKP     */
                           unsigned short       :3;     /*              */
                           unsigned short BRGRST:1;     /*    BRGRST    */
                           }       BIT;                 /*              */
                    }           DMAPCR;                 /*              */
              char              wk12[2];                /*              */
              union {                                   /* SCIHZR       */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short SCICLK0:1;    /*    SCICLK0   */
                           unsigned short SCIRXD0:1;    /*    SCIRXD0   */
                           unsigned short SCITXD0:1;    /*    SCITXD0   */
                           unsigned short SCICLK1:1;    /*    SCICLK1   */
                           unsigned short SCICTS1:1;    /*    SCICTS1   */
                           unsigned short SCIRTS1:1;    /*    SCIRTS1   */
                           unsigned short SCIRXD1:1;    /*    SCIRXD1   */
                           unsigned short SCITXD1:1;    /*    SCITXD1   */
                           unsigned short SCICLK2:1;    /*    SCICLK2   */
                           unsigned short SCICTS2:1;    /*    SCICTS2   */
                           unsigned short SCIRTS2:1;    /*    SCIRTS2   */
                           unsigned short SCIRXD2:1;    /*    SCIRXD2   */
                           unsigned short SCITXD2:1;    /*    SCITXD2   */
                           }       BIT;                 /*              */
                    }           SCIHZR;                 /*              */
              char              wk13[2];                /*              */
              union {                                   /* IPSELR       */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short IPSELR15:2;   /*    IPSELR15-4*/
                           unsigned short IPSELR13:1;   /*    IPSELR13  */
                           unsigned short IPSELR12:1;   /*    IPSELR12  */
                           unsigned short IPSELR11:2;   /*    IPSELR11-0*/
                           unsigned short IPSELR9 :1;   /*    IPSELR9   */
                           unsigned short         :7;   /*              */
                           unsigned short LCDMD   :2;   /*    LCDMD     */
                           }       BIT;                 /*              */
                    }           IPSELR;                 /*              */
              char              wk14[10];               /*              */
              union {                                   /* PADR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PA7DT:1;       /*    PA7DT     */
                           unsigned char PA6DT:1;       /*    PA6DT     */
                           unsigned char PA5DT:1;       /*    PA5DT     */
                           unsigned char PA4DT:1;       /*    PA4DT     */
                           unsigned char PA3DT:1;       /*    PA3DT     */
                           unsigned char PA2DT:1;       /*    PA2DT     */
                           }      BIT;                  /*              */
                    }           PADR;                   /*              */
              char              wk15[3];                /*              */
              union {                                   /* PBDR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PB7DT:1;       /*    PB7DT     */
                           unsigned char PB6DT:1;       /*    PB6DT     */
                           unsigned char PB5DT:1;       /*    PB5DT     */
                           unsigned char PB4DT:1;       /*    PB4DT     */
                           unsigned char PB3DT:1;       /*    PB3DT     */
                           unsigned char PB2DT:1;       /*    PB2DT     */
                           unsigned char PB1DT:1;       /*    PB1DT     */
                           }      BIT;                  /*              */
                    }           PBDR;                   /*              */
              char              wk16[3];                /*              */
              union {                                   /* PCDR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PC7DT:1;       /*    PC7DT     */
                           unsigned char PC6DT:1;       /*    PC6DT     */
                           unsigned char PC5DT:1;       /*    PC5DT     */
                           unsigned char PC4DT:1;       /*    PC4DT     */
                           unsigned char PC3DT:1;       /*    PC3DT     */
                           unsigned char PC2DT:1;       /*    PC2DT     */
                           unsigned char PC1DT:1;       /*    PC1DT     */
                           unsigned char PC0DT:1;       /*    PC0DT     */
                           }      BIT;                  /*              */
                    }           PCDR;                   /*              */
              char              wk17[3];                /*              */
              union {                                   /* PDDR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PD7DT:1;       /*    PD7DT     */
                           unsigned char PD6DT:1;       /*    PD6DT     */
                           unsigned char PD5DT:1;       /*    PD5DT     */
                           unsigned char PD4DT:1;       /*    PD4DT     */
                           unsigned char PD3DT:1;       /*    PD3DT     */
                           unsigned char PD2DT:1;       /*    PD2DT     */
                           unsigned char PD1DT:1;       /*    PD1DT     */
                           unsigned char PD0DT:1;       /*    PD0DT     */
                           }      BIT;                  /*              */
                    }           PDDR;                   /*              */
              char              wk18[3];                /*              */
              union {                                   /* PEDR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PE7DT:1;       /*    PE7DT     */
                           unsigned char PE6DT:1;       /*    PE6DT     */
                           unsigned char PE5DT:1;       /*    PE5DT     */
                           unsigned char PE4DT:1;       /*    PE4DT     */
                           unsigned char PE3DT:1;       /*    PE3DT     */
                           unsigned char PE2DT:1;       /*    PE2DT     */
                           unsigned char PE1DT:1;       /*    PE1DT     */
                           unsigned char PE0DT:1;       /*    PE0DT     */
                           }      BIT;                  /*              */
                    }           PEDR;                   /*              */
              char              wk19[3];                /*              */
              union {                                   /* PFDR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char      :4;       /*              */
                           unsigned char PF3DT:1;       /*    PF3DT     */
                           unsigned char PF2DT:1;       /*    PF2DT     */
                           unsigned char PF1DT:1;       /*    PF1DT     */
                           unsigned char PF0DT:1;       /*    PF0DT     */
                           }      BIT;                  /*              */
                    }           PFDR;                   /*              */
              char              wk20[3];                /*              */
              union {                                   /* PGDR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PG7DT:1;       /*    PG7DT     */
                           unsigned char PG6DT:1;       /*    PG6DT     */
                           unsigned char PG5DT:1;       /*    PG5DT     */
                           unsigned char PG4DT:1;       /*    PG4DT     */
                           unsigned char PG3DT:1;       /*    PG3DT     */
                           unsigned char PG2DT:1;       /*    PG2DT     */
                           unsigned char PG1DT:1;       /*    PG1DT     */
                           unsigned char PG0DT:1;       /*    PG0DT     */
                           }      BIT;                  /*              */
                    }           PGDR;                   /*              */
              char              wk21[3];                /*              */
              union {                                   /* PHDR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PH7DT:1;       /*    PH7DT     */
                           unsigned char PH6DT:1;       /*    PH6DT     */
                           unsigned char PH5DT:1;       /*    PH5DT     */
                           unsigned char PH4DT:1;       /*    PH4DT     */
                           unsigned char PH3DT:1;       /*    PH3DT     */
                           unsigned char PH2DT:1;       /*    PH2DT     */
                           unsigned char PH1DT:1;       /*    PH1DT     */
                           unsigned char PH0DT:1;       /*    PH0DT     */
                           }      BIT;                  /*              */
                    }           PHDR;                   /*              */
              char              wk22[3];                /*              */
              union {                                   /* PJDR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
  #ifdef BIT_ORDER_RIGHT
                           unsigned char      :1;       /*    (bit0)    */
                           unsigned char PJ1DT:1;       /*    PJ1DT     */
                           unsigned char PJ2DT:1;       /*    PJ2DT     */
                           unsigned char PJ3DT:1;       /*    PJ3DT     */
                           unsigned char PJ4DT:1;       /*    PJ4DT     */
                           unsigned char PJ5DT:1;       /*    PJ5DT     */
                           unsigned char PJ6DT:1;       /*    PJ6DT     */
                           unsigned char PJ7DT:1;       /*    PJ7DT     */
  #else
                           unsigned char PJ7DT:1;       /*    PJ7DT     */
                           unsigned char PJ6DT:1;       /*    PJ6DT     */
                           unsigned char PJ5DT:1;       /*    PJ5DT     */
                           unsigned char PJ4DT:1;       /*    PJ4DT     */
                           unsigned char PJ3DT:1;       /*    PJ3DT     */
                           unsigned char PJ2DT:1;       /*    PJ2DT     */
                           unsigned char PJ1DT:1;       /*    PJ1DT     */
  #endif
                           }      BIT;                  /*              */
                    }           PJDR;                   /*              */
              char              wk23[3];                /*              */
              union {                                   /* PKDR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PK7DT:1;       /*    PK7DT     */
                           unsigned char PK6DT:1;       /*    PK6DT     */
                           unsigned char PK5DT:1;       /*    PK5DT     */
                           unsigned char PK4DT:1;       /*    PK4DT     */
                           unsigned char PK3DT:1;       /*    PK3DT     */
                           unsigned char PK2DT:1;       /*    PK2DT     */
                           }      BIT;                  /*              */
                    }           PKDR;                   /*              */
              char              wk24[27];               /*              */
              union {                                   /* PAPUPR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PA7PUPR:1;     /*    PA7PUPR   */
                           unsigned char PA6PUPR:1;     /*    PA6PUPR   */
                           unsigned char PA5PUPR:1;     /*    PA5PUPR   */
                           unsigned char PA4PUPR:1;     /*    PA4PUPR   */
                           unsigned char PA3PUPR:1;     /*    PA3PUPR   */
                           unsigned char PA2PUPR:1;     /*    PA2PUPR   */
                           }      BIT;                  /*              */
                    }           PAPUPR;                 /*              */
              char              wk25[3];                /*              */
              union {                                   /* PBPUPR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PB7PUPR:1;     /*    PB7PUPR   */
                           unsigned char PB6PUPR:1;     /*    PB6PUPR   */
                           unsigned char PB5PUPR:1;     /*    PB5PUPR   */
                           unsigned char PB4PUPR:1;     /*    PB4PUPR   */
                           unsigned char PB3PUPR:1;     /*    PB3PUPR   */
                           unsigned char PB2PUPR:1;     /*    PB2PUPR   */
                           unsigned char PB1PUPR:1;     /*    PB1PUPR   */
                           }      BIT;                  /*              */
                    }           PBPUPR;                 /*              */
              char              wk26[3];                /*              */
              union {                                   /* PCPUPR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PC7PUPR:1;     /*    PC7PUPR   */
                           unsigned char PC6PUPR:1;     /*    PC6PUPR   */
                           unsigned char PC5PUPR:1;     /*    PC5PUPR   */
                           unsigned char PC4PUPR:1;     /*    PC4PUPR   */
                           unsigned char PC3PUPR:1;     /*    PC3PUPR   */
                           unsigned char PC2PUPR:1;     /*    PC2PUPR   */
                           unsigned char PC1PUPR:1;     /*    PC1PUPR   */
                           unsigned char PC0PUPR:1;     /*    PC0PUPR   */
                           }      BIT;                  /*              */
                    }           PCPUPR;                 /*              */
              char              wk27[3];                /*              */
              union {                                   /* PDPUPR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PD7PUPR:1;     /*    PD7PUPR   */
                           unsigned char PD6PUPR:1;     /*    PD6PUPR   */
                           unsigned char PD5PUPR:1;     /*    PD5PUPR   */
                           unsigned char PD4PUPR:1;     /*    PD4PUPR   */
                           unsigned char PD3PUPR:1;     /*    PD3PUPR   */
                           unsigned char PD2PUPR:1;     /*    PD2PUPR   */
                           unsigned char PD1PUPR:1;     /*    PD1PUPR   */
                           unsigned char PD0PUPR:1;     /*    PD0PUPR   */
                           }      BIT;                  /*              */
                    }           PDPUPR;                 /*              */
              char              wk28[3];                /*              */
              union {                                   /* PEPUPR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PE7PUPR:1;     /*    PE7PUPR   */
                           unsigned char PE6PUPR:1;     /*    PE6PUPR   */
                           unsigned char PE5PUPR:1;     /*    PE5PUPR   */
                           unsigned char PE4PUPR:1;     /*    PE4PUPR   */
                           unsigned char PE3PUPR:1;     /*    PE3PUPR   */
                           unsigned char PE2PUPR:1;     /*    PE2PUPR   */
                           unsigned char PE1PUPR:1;     /*    PE1PUPR   */
                           unsigned char PE0PUPR:1;     /*    PE0PUPR   */
                           }      BIT;                  /*              */
                    }           PEPUPR;                 /*              */
              char              wk29[3];                /*              */
              union {                                   /* PFPUPR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char        :4;     /*              */
                           unsigned char PF3PUPR:1;     /*    PF3PUPR   */
                           unsigned char PF2PUPR:1;     /*    PF2PUPR   */
                           unsigned char PF1PUPR:1;     /*    PF1PUPR   */
                           unsigned char PF0PUPR:1;     /*    PF0PUPR   */
                           }      BIT;                  /*              */
                    }           PFPUPR;                 /*              */
              char              wk30[3];                /*              */
              union {                                   /* PGPUPR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PG7PUPR:1;     /*    PG7PUPR   */
                           unsigned char PG6PUPR:1;     /*    PG6PUPR   */
                           unsigned char PG5PUPR:1;     /*    PG5PUPR   */
                           unsigned char PG4PUPR:1;     /*    PG4PUPR   */
                           unsigned char PG3PUPR:1;     /*    PG3PUPR   */
                           unsigned char PG2PUPR:1;     /*    PG2PUPR   */
                           unsigned char PG1PUPR:1;     /*    PG1PUPR   */
                           unsigned char PG0PUPR:1;     /*    PG0PUPR   */
                           }      BIT;                  /*              */
                    }           PGPUPR;                 /*              */
              char              wk31[3];                /*              */
              union {                                   /* PHPUPR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PH7PUPR:1;     /*    PH7PUPR   */
                           unsigned char PH6PUPR:1;     /*    PH6PUPR   */
                           unsigned char PH5PUPR:1;     /*    PH5PUPR   */
                           unsigned char PH4PUPR:1;     /*    PH4PUPR   */
                           unsigned char PH3PUPR:1;     /*    PH3PUPR   */
                           unsigned char PH2PUPR:1;     /*    PH2PUPR   */
                           unsigned char PH1PUPR:1;     /*    PH1PUPR   */
                           unsigned char PH0PUPR:1;     /*    PH0PUPR   */
                           }      BIT;                  /*              */
                    }           PHPUPR;                 /*              */
              char              wk32[3];                /*              */
              union {                                   /* PJPUPR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PJ7PUPR:1;     /*    PJ7PUPR   */
                           unsigned char PJ6PUPR:1;     /*    PJ6PUPR   */
                           unsigned char PJ5PUPR:1;     /*    PJ5PUPR   */
                           unsigned char PJ4PUPR:1;     /*    PJ4PUPR   */
                           unsigned char PJ3PUPR:1;     /*    PJ3PUPR   */
                           unsigned char PJ2PUPR:1;     /*    PJ2PUPR   */
                           unsigned char PJ1PUPR:1;     /*    PJ1PUPR   */
                           }      BIT;                  /*              */
                    }           PJPUPR;                 /*              */
              char              wk33[3];                /*              */
              union {                                   /* PKPUPR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PK7PUPR:1;     /*    PK7PUPR   */
                           unsigned char PK6PUPR:1;     /*    PK6PUPR   */
                           unsigned char PK5PUPR:1;     /*    PK5PUPR   */
                           unsigned char PK4PUPR:1;     /*    PK4PUPR   */
                           unsigned char PK3PUPR:1;     /*    PK3PUPR   */
                           unsigned char PK2PUPR:1;     /*    PK2PUPR   */
                           }      BIT;                  /*              */
                    }           PKPUPR;                 /*              */
              char              wk34[3];                /*              */
              union {                                   /* MDPUPR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char MDPUPR7:1;     /*    MDPUPR7   */
                           unsigned char MDPUPR6:1;     /*    MDPUPR6   */
                           unsigned char MDPUPR5:1;     /*    MDPUPR5   */
                           unsigned char MDPUPR4:1;     /*    MDPUPR4   */
                           unsigned char MDPUPR3:1;     /*    MDPUPR3   */
                           unsigned char MDPUPR2:1;     /*    MDPUPR2   */
                           unsigned char MDPUPR1:1;     /*    MDPUPR1   */
                           unsigned char MDPUPR0:1;     /*    MDPUPR0   */
                           }      BIT;                  /*              */
                    }           MDPUPR;                 /*              */
              char              wk35[3];                /*              */
              union {                                   /* MODSELR      */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char MODSELR7:1;    /*    MODSELR7  */
                           unsigned char MODSELR6:1;    /*    MODSELR6  */
                           unsigned char MODSELR5:1;    /*    MODSELR5  */
                           unsigned char MODSELR4:1;    /*    MODSELR4  */
                           unsigned char MODSELR3:1;    /*    MODSELR3  */
                           unsigned char MODSELR2:1;    /*    MODSELR2  */
                           unsigned char MODSELR1:1;    /*    MODSELR1  */
                           }      BIT;                  /*              */
                    }           MODSELR;                /*              */
              char              wk36[20971418];         /*              */
              union {                                   /* GPIOIC       */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PTIREN15:1;   /*    PTIREN15  */
                           unsigned short PTIREN14:1;   /*    PTIREN14  */
                           unsigned short PTIREN13:1;   /*    PTIREN13  */
                           unsigned short PTIREN12:1;   /*    PTIREN12  */
                           unsigned short PTIREN11:1;   /*    PTIREN11  */
                           unsigned short PTIREN10:1;   /*    PTIREN10  */
                           unsigned short PTIREN9 :1;   /*    PTIREN9   */
                           unsigned short STBRT8  :1;   /*    STBRT8    */
                           unsigned short STBRT7  :1;   /*    STBRT7    */
                           unsigned short STBRT6  :1;   /*    STBRT6    */
                           unsigned short STBIRQ5 :1;   /*    STBIRQ5   */
                           unsigned short STBIRQ4 :1;   /*    STBIRQ4   */
                           unsigned short STBIRL3 :1;   /*    STBIRL3   */
                           unsigned short STBIRL2 :1;   /*    STBIRL2   */
                           unsigned short STBIRL1 :1;   /*    STBIRL1   */
                           unsigned short STBIRL0 :1;   /*    STBIRL0   */
                           }       BIT;                 /*              */
                    }           GPIOIC;                 /*              */
};                                                      /*              */
struct st_hac {                                         /* struct HAC   */
              union {                                   /* HACCR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :16;        /*              */
                           unsigned int CR  : 1;        /*    CR        */
                           unsigned int     : 3;        /*              */
                           unsigned int CDRT: 1;        /*    CDRT      */
                           unsigned int WMRT: 1;        /*    WMRT      */
                           unsigned int     : 4;        /*              */
                           unsigned int ST  : 1;        /*    ST        */
                           }      BIT;                  /*              */
                    }           HACCR;                  /*              */
              char              wk1[20];                /*              */
              union {                                   /* HACCSAR      */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int        :12;     /*              */
                           unsigned int RW     : 1;     /*    RW        */
                           unsigned int CASA   : 7;     /*    CA/SA    */
                           unsigned int SLREQ3 : 1;     /*    SLREQ3    */
                           unsigned int SLREQ4 : 1;     /*    SLREQ4    */
                           unsigned int SLREQ5 : 1;     /*    SLREQ5    */
                           unsigned int SLREQ6 : 1;     /*    SLREQ6    */
                           unsigned int SLREQ7 : 1;     /*    SLREQ7    */
                           unsigned int SLREQ8 : 1;     /*    SLREQ8    */
                           unsigned int SLREQ9 : 1;     /*    SLREQ9    */
                           unsigned int SLREQ10: 1;     /*    SLREQ10   */
                           unsigned int SLREQ11: 1;     /*    SLREQ11   */
                           unsigned int SLREQ12: 1;     /*    SLREQ12   */
                           }     BIT;                   /*              */
                    }           HACCSAR;                /*              */
              union {                                   /* HACCSDR      */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :12;        /*              */
                           unsigned int CDSD:16;        /*    CD/SD     */
                           unsigned int     : 4;        /*              */
                           }     BIT;                   /*              */
                    }           HACCSDR;                /*              */
              union {                                   /* HACPCML      */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int  :12;           /*              */
                           unsigned int D:20;           /*    D         */
                           }     BIT;                   /*              */
                    struct {                            /*  Word Access */
                           unsigned int LD:16;          /*    LD        */
                           unsigned int RD:16;          /*    RD        */
                           }     WORD;                  /*              */
                    }           HACPCML;                /*              */
              union {                                   /* HACPCMR      */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int  :12;           /*              */
                           unsigned int D:20;           /*    D         */
                           }     BIT;                   /*              */
                    }           HACPCMR;                /*              */
              char              wk2[32];                /*              */
              union {                                   /* HACTIER      */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int         : 2;    /*              */
                           unsigned int PLTFRQIE: 1;    /*    PLTFRQIE  */
                           unsigned int PRTFRQIE: 1;    /*    PRTFRQIE  */
                           unsigned int         :18;    /*              */
                           unsigned int PLTFUNIE: 1;    /*    PLTFUNIE  */
                           unsigned int PRTFUNIE: 1;    /*    PRTFUNIE  */
                           }     BIT;                   /*              */
                    }           HACTIER;                /*              */
              union {                                   /* HACTSR       */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int CMDAMT: 1;      /*    CMDAMT    */
                           unsigned int CMDDMT: 1;      /*    CMDDMT    */
                           unsigned int PLTFRQ: 1;      /*    PLTFRQ    */
                           unsigned int PRTFRQ: 1;      /*    PRTFRQ    */
                           unsigned int       :18;      /*              */
                           unsigned int PLTFUN: 1;      /*    PLTFUN    */
                           unsigned int PRTFUN: 1;      /*    PRTFUN    */
                           }     BIT;                   /*              */
                    }           HACTSR;                 /*              */
              union {                                   /* HACRIER      */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int         :9;     /*              */
                           unsigned int STARYIE :1;     /*    STARYIE   */
                           unsigned int STDRYIE :1;     /*    STDRYIE   */
                           unsigned int PLRFRQIE:1;     /*    PLRFRQIE  */
                           unsigned int PRRFRQIE:1;     /*    PRRFRQIE  */
                           unsigned int         :5;     /*              */
                           unsigned int PLRFOVIE:1;     /*    PLRFOVIE  */
                           unsigned int PRRFOVIE:1;     /*    PRRFOVIE  */
                           }     BIT;                   /*              */
                    }           HACRIER;                /*              */
              union {                                   /* HACRSR       */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int       :9;       /*              */
                           unsigned int STARY :1;       /*    STARY     */
                           unsigned int STDRY :1;       /*    STDRY     */
                           unsigned int PLRFRQ:1;       /*    PLRFRQ    */
                           unsigned int PRRFRQ:1;       /*    PRRFRQ    */
                           unsigned int       :5;       /*              */
                           unsigned int PLRFOV:1;       /*    PLRFOV    */
                           unsigned int PRRFOV:1;       /*    PRRFOV    */
                           }     BIT;                   /*              */
                    }           HACRSR;                 /*              */
              union {                                   /* HACACR       */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int            :1;  /*              */
                           unsigned int DMARX16    :1;  /*    DMARX16   */
                           unsigned int DMATX16    :1;  /*    DMATX16   */
                           unsigned int            :2;  /*              */
                           unsigned int TX12_ATOMIC:1;  /*    TX12_ATOMI*/
                           unsigned int            :1;  /*              */
                           unsigned int RXDMAL_EN  :1;  /*    RXDMAL_EN */
                           unsigned int TXDMAL_EN  :1;  /*    TXDMAL_EN */
                           unsigned int RXDMAR_EN  :1;  /*    RXDMAR_EN */
                           unsigned int TXDMAR_EN  :1;  /*    TXDMAR_EN */
                           }     BIT;                   /*              */
                    }           HACACR;                 /*              */
};                                                      /*              */
struct st_mmcif {                                       /* struct MMCIF */
                union {                                 /* CMDR0        */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char Start:1;     /*    Start     */
                             unsigned char Host :1;     /*    Host      */
                             unsigned char INDEX:6;     /*    INDEX     */
                             }      BIT;                /*              */
                      }         CMDR0;                  /*              */
                unsigned char   CMDR1;                  /* CMDR1        */
                unsigned char   CMDR2;                  /* CMDR2        */
                unsigned char   CMDR3;                  /* CMDR3        */
                unsigned char   CMDR4;                  /* CMDR4        */
                union {                                 /* CMDR5        */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char CRC:7;       /*    CRC       */
                             unsigned char End:1;       /*    End       */
                             }      BIT;                /*              */
                      }         CMDR5;                  /*              */
                union {                                 /* CMDSTRT      */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char      :7;     /*              */
                             unsigned char START:1;     /*    START     */
                             }      BIT;                /*              */
                      }         CMDSTRT;                /*              */
                char            wk1[3];                 /*              */
                union {                                 /* OPCR         */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char CMDOFF  :1;  /*    CMDOFF    */
                             unsigned char         :1;  /*              */
                             unsigned char RD_CONTI:1;  /*    RD_CONTI  */
                             unsigned char DATAEN  :1;  /*    DATAEN    */
                             }      BIT;                /*              */
                      }         OPCR;                   /*              */
                union {                                 /* CSTR         */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char BUSY      :1;/*    BUSY      */
                             unsigned char FIFO_FULL :1;/*    FIFO_FULL */
                             unsigned char FIFO_EMPTY:1;/*    FIFO_EMPTY*/
                             unsigned char CWRE      :1;/*    CWRE      */
                             unsigned char DTBUSY    :1;/*    DTBUSY    */
                             unsigned char DTBUSY_TU :1;/*    DTBUSY_TU */
                             unsigned char           :1;/*              */
                             unsigned char REQ       :1;/*    REQ       */
                             }      BIT;                /*              */
                      }         CSTR;                   /*              */
                union {                                 /* INTCR0       */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char FEIE  :1;    /*    FEIE      */
                             unsigned char FFIE  :1;    /*    FFIE      */
                             unsigned char DRPIE :1;    /*    DRPIE     */
                             unsigned char DTIE  :1;    /*    DTIE      */
                             unsigned char CRPIE :1;    /*    CRPIE     */
                             unsigned char CMDIE :1;    /*    CMDIE     */
                             unsigned char DBSYIE:1;    /*    DBSYIE    */
                             }      BIT;                /*              */
                      }         INTCR0;                 /*              */
                union {                                 /* INTCR1       */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char INTRQ2E:1;   /*    INTRQ2E   */
                             unsigned char INTRQ1E:1;   /*    INTRQ1E   */
                             unsigned char INTRQ0E:1;   /*    INTRQ0E   */
                             unsigned char        :2;   /*              */
                             unsigned char CRCERIE:1;   /*    CRCERIE   */
                             unsigned char DTERIE :1;   /*    DTERIE    */
                             unsigned char CTERIE :1;   /*    CTERIE    */
                             }      BIT;                /*              */
                      }         INTCR1;                 /*              */
                union {                                 /* INTSTR0      */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char FEI  :1;     /*    FEI       */
                             unsigned char FFI  :1;     /*    FFI       */
                             unsigned char DRPI :1;     /*    DRPI      */
                             unsigned char DTI  :1;     /*    DTI       */
                             unsigned char CRPI :1;     /*    CRPI      */
                             unsigned char CMDI :1;     /*    CMDI      */
                             unsigned char DBSYI:1;     /*    DBSYI     */
                             }      BIT;                /*              */
                      }         INTSTR0;                /*              */
                union {                                 /* INTSTR1      */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char       :5;    /*              */
                             unsigned char CRCERI:1;    /*    CRCERI    */
                             unsigned char DTERI :1;    /*    DTERI     */
                             unsigned char CTERI :1;    /*    CTERI     */
                             }      BIT;                /*              */
                      }         INTSTR1;                /*              */
                union {                                 /* CLKON        */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char CLKON:1;     /*    CLKON     */
                             unsigned char      :4;     /*              */
                             unsigned char CSEL :3;     /*    CSEL      */
                             }      BIT;                /*              */
                      }         CLKON;                  /*              */
                union {                                 /* CTOCR        */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char      :6;     /*              */
                             unsigned char CTSEL:2;     /*    CTSEL     */
                             }      BIT;                /*              */
                      }         CTOCR;                  /*              */
                char            wk2[2];                 /*              */
                union {                                 /* TBCR         */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char   :4;        /*              */
                             unsigned char CS:4;        /*    CS        */
                             }      BIT;                /*              */
                      }         TBCR;                   /*              */
                char            wk3;                    /*              */
                union {                                 /* MODER        */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char     :7;      /*              */
                             unsigned char MODE:1;      /*    MODE      */
                             }      BIT;                /*              */
                      }         MODER;                  /*              */
                char            wk4;                    /*              */
                union {                                 /* CMDTYR       */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char     :3;      /*              */
                             unsigned char TY4 :1;      /*    TY4       */
                             unsigned char TY3 :1;      /*    TY3       */
                             unsigned char TY2 :1;      /*    TY2       */
                             unsigned char TY10:2;      /*    TY1,TY0   */
                             }      BIT;                /*              */
                      }         CMDTYR;                 /*              */
                union {                                 /* RSPTYR       */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char       :2;    /*              */
                             unsigned char RTY5  :1;    /*    RTY5      */
                             unsigned char RTY4  :1;    /*    RTY4      */
                             unsigned char       :1;    /*              */
                             unsigned char RTY210:3;    /*    RTY2-RTY0 */
                             }      BIT;                /*              */
                      }         RSPTYR;                 /*              */
                char            wk5[6];                 /*              */
                unsigned char   RSPR0;                  /* RSPR0        */
                unsigned char   RSPR1;                  /* RSPR1        */
                unsigned char   RSPR2;                  /* RSPR2        */
                unsigned char   RSPR3;                  /* RSPR3        */
                unsigned char   RSPR4;                  /* RSPR4        */
                unsigned char   RSPR5;                  /* RSPR5        */
                unsigned char   RSPR6;                  /* RSPR6        */
                unsigned char   RSPR7;                  /* RSPR7        */
                unsigned char   RSPR8;                  /* RSPR8        */
                unsigned char   RSPR9;                  /* RSPR9        */
                unsigned char   RSPR10;                 /* RSPR10       */
                unsigned char   RSPR11;                 /* RSPR11       */
                unsigned char   RSPR12;                 /* RSPR12       */
                unsigned char   RSPR13;                 /* RSPR13       */
                unsigned char   RSPR14;                 /* RSPR14       */
                unsigned char   RSPR15;                 /* RSPR15       */
                unsigned char   RSPR16;                 /* RSPR16       */
                char            wk6;                    /*              */
                unsigned short  DTOUTR;                 /* DTOUTR       */
                char            wk7[12];                /*              */
                unsigned short  DR;                     /* DR           */
                unsigned char   FIFOCLR;                /* FIFOCLR      */
                char            wk8;                    /*              */
                union {                                 /* DMACR        */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char DMAEN:1;     /*    DMAEN     */
                             unsigned char      :4;     /*              */
                             unsigned char SET  :3;     /*    SET       */
                             }      BIT;                /*              */
                      }         DMACR;                  /*              */
                char            wk9;                    /*              */
                union {                                 /* INTCR2       */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char       :7;    /*              */
                             unsigned char FRDYIE:1;    /*    FRDYIE    */
                             }      BIT;                /*              */
                      }         INTCR2;                 /*              */
                char            wk10;                   /*              */
                union {                                 /* INTSTR2      */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char        :6;   /*              */
                             unsigned char FRDY_TU:1;   /*    FRDY_TU   */
                             unsigned char FRDYI  :1;   /*    FRDYI     */
                             }      BIT;                /*              */
                      }         INTSTR2;                /*              */
                char            wk11;                   /*              */
                union {                                 /* RDTIMSEL     */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char      :7;     /*              */
                             unsigned char RTSEL:1;     /*    RTSEL     */
                             }      BIT;                /*              */
                      }         RDTIMSEL;               /*              */
};                                                      /*              */
struct st_mfi {                                         /* struct MFI   */
              union {                                   /* MFIIDX       */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :24;        /*              */
                           unsigned int REG : 6;        /*    REG       */
                           unsigned int BYTE: 2;        /*    BYTE      */
                           }     BIT;                   /*              */
                    }           MFIIDX;                 /*              */
              union {                                   /* MFIGSR       */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int       :24;      /*              */
                           unsigned int STATUS: 8;      /*    STATUS    */
                           }     BIT;                   /*              */
                    }           MFIGSR;                 /*              */
              union {                                   /* MFISCR       */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int       :25;      /*              */
                           unsigned int SCRMD2: 1;      /*    SCRMD2    */
                           unsigned int       : 1;      /*              */
                           unsigned int SCRMD0: 1;      /*    SCRMD0    */
                           unsigned int       : 2;      /*              */
                           unsigned int EDN   : 1;      /*    EDN       */
                           unsigned int BO    : 1;      /*    BO        */
                           }     BIT;                   /*              */
                    }           MFISCR;                 /*              */
              union {                                   /* MFIMCR       */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int     :24;        /*              */
                           unsigned int LOCK: 1;        /*    LOCK      */
                           unsigned int     : 1;        /*              */
                           unsigned int WT  : 1;        /*    WT        */
                           unsigned int     : 1;        /*              */
                           unsigned int RD  : 1;        /*    RD        */
                           unsigned int     : 2;        /*              */
                           unsigned int AIAD: 1;        /*    AI/AD     */
                           }     BIT;                   /*              */
                    }           MFIMCR;                 /*              */
              union {                                   /* MFIIICR      */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int    :24;         /*              */
                           unsigned int IIC: 7;         /*    IIC       */
                           unsigned int IIR: 1;         /*    IIR       */
                           }     BIT;                   /*              */
                    }           MFIIICR;                /*              */
              union {                                   /* MFIEICR      */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int    :24;         /*              */
                           unsigned int EIC: 7;         /*    EIC       */
                           unsigned int EIR: 1;         /*    EIR       */
                           }     BIT;                   /*              */
                    }           MFIEICR;                /*              */
              union {                                   /* MFIADR       */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int  :21;           /*              */
                           unsigned int A: 9;           /*    A10-A2    */
                           }     BIT;                   /*              */
                    }           MFIADR;                 /*              */
              unsigned int      MFIDATA;                /* MFIDATA      */
};                                                      /*              */
struct st_adc {                                         /* struct ADC   */
              unsigned short    ADDRA;                  /* ADDRA        */
              unsigned short    ADDRB;                  /* ADDRB        */
              unsigned short    ADDRC;                  /* ADDRC        */
              unsigned short    ADDRD;                  /* ADDRD        */
              union {                                   /* ADCSR        */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short ADF  :1;      /*    ADF       */
                           unsigned short ADIE :1;      /*    ADIE      */
                           unsigned short ADST :1;      /*    ADST      */
                           unsigned short DMASL:1;      /*    DMASL     */
                           unsigned short TRGE :2;      /*    TRGE      */
                           unsigned short      :2;      /*              */
                           unsigned short CKSL :2;      /*    CKSL      */
                           unsigned short MDS  :2;      /*    MDS       */
                           unsigned short      :2;      /*              */
                           unsigned short CH   :2;      /*    CH        */
                           }       BIT;                 /*              */
                    }           ADCSR;                  /*              */
};                                                      /*              */
struct st_lcdc {                                        /* struct LCDC  */
               union {                                  /* LDPR         */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int  :8;           /*              */
                            unsigned int R:8;           /*    R         */
                            unsigned int G:8;           /*    G         */
                            unsigned int B:8;           /*    B         */
                            }     BIT;                  /*              */
                     }          LDPR[256];              /*              */
               union {                                  /* LDICKR       */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short       :2;    /*              */
                            unsigned short ICKSEL:2;    /*    ICKSEL    */
                            unsigned short       :7;    /*              */
                            unsigned short DCDR  :5;    /*    DCDR4     */
                            }       BIT;                /*              */
                     }          LDICKR;                 /*              */
               union {                                  /* LDMTR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short FLMPOL :1;   /*    FLMPOL    */
                            unsigned short CL1POL :1;   /*    CL1POL    */
                            unsigned short DISPPOL:1;   /*    DISPPOL   */
                            unsigned short DPOL   :1;   /*    DPOL      */
                            unsigned short        :1;   /*              */
                            unsigned short MCNT   :1;   /*    MCNT      */
                            unsigned short CL1CNT :1;   /*    CL1CNT    */
                            unsigned short CL2CNT :1;   /*    CL2CNT    */
                            unsigned short        :2;   /*              */
                            unsigned short MIFTYP :6;   /*    MIFTYP    */
                            }       BIT;                /*              */
                     }          LDMTR;                  /*              */
               union {                                  /* LDDFR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short         :7;  /*              */
                            unsigned short PABD    :1;  /*    PABD      */
                            unsigned short         :1;  /*              */
                            unsigned short DSPCOLOR:7;  /*    DSPCOLOR  */
                            }       BIT;                /*              */
                     }          LDDFR;                  /*              */
               union {                                  /* LDSMR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short    :2;       /*              */
                            unsigned short ROT:1;       /*    ROT       */
                            }       BIT;                /*              */
                     }          LDSMR;                  /*              */
               void            *LDSARU;                 /* LDSARU       */
               void            *LDSARL;                 /* LDSARL       */
               unsigned short   LDLAOR;                 /* LDLAOR       */
               union {                                  /* LDPALCR      */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short      :11;    /*              */
                            unsigned short PALS : 1;    /*    PALS      */
                            unsigned short      : 3;    /*              */
                            unsigned short PALEN: 1;    /*    PALEN     */
                            }       BIT;                /*              */
                     }          LDPALCR;                /*              */
               union {                                  /* LDHCNR       */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short HDCN:8;      /*    HDCN      */
                            unsigned short HTCN:8;      /*    HTCN      */
                            } BIT;                      /*              */
                     } LDHCNR;                          /*              */
               union {                                  /* LDHSYNR      */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short HSYNW:4;     /*    HSYNW     */
                            unsigned short      :4;     /*              */
                            unsigned short HSYNP:8;     /*    HSYNP     */
                            }       BIT;                /*              */
                     }          LDHSYNR;                /*              */
               union {                                  /* LDVDLNR      */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short     : 5;     /*              */
                            unsigned short VDLN:11;     /*    VDLN      */
                            }       BIT;                /*              */
                     }          LDVDLNR;                /*              */
               union {                                  /* LDVTLNR      */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short     : 5;     /*              */
                            unsigned short VTLN:11;     /*    VTLN      */
                            }       BIT;                /*              */
                     }          LDVTLNR;                /*              */
               union {                                  /* LDVSYNR      */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short VSYNW: 4;    /*    VSYNW     */
                            unsigned short      : 1;    /*              */
                            unsigned short VSYNP:11;    /*    VSYNP     */
                            }       BIT;                /*              */
                     }          LDVSYNR;                /*              */
               union {                                  /* LDACLNR      */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short     :11;     /*              */
                            unsigned short ACLN: 5;     /*    ACLN      */
                            }       BIT;                /*              */
                     }          LDACLNR;                /*              */
               union {                                  /* LDINTR       */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short        :3;   /*              */
                            unsigned short VINTSEL:1;   /*    VINTSEL   */
                            unsigned short        :3;   /*              */
                            unsigned short VINTE  :1;   /*    VINTE     */
                            unsigned short        :7;   /*              */
                            unsigned short VINTS  :1;   /*    VINTS     */
                            }       BIT;                /*              */
                     }          LDINTR;                 /*              */
               char             wk[2];                  /*              */
               union {                                  /* LDPMMR       */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short ONC :4;      /*    ONC       */
                            unsigned short OFFD:4;      /*    OFFD      */
                            unsigned short     :1;      /*              */
                            unsigned short VCPE:1;      /*    VCPE      */
                            unsigned short VEPE:1;      /*    VEPE      */
                            unsigned short DONE:1;      /*    DONE      */
                            unsigned short     :2;      /*              */
                            unsigned short LPS :2;      /*    LPS       */
                            }       BIT;                /*              */
                     }          LDPMMR;                 /*              */
               union {                                  /* LDPSPR       */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short ONA :4;      /*    ONA       */
                            unsigned short ONB :4;      /*    ONB       */
                            unsigned short OFFE:4;      /*    OFFE      */
                            unsigned short OFFF:4;      /*    OFFF      */
                            }       BIT;                /*              */
                     }          LDPSPR;                 /*              */
               union {                                  /* LDCNTR       */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short     :11;     /*              */
                            unsigned short DON2: 1;     /*    DON2      */
                            unsigned short     : 3;     /*              */
                            unsigned short DON : 1;     /*    DON       */
                            }       BIT;                /*              */
                     }          LDCNTR;                 /*              */
};                                                      /*              */



struct st_ubc {                                         /* struct UBC   */
              unsigned char     BASRA;                  /* BASRA        */
              char              wk1[3];                 /*              */
              unsigned char     BASRB;                  /* BASRB Address*/
              char              wk2[2097127];           /*              */
              void             *BARA;                   /* BARA         */
              unsigned char     BAMRA;                  /* BAMRA        */
              char              wk3[3];                 /*              */
              unsigned short    BBRA;                   /* BBRA         */
              char              wk4[2];                 /*              */
              void             *BARB;                   /* BARB         */
              unsigned char     BAMRB;                  /* BAMRB        */
              char              wk5[3];                 /*              */
              unsigned short    BBRB;                   /* BBRB         */
              char              wk6[2];                 /*              */
              unsigned int      BDRB;                   /* BDRB         */
              unsigned int      BDMRB;                  /* BDMRB        */
              union {                                   /* BRCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short CMFA:1;       /*    CMFA      */
                           unsigned short CMFB:1;       /*    CMFB      */
                           unsigned short     :3;       /*              */
                           unsigned short PCBA:1;       /*    PCBA      */
                           unsigned short     :2;       /*              */
                           unsigned short DBEB:1;       /*    DBEB      */
                           unsigned short PCBB:1;       /*    PCBB      */
                           unsigned short     :2;       /*              */
                           unsigned short SEQ :1;       /*    SEQ       */
                           unsigned short     :2;       /*              */
                           unsigned short UBDE:1;       /*    UBDE      */
                           }       BIT;                 /*              */
                    }           BRCR;                   /*              */
};                                                      /*              */
struct st_hudi {                                        /* struct H-UDI */
               union {                                  /* SDIR         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short TI:8;        /*    TI        */
                            }       BIT;                /*              */
                     }          SDIR;                   /*              */
               char             wk1[6];                 /*              */
               unsigned int     SDDR;                   /* SDDR         */
               char             wk2[8];                 /*              */
               union {                                  /* SDINT        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short       :15;   /*              */
                            unsigned short INTREQ: 1;   /*   INTREQ     */
                            }       BIT;                /*              */
                     }          SDINT;                  /*              */
};                                                      /*              */
#define MMU    (*(volatile struct st_mmu   *)0xFF000000)/* MMU   Address*/
#define CACHE  (*(volatile struct st_cache *)0xFF00001C)/* CACHE Address*/
#define EXP    (*(volatile struct st_exp   *)0xFF000020)/* EXP   Address*/
#define INTC   (*(volatile struct st_intc  *)0xFE080000)/* INTC  Address*/
#define BSC    (*(volatile struct st_bsc   *)0xFE0A0028)/* BSC   Address*/
#define DMAC   (*(volatile struct st_dmac  *)0xFE090000)/* DMAC  Address*/
#define DMAC0  (*(volatile struct st_dmac0 *)0xFFA00000)/* DMAC0 Address*/
#define DMAC1  (*(volatile struct st_dmac0 *)0xFFA00010)/* DMAC1 Address*/
#define DMAC2  (*(volatile struct st_dmac0 *)0xFFA00020)/* DMAC2 Address*/
#define DMAC3  (*(volatile struct st_dmac0 *)0xFFA00030)/* DMAC3 Address*/
#define DMAC4  (*(volatile struct st_dmac0 *)0xFFA00050)/* DMAC4 Address*/
#define DMAC5  (*(volatile struct st_dmac0 *)0xFFA00060)/* DMAC5 Address*/
#define DMAC6  (*(volatile struct st_dmac0 *)0xFFA00070)/* DMAC6 Address*/
#define DMAC7  (*(volatile struct st_dmac0 *)0xFFA00080)/* DMAC7 Address*/
#define CPG    (*(volatile struct st_cpg   *)0xFE0A0020)/* CPG   Address*/
#define WDT    (*(volatile union  un_wdt   *)0xFFC00008)/* WDT   Address*/
#define STB    (*(volatile struct st_stb   *)0xFFC00004)/* STB   Address*/
#define CLK    (*(volatile struct st_clk   *)0xFE0A0000)/* CLK   Address*/
#define TMU    (*(volatile struct st_tmu   *)0xFFD80004)/* TMU   Address*/
#define TMU0   (*(volatile struct st_tmu0  *)0xFFD80008)/* TMU0  Address*/
#define TMU1   (*(volatile struct st_tmu0  *)0xFFD80014)/* TMU1  Address*/
#define TMU2   (*(volatile struct st_tmu2  *)0xFFD80020)/* TMU2  Address*/
#define CMT    (*(volatile struct st_cmt   *)0xFE1C0000)/* CMT   Address*/
#define CMT0   (*(volatile struct st_cmt0  *)0xFE1C0010)/* CMT0  Address*/
#define CMT1   (*(volatile struct st_cmt0  *)0xFE1C0014)/* CMT1  Address*/
#define CMT2   (*(volatile struct st_cmt0  *)0xFE1C0018)/* CMT2  Address*/
#define CMT3   (*(volatile struct st_cmt0  *)0xFE1C001C)/* CMT3  Address*/
#define SCIF0  (*(volatile struct st_scif  *)0xFE600000)/* SCIF0 Address*/
#define SCIF1  (*(volatile struct st_scif  *)0xFE610000)/* SCIF1 Address*/
#define SCIF2  (*(volatile struct st_scif  *)0xFE620000)/* SCIF2 Address*/
#define SIM    (*(volatile struct st_sim   *)0xFE480000)/* SIM   Address*/
#define IIC0   (*(volatile struct st_iic   *)0xFE140000)/* IIC0  Address*/
#define IIC1   (*(volatile struct st_iic   *)0xFE150000)/* IIC1  Address*/
#define ICRXD  UN_IIC.REG_ICRXD                         /* ICRXD Change */
#define ICTXD  UN_IIC.REG_ICTXD                         /* ICTXD Change */
#define SSI0   (*(volatile struct st_ssi   *)0xFE680000)/* SSI0  Address*/
#define SSI1   (*(volatile struct st_ssi   *)0xFE690000)/* SSI1  Address*/
#define USB    (*(volatile struct st_usb   *)0xFE340000)/* USB   Address*/
#define HcRevision          HR                          /* HR    Change */
#define HcControl           HC                          /* HC    Change */
#define HcCommandStatus     HCS                         /* HCS   Change */
#define HcInterruptStatus   HIS                         /* HIS   Change */
#define HcInterruptEnable   HIE                         /* HIE   Change */
#define HcInterruptDisable  HID                         /* HID   Change */
#define HcHCCA              HHCCA                       /* HCCA  Change */
#define HcPeriodCurrentED   HPCED                       /* HPCED Change */
#define HcControlHeadED     HCHED                       /* HCHED Change */
#define HcControlCurrentED  HCCED                       /* HCCED Change */
#define HcBulkHeadED        HBHED                       /* HBHED Change */
#define HcBulkCurrentED     HBCED                       /* HBCED Change */
#define HcDoneHead          HDH                         /* HDH   Change */
#define HcFmInterval        HFI                         /* HFI   Change */
#define HcFmRemaining       HFR                         /* HFR   Change */
#define HcFmNumber          HFN                         /* HFN   Change */
#define HcPeriodicStart     HPS                         /* HPS   Change */
#define HcLSThreshold       HLST                        /* HLST  Change */
#define HcRhDescriptorA     HRDA                        /* HRDA  Change */
#define HcRhDescriptorB     HRDB                        /* HRDB  Change */
#define HcRhStatus          HRS                         /* HRS   Change */
#define HcRhPortStatus1     HRPS1                       /* HRPS1 Change */
#define HCAN20 (*(volatile struct st_hcan2 *)0xFE380000)/* HCAN2 Address*/
#define HCAN21 (*(volatile struct st_hcan2 *)0xFE390000)/* HCAN2 Address*/
#define HSPI   (*(volatile struct st_hspi  *)0xFE180000)/* HSPI  Address*/
#define PFC    (*(volatile struct st_pfc   *)0xFE400000)/* PFC   Address*/
#define HAC0   (*(volatile struct st_hac   *)0xFE240008)/* HAC0  Address*/
#define HAC1   (*(volatile struct st_hac   *)0xFE250008)/* HAC1  Address*/
#define MMCIF  (*(volatile struct st_mmcif *)0xFE500000)/* MMCIF Address*/
#define MFI    (*(volatile struct st_mfi   *)0xFE2C0000)/* MFI   Address*/
#define ADC    (*(volatile struct st_adc   *)0xFE280000)/* ADC   Address*/
#define LCDC   (*(volatile struct st_lcdc  *)0xFE300800)/* LCDC  Address*/
#define UBC    (*(volatile struct st_ubc   *)0xFF000014)/* UBC   Address*/
#define HUDI   (*(volatile struct st_hudi  *)0xFFF00000)/* H-UDI Address*/


#endif /*__INCLUDE_SH7760_H__*/


/*----------------------------------------------------------------------
#|History of "sh7760.h"
#|=====================
#|* 2015/12/02	[app_sh7760]用に新規作成。（IOPモニターの"7760.h"を流用）
#|* 2015/12/03	本ﾍｯﾀﾞはﾙﾈｻｽのｺﾝﾊﾟｲﾗのものを流用しているが、ﾋﾞｯﾄﾌｨｰﾙﾄﾞの扱いが
#|  GCCの期待しているのと反対。ﾙﾈｻｽｺﾝﾊﾟｲﾗでは#pragma bit_order {left/right}
#|  で変更できるが、GCCではそれができないので、ﾋﾞｯﾄﾌｨｰﾙﾄﾞの記述を最下位ビット
#|  から並ぶように手修正。(BIT_ORDER_RIGHT)
#|
*/
