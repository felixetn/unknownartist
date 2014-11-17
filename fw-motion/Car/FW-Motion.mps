Local Symbol List                                                                                   2014-11-11 19:55:42  Page:     1

Module(s)
   1. androidbtcontrol
   2. linesensordemo
   3. main
   4. mode_controller
   5. modes
   6. scanf_stdarg
   7. MB96348HS
   8. STARTUP
   9. vectors
  10. croutine
  11. list
  12. queue
  13. tasks
  14. timers
  15. __std_lib_sbrk
  16. port
  17. heap_3
  18. wireless
  19. bluetooth
  20. amb8420
  21. adc
  22. api
  23. battery
  24. carid
  25. drive
  26. fram
  27. i2c
  28. interrupts
  29. linesensor
  30. printf_stdarg
  31. regler
  32. runtime_stats
  33. seg
  34. serial
  35. speed
  36. trigonometry
  37. us
  38. us_i2c
  39. util
  40. __STD_LIB_divl
  41. __STD_LIB_divul
  42. __STD_LIB_fadto_
  43. __STD_LIB_fafaddl
  44. __STD_LIB_fafdivl
  45. __STD_LIB_fafmull
  46. __STD_LIB_fa_itod
  47. __STD_LIB_fa_ltod
  48. __STD_LIB_modul
  49. __STD_LIB_mull
  50. __STD_LIB_mulul
  51. __STD_LIB_fashl
  52. __STD_LIB_fashr
  53. __STD_LIB_free
  54. __STD_LIB_malloc
  55. __STD_LIB_memcpy
  56. __STD_LIB_memset
  57. __STD_LIB_pow
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:     2

  58. __STD_LIB_strcat
  59. __STD_LIB_strlen
  60. __STD_LIB_strncpy
  61. __STD_LIB_fafcmpl
  62. __STD_LIB_balloc
  63. __STD_LIB_math
  64. __STD_LIB_noreentrant
  65. __STD_LIB_errno
  66. __STD_LIB_otherinit
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:     3

 === Module No. 1 (androidbtcontrol) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 motorControlTest_bt     Func.   g 0xFE0000
 calibrate_us_bt         Func.   g 0xFE002B
   val                   loc.    s 0x002240
 calibrate_ls_bt         Func.   g 0xFE00C8
 bt_send_sensor_data     Func.   g 0xFE00CC
 Init_AndroidBTContro>   Func.   g 0xFE017E

 === Module No. 2 (linesensordemo) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 Init_LineSensorDemo     Func.   g 0xFE01C3
 laneChange              Func.   g 0xFE01C4
 runDemoLine             Func.   g 0xFE01F8
 initialSpeed            Var.    g 0xFF8040
 steer                   Var.    s 0x002250
 steerP                  Var.    s 0x00224C
 steerD                  Var.    s 0x00224A
 steerI                  Var.    s 0x002246
 integ                   Var.    s 0x002242
 xHandleSteerToLine      Var.    s 0x0023E0
 Kd                      Var.    s 0xFF803E
 Ki                      Var.    s 0xFF803C

 === Module No. 3 (main) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 testtask                Func.   s 0xFE02BF
 main                    Func.   g 0xFE0322
 Kp                      Var.    g 0x0023F2
 Ki                      Var.    g 0x0023EA
 Kd                      Var.    g 0x0023E2

 === Module No. 4 (mode_controller) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 initModeController      Func.   g 0xFE0356
 startModeController     Func.   g 0xFE0357
 stopModeController      Func.   g 0xFE0358
 ModeControllerThread    Func.   g 0xFE0364
 registerMode            Func.   g 0xFE040E
 switchMode              Func.   g 0xFE0437
 xHandleModeControlle>   Var.    g 0x0023FA
 currentMode             Var.    g 0x0023FC
 startModeHandlers       Var.    g 0x002264
 stopModeHandlers        Var.    g 0x002254

 === Module No. 5 (modes) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 registerModes           Func.   g 0xFE04A5

 === Module No. 6 (scanf_stdarg) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 scanWhitespaces         Func.   g 0xFE04A6
 scanWord                Func.   g 0xFE04DD

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __scr1                  Addr.   g 0x0000CB
 __ddr04                 Addr.   g 0x000434
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:     4

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 PIER04                  EQU     s 0x00000448
 __bapm5                 Addr.   g 0x000129
 __fmwc0                 Addr.   g 0x0003F8
 PFD2                    EQU     s 0x000003D4
 __occp1                 Addr.   g 0x00002C
 __pcnl5                 Addr.   g 0x0000AA
 __esir3                 Addr.   g 0x0000E6
 EPILR00                 EQU     s 0x0000046C
 INTPND0                 EQU     s 0x000007A0
 TREQR1H1                EQU     s 0x00000881
 __pucr04                Addr.   g 0x0004AC
 __treqr1h0              Addr.   g 0x000781
 CUTR1H                  EQU     s 0x000004F1
 BGRH0                   EQU     s 0x000000C7
 CUTR2H                  EQU     s 0x000004EF
 BGRH1                   EQU     s 0x000000D1
 __if1mctrl1             Addr.   g 0x00081C
 BGRH2                   EQU     s 0x000000DB
 __pcnh8                 Addr.   g 0x00057F
 __pcn10                 Addr.   g 0x00058E
 __if2mctrl1             Addr.   g 0x00084C
 PCNL15                  EQU     s 0x000005BA
 __adcrl                 Addr.   g 0x00001A
 __tmcsr2                Addr.   g 0x000068
 BGRH3                   EQU     s 0x000000E5
 __treqr1l0              Addr.   g 0x000780
 __coar                  Addr.   g 0x000415
 __escr1                 Addr.   g 0x0000CF
 DISEL0                  EQU     s 0x00000380
 __gcn1l2                Addr.   g 0x000574
 BGRH7                   EQU     s 0x00000545
 IF1MSK11                EQU     s 0x00000814
 IF1DTA11                EQU     s 0x0000081E
 __ddr02                 Addr.   g 0x000432
 BGRH8                   EQU     s 0x0000054F
 SCR3                    EQU     s 0x000000DF
 __bapm3                 Addr.   g 0x000119
 BGRH9                   EQU     s 0x00000559
 EAS4                    EQU     s 0x000006EE
 __pfd3                  Addr.   g 0x0003D6
 EPSR03                  EQU     s 0x000004BF
 BGR9                    EQU     s 0x00000558
 PCSR15                  EQU     s 0x000005B6
 INTPND10                EQU     s 0x000007A0
 __pcnl3                 Addr.   g 0x000096
 __itba1                 Addr.   g 0x0000B8
 __esir1                 Addr.   g 0x0000D2
 TMCSRL2                 EQU     s 0x00000068
 OCS5                    EQU     s 0x00000035
 __dct2                  Addr.   g 0x000116
 __if1cmskh0             Addr.   g 0x000713
 NEWDT2H1                EQU     s 0x00000893
 ITMKH1                  EQU     s 0x000000BB
 __pdut11                Addr.   g 0x000594
 __if2cmskh0             Addr.   g 0x000743
 __pcnh6                 Addr.   g 0x00056B
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:     5

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 TCCSH1                  EQU     s 0x00000027
 IF2MCTRL0               EQU     s 0x0000074C
 ITMKL1                  EQU     s 0x000000BA
 __pcsr11                Addr.   g 0x000592
 PUCR03                  EQU     s 0x000004AB
 IF2MCTRL1               EQU     s 0x0000084C
 TMRLR6                  EQU     s 0x00000072
 ITBAH1                  EQU     s 0x000000B9
 __if1dta1h0             Addr.   g 0x00071F
 TCCSL1                  EQU     s 0x00000026
 __if1dta2h0             Addr.   g 0x000721
 __if2dta1h0             Addr.   g 0x00074F
 __ddr00                 Addr.   g 0x000430
 __if2dta2h0             Addr.   g 0x000751
 __ocs1                  Addr.   g 0x000029
 __bapm1                 Addr.   g 0x000109
 __msgval11              Addr.   g 0x0008B0
 BRPERL1                 EQU     s 0x0000080C
 IF1CMSK1                EQU     s 0x00000812
 ITBAL1                  EQU     s 0x000000B8
 __msgval21              Addr.   g 0x0008B2
 PFD6                    EQU     s 0x000003DC
 __pcnl1                 Addr.   g 0x000086
 __ibcr0                 Addr.   g 0x0000AD
 TMCSRL0                 EQU     s 0x00000060
 __pucr08                Addr.   g 0x0004B0
 __testr1                Addr.   g 0x00080A
 BGRL0                   EQU     s 0x000000C6
 __pcnh4                 Addr.   g 0x0000A3
 IPCP0                   EQU     s 0x00000042
 TMCSRH3                 EQU     s 0x0000006D
 BGRL1                   EQU     s 0x000000D0
 __adcrh                 Addr.   g 0x00001B
 __tccs0                 Addr.   g 0x000022
 __idar1                 Addr.   g 0x0000BE
 IPCP1                   EQU     s 0x00000044
 BGRL2                   EQU     s 0x000000DA
 __tmcsr6                Addr.   g 0x000070
 IPCP2                   EQU     s 0x00000048
 BGRL3                   EQU     s 0x000000E4
 IPCP3                   EQU     s 0x0000004A
 PILR03                  EQU     s 0x0000045B
 IPCP4                   EQU     s 0x0000004E
 PCN3                    EQU     s 0x00000096
 __phdr10                Addr.   g 0x00049E
 __if1mctrh0             Addr.   g 0x00071D
 IPCP5                   EQU     s 0x00000050
 __pllcrh                Addr.   g 0x000407
 __gcn1h2                Addr.   g 0x000575
 __if2mctrh0             Addr.   g 0x00074D
 IPCP6                   EQU     s 0x00000054
 BGRL7                   EQU     s 0x00000544
 IPCP7                   EQU     s 0x00000056
 BGRL8                   EQU     s 0x0000054E
 PDUT10                  EQU     s 0x0000058C
 TMCSR2                  EQU     s 0x00000068
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:     6

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __eac5                  Addr.   g 0x0006EA
 PODR05                  EQU     s 0x00000485
 SCR7                    EQU     s 0x0000053F
 BGRL9                   EQU     s 0x00000558
 TREQR2L0                EQU     s 0x00000782
 MTCRA                   EQU     s 0x000003F2
 __treqr0                Addr.   g 0x000780
 MTCRB                   EQU     s 0x000003F6
 __ibsr0                 Addr.   g 0x0000AC
 IF2MSK2H0               EQU     s 0x00000747
 IF2DTA2H0               EQU     s 0x00000751
 IOA3                    EQU     s 0x0000011C
 IF2MSK2H1               EQU     s 0x00000847
 IF2DTA2H1               EQU     s 0x00000851
 __prrr9                 Addr.   g 0x0004DF
 __cutr2                 Addr.   g 0x0004EE
 __pcnh2                 Addr.   g 0x00008F
 TMCSRH1                 EQU     s 0x00000065
 __gcn13                 Addr.   g 0x000598
 __pdut15                Addr.   g 0x0005B8
 __msgval1l0             Addr.   g 0x0007B0
 __msgval2l0             Addr.   g 0x0007B2
 IPCPH4                  EQU     s 0x0000004F
 SSR0                    EQU     s 0x000000C3
 __epsr03                Addr.   g 0x0004BF
 __pcn8                  Addr.   g 0x00057E
 __pcsr15                Addr.   g 0x0005B6
 IF2DTB1                 EQU     s 0x00000852
 IPCPL4                  EQU     s 0x0000004E
 __wtbr1                 Addr.   g 0x0004E2
 __eacl5                 Addr.   g 0x0006EA
 EBAE0                   EQU     s 0x000006F2
 EBAE1                   EQU     s 0x000006F3
 IF1ARB21                EQU     s 0x0000081A
 IF2ARB11                EQU     s 0x00000848
 __dmacs3                Addr.   g 0x00011B
 __eas5                  Addr.   g 0x0006EF
 EBAE2                   EQU     s 0x000006F4
 __bgr2                  Addr.   g 0x0000DA
 WTCRH                   EQU     s 0x000004E9
 CUTDH                   EQU     s 0x000004ED
 CTRLRL0                 EQU     s 0x00000700
 IF1MSK1H0               EQU     s 0x00000715
 IF1DTA1H0               EQU     s 0x0000071F
 __itmkl0                Addr.   g 0x0000B0
 __intrl0                Addr.   g 0x000708
 __if1cmsk0              Addr.   g 0x000712
 BRPERH0                 EQU     s 0x0000070D
 INTRH1                  EQU     s 0x00000809
 IF1MSK1H1               EQU     s 0x00000815
 IF1DTA1H1               EQU     s 0x0000081F
 __pier01                Addr.   g 0x000445
 __if2cmsk0              Addr.   g 0x000742
 PIER07                  EQU     s 0x0000044B
 __prrr7                 Addr.   g 0x0004DD
 __pcnh0                 Addr.   g 0x00007F
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:     7

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __gcn11                 Addr.   g 0x000098
 __ssr9                  Addr.   g 0x000555
 EDSU2H                  EQU     s 0x000003AD
 CKFCRL                  EQU     s 0x00000404
 WTCRL                   EQU     s 0x000004E8
 CUTDL                   EQU     s 0x000004EC
 __tmr1                  Addr.   g 0x000066
 __vrcr                  Addr.   g 0x00042C
 INTRL1                  EQU     s 0x00000808
 __ptmr8                 Addr.   g 0x000578
 __pcnl12                Addr.   g 0x0005A2
 __if1dta10              Addr.   g 0x00071E
 IF2CREQ0                EQU     s 0x00000740
 IF2DTB20                EQU     s 0x00000754
 ITMK0                   EQU     s 0x000000B0
 __if1dta20              Addr.   g 0x000720
 __if2dta10              Addr.   g 0x00074E
 IF1DTB0                 EQU     s 0x00000722
 ITMK1                   EQU     s 0x000000BA
 __pilr01                Addr.   g 0x000459
 __if2dta20              Addr.   g 0x000750
 __msgval1h1             Addr.   g 0x0008B1
 PCN7                    EQU     s 0x00000572
 TCCS0                   EQU     s 0x00000022
 __pllcrl                Addr.   g 0x000406
 __mctcr                 Addr.   g 0x000409
 __aecsr0                Addr.   g 0x000561
 __eacl3                 Addr.   g 0x0006E6
 __msgval2h1             Addr.   g 0x0008B3
 TCCS1                   EQU     s 0x00000026
 __errcnth0              Addr.   g 0x000705
 TREQR2H0                EQU     s 0x00000783
 ITBA0                   EQU     s 0x000000AE
 __wtcr                  Addr.   g 0x0004E8
 ITBA1                   EQU     s 0x000000B8
 __pfam7                 Addr.   g 0x0003CE
 __errcntl0              Addr.   g 0x000704
 DISEL3                  EQU     s 0x00000383
 __tdr2                  Addr.   g 0x0000D6
 ADCR                    EQU     s 0x0000001A
 __disel2                Addr.   g 0x000382
 IF2MSK2L0               EQU     s 0x00000746
 IF2DTA2L0               EQU     s 0x00000750
 __ice45                 Addr.   g 0x00004D
 IF1CMSKH0               EQU     s 0x00000713
 IF2MSK2L1               EQU     s 0x00000846
 IF2DTA2L1               EQU     s 0x00000850
 __prrr5                 Addr.   g 0x0004DB
 __wthr                  Addr.   g 0x0004E5
 __pcnh10                Addr.   g 0x00058F
 IF1CMSKH1               EQU     s 0x00000813
 __btr1                  Addr.   g 0x000806
 __pfd6                  Addr.   g 0x0003DC
 EPSR06                  EQU     s 0x000004C2
 CTRLRH1                 EQU     s 0x00000801
 __ipcpl1                Addr.   g 0x000044
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:     8

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __ptmr6                 Addr.   g 0x000564
 __gcn2l1                Addr.   g 0x00009A
 __dct5                  Addr.   g 0x00012E
 __if1mctr0              Addr.   g 0x00071C
 IF1DTB10                EQU     s 0x00000722
 RDR2                    EQU     s 0x000000D6
 __wtmr                  Addr.   g 0x0004E4
 __if2mctr0              Addr.   g 0x00074C
 __epsr07                Addr.   g 0x0004C3
 __ader3                 Addr.   g 0x0004D3
 TDR2                    EQU     s 0x000000D6
 __eacl1                 Addr.   g 0x0006E2
 NEWDT10                 EQU     s 0x00000790
 WTBRH0                  EQU     s 0x000004E1
 __wtsr                  Addr.   g 0x0004E3
 PUCR06                  EQU     s 0x000004AE
 __pfam5                 Addr.   g 0x0003C8
 __each4                 Addr.   g 0x0006E9
 __ics45                 Addr.   g 0x00004C
 __itmkh0                Addr.   g 0x0000B1
 __rdr2                  Addr.   g 0x0000D6
 __intrh0                Addr.   g 0x000709
 IF1MSK1L0               EQU     s 0x00000714
 IF1DTA1L0               EQU     s 0x0000071E
 WTBRL0                  EQU     s 0x000004E0
 IF1MSK1L1               EQU     s 0x00000814
 IF1DTA1L1               EQU     s 0x0000081E
 __ocs4                  Addr.   g 0x000034
 __pier05                Addr.   g 0x000449
 __prrr3                 Addr.   g 0x0004D9
 TBRH                    EQU     s 0x000003A3
 __dctl5                 Addr.   g 0x00012E
 PCNH10                  EQU     s 0x0000058F
 __ptmr4                 Addr.   g 0x00009C
 __ckssr                 Addr.   g 0x000402
 __wtcer                 Addr.   g 0x0004E6
 BAPM0                   EQU     s 0x00000101
 __bapl4                 Addr.   g 0x000120
 BAPM1                   EQU     s 0x00000109
 BAPM2                   EQU     s 0x00000111
 __ader1                 Addr.   g 0x0004D1
 __msgval0               Addr.   g 0x0007B0
 BAPM3                   EQU     s 0x00000119
 __pilr05                Addr.   g 0x00045D
 __msgval1               Addr.   g 0x0008B0
 GCN2H2                  EQU     s 0x00000577
 EAC0                    EQU     s 0x000006E0
 BAPM4                   EQU     s 0x00000121
 SMR1                    EQU     s 0x000000CA
 BAPM5                   EQU     s 0x00000129
 __btrl0                 Addr.   g 0x000706
 RCCSR                   EQU     s 0x0000040D
 TMR1                    EQU     s 0x00000066
 __ctrlrh0               Addr.   g 0x000701
 __pdut9                 Addr.   g 0x000584
 __ctrlrh1               Addr.   g 0x000801
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:     9

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 PILR06                  EQU     s 0x0000045E
 GCN2L2                  EQU     s 0x00000576
 __pfam3                 Addr.   g 0x0003C2
 __each2                 Addr.   g 0x0006E5
 __brper1                Addr.   g 0x00080C
 COAR                    EQU     s 0x00000415
 GCN1H0                  EQU     s 0x00000075
 DMACS0                  EQU     s 0x00000103
 __pcn0                  Addr.   g 0x00007E
 __prrr1                 Addr.   g 0x0004D7
 __pcsr9                 Addr.   g 0x000582
 PDUT13                  EQU     s 0x000005A8
 IF1CMSKL0               EQU     s 0x00000712
 IF1ARB2H0               EQU     s 0x0000071B
 IF2ARB1H0               EQU     s 0x00000749
 __bgrl9                 Addr.   g 0x000558
 __pcnh14                Addr.   g 0x0005B3
 PODR08                  EQU     s 0x00000488
 ADER0                   EQU     s 0x000004D0
 ERRCNTL0                EQU     s 0x00000704
 IF1CMSKL1               EQU     s 0x00000812
 IF1ARB2H1               EQU     s 0x0000081B
 IF2ARB1H1               EQU     s 0x00000849
 GCN1L0                  EQU     s 0x00000074
 __dctl3                 Addr.   g 0x00011E
 ADER1                   EQU     s 0x000004D1
 __ipcph1                Addr.   g 0x000045
 __ptmr2                 Addr.   g 0x000088
 ADER2                   EQU     s 0x000004D2
 INTR0                   EQU     s 0x00000708
 __ipcpl5                Addr.   g 0x000050
 __gcn2h1                Addr.   g 0x00009B
 __smr8                  Addr.   g 0x000548
 ADER3                   EQU     s 0x000004D3
 INTR1                   EQU     s 0x00000808
 __bapl2                 Addr.   g 0x000110
 ADER4                   EQU     s 0x000004D4
 __adcr                  Addr.   g 0x00001A
 __mtcrb                 Addr.   g 0x0003F6
 IF1MCTR0                EQU     s 0x0000071C
 SSR8                    EQU     s 0x0000054B
 STATR0                  EQU     s 0x00000702
 IPCPH7                  EQU     s 0x00000057
 __baph5                 Addr.   g 0x00012A
 __ssr1                  Addr.   g 0x0000CD
 __pdut7                 Addr.   g 0x000570
 __if1arb1l1             Addr.   g 0x000818
 __pfam1                 Addr.   g 0x0003BC
 __each0                 Addr.   g 0x0006E1
 __if1arb2l1             Addr.   g 0x00081A
 __if2arb1l1             Addr.   g 0x000848
 __tmcsrh0               Addr.   g 0x000061
 __if2arb2l1             Addr.   g 0x00084A
 __tmcsrh1               Addr.   g 0x000065
 IPCPL7                  EQU     s 0x00000056
 __btrh1                 Addr.   g 0x000807
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    10

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __tmcsrh2               Addr.   g 0x000069
 __podr03                Addr.   g 0x000483
 __pcsr7                 Addr.   g 0x00056E
 COER0                   EQU     s 0x000007CE
 __tmcsrh3               Addr.   g 0x00006D
 __pier09                Addr.   g 0x00044D
 __bgrl7                 Addr.   g 0x000544
 COER1                   EQU     s 0x000008CE
 __dctl1                 Addr.   g 0x00010E
 TBRL                    EQU     s 0x000003A2
 __ptmr0                 Addr.   g 0x000078
 PCNL10                  EQU     s 0x0000058E
 __tmcsrh6               Addr.   g 0x000071
 PRRR10                  EQU     s 0x00000660
 __bapl0                 Addr.   g 0x000100
 __scr7                  Addr.   g 0x00053F
 __adsr                  Addr.   g 0x00001C
 __dcth4                 Addr.   g 0x000127
 __statr0                Addr.   g 0x000702
 __pilr09                Addr.   g 0x000461
 EAC4                    EQU     s 0x000006E8
 __baph3                 Addr.   g 0x00011A
 DCT0                    EQU     s 0x00000106
 __newdt2h0              Addr.   g 0x000793
 PFCS0                   EQU     s 0x000003B0
 __pdut5                 Addr.   g 0x0000A8
 BGR2                    EQU     s 0x000000DA
 __ctrlrl0               Addr.   g 0x000700
 PFCS1                   EQU     s 0x000003B2
 __ctrlrl1               Addr.   g 0x000800
 PFCS2                   EQU     s 0x000003B4
 PCSR10                  EQU     s 0x0000058A
 PFCS3                   EQU     s 0x000003B6
 __newdt2l0              Addr.   g 0x000792
 PTMR12                  EQU     s 0x0000059C
 __pcsr5                 Addr.   g 0x0000A6
 COCR0                   EQU     s 0x00000416
 __itmk0                 Addr.   g 0x0000B0
 __ioal4                 Addr.   g 0x000124
 __intr0                 Addr.   g 0x000708
 COCR1                   EQU     s 0x00000417
 ERRCNTH0                EQU     s 0x00000705
 IF1ARB2L0               EQU     s 0x0000071A
 IF2ARB1L0               EQU     s 0x00000748
 __tccsl1                Addr.   g 0x000026
 __ptmr10                Addr.   g 0x000588
 IF1ARB2L1               EQU     s 0x0000081A
 IF2ARB1L1               EQU     s 0x00000848
 BRPER1                  EQU     s 0x0000080C
 __ipcph5                Addr.   g 0x000051
 __icr                   Addr.   g 0x0003A0
 __bgrh8                 Addr.   g 0x00054F
 TMRLR1                  EQU     s 0x00000066
 __dcth2                 Addr.   g 0x000117
 __prrr12                Addr.   g 0x000662
 DERH                    EQU     s 0x00000395
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    11

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __ioa1                  Addr.   g 0x00010C
 IDAR0                   EQU     s 0x000000B4
 __rccsrc                Addr.   g 0x00040B
 EPSR09                  EQU     s 0x000004C5
 TESTRL0                 EQU     s 0x0000070A
 __elvr1                 Addr.   g 0x00005E
 __tmrlr1                Addr.   g 0x000066
 ADCRH                   EQU     s 0x0000001B
 IDAR1                   EQU     s 0x000000BE
 __baph1                 Addr.   g 0x00010A
 __pdut3                 Addr.   g 0x000094
 __ebcf                  Addr.   g 0x0006F1
 __tmcsrl0               Addr.   g 0x000060
 ADCRL                   EQU     s 0x0000001A
 __tmcsrl1               Addr.   g 0x000064
 __pcsr3                 Addr.   g 0x000092
 __pucr01                Addr.   g 0x0004A9
 __cutr2h                Addr.   g 0x0004EF
 __tmcsrl2               Addr.   g 0x000068
 __bgrl3                 Addr.   g 0x0000E4
 __ioal2                 Addr.   g 0x000114
 __podr07                Addr.   g 0x000487
 __if1arb1h0             Addr.   g 0x000719
 WTCKSR                  EQU     s 0x000004E7
 MSGVAL11                EQU     s 0x000008B0
 __tmcsrl3               Addr.   g 0x00006C
 __if1arb2h0             Addr.   g 0x00071B
 __if2arb1h0             Addr.   g 0x000749
 IF2MSK0                 EQU     s 0x00000744
 IF2DTA0                 EQU     s 0x0000074E
 IF1MSK1                 EQU     s 0x00000814
 IF1DTA1                 EQU     s 0x0000081E
 __if2arb2h0             Addr.   g 0x00074B
 PUCR09                  EQU     s 0x000004B1
 __enir0                 Addr.   g 0x000058
 __tmcsrl6               Addr.   g 0x000070
 __ioah5                 Addr.   g 0x00012D
 __ocs7                  Addr.   g 0x00003B
 __dcth0                 Addr.   g 0x000107
 PCNH13                  EQU     s 0x000005AB
 SCR0                    EQU     s 0x000000C1
 __pdut1                 Addr.   g 0x000084
 DCT4                    EQU     s 0x00000126
 __eac0                  Addr.   g 0x0006E0
 PODR00                  EQU     s 0x00000480
 SMR9                    EQU     s 0x00000552
 PODR10                  EQU     s 0x0000048A
 __smr0                  Addr.   g 0x0000C0
 OCS2                    EQU     s 0x0000002E
 __edsu                  Addr.   g 0x0003AF
 TESTRH1                 EQU     s 0x0000080B
 __pcsr1                 Addr.   g 0x000082
 __bgrl1                 Addr.   g 0x0000D0
 __ioal0                 Addr.   g 0x000104
 IF1MSK20                EQU     s 0x00000716
 IF1DTA20                EQU     s 0x00000720
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    12

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 IF2MSK10                EQU     s 0x00000744
 IF2DTA10                EQU     s 0x0000074E
 __tccsh1                Addr.   g 0x000027
 __ptmr14                Addr.   g 0x0005AC
 __if1dtb1l1             Addr.   g 0x000822
 PILR09                  EQU     s 0x00000461
 __if1dtb2l1             Addr.   g 0x000824
 __if2dtb1l1             Addr.   g 0x000852
 __if2dtb2l1             Addr.   g 0x000854
 GCN1H3                  EQU     s 0x00000599
 __ioah3                 Addr.   g 0x00011D
 DMACS3                  EQU     s 0x0000011B
 __pcn3                  Addr.   g 0x000096
 __nmi                   Addr.   g 0x0003A5
 __newdt1                Addr.   g 0x000890
 DERL                    EQU     s 0x00000394
 VRCR                    EQU     s 0x0000042C
 EBCF                    EQU     s 0x000006F1
 __pfal6                 Addr.   g 0x0003CA
 __if1creq1              Addr.   g 0x000810
 GCN1L3                  EQU     s 0x00000598
 __if2creq1              Addr.   g 0x000840
 __pfdl6                 Addr.   g 0x0003DC
 PFD3                    EQU     s 0x000003D6
 __mcsra                 Addr.   g 0x0003F1
 __eccr8                 Addr.   g 0x00054C
 __derh                  Addr.   g 0x000395
 __pucr05                Addr.   g 0x0004AD
 __cutr2l                Addr.   g 0x0004EE
 __treqr2h0              Addr.   g 0x000783
 PIER02                  EQU     s 0x00000446
 IF2CMSK0                EQU     s 0x00000742
 __tmcsr3                Addr.   g 0x00006C
 __treqr2l0              Addr.   g 0x000782
 __isba0                 Addr.   g 0x0000B2
 __bgrh2                 Addr.   g 0x0000DB
 __ioah1                 Addr.   g 0x00010D
 PCN0                    EQU     s 0x0000007E
 __pdr10                 Addr.   g 0x00000A
 NEWDT0                  EQU     s 0x00000790
 __pfal4                 Addr.   g 0x0003C4
 __gcn1l3                Addr.   g 0x000598
 __ckfcr                 Addr.   g 0x000404
 __wdtcp                 Addr.   g 0x00040F
 __bgr8                  Addr.   g 0x00054E
 PCNL13                  EQU     s 0x000005AA
 PRRR13                  EQU     s 0x00000663
 EAS5                    EQU     s 0x000006EF
 __pfdl4                 Addr.   g 0x0003D8
 DIRR                    EQU     s 0x000003A4
 MCTCR                   EQU     s 0x00000409
 INTPND11                EQU     s 0x000008A0
 __der                   Addr.   g 0x000394
 __pfah7                 Addr.   g 0x0003CF
 OCS6                    EQU     s 0x0000003A
 IOA0                    EQU     s 0x00000104
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    13

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __if1arb10              Addr.   g 0x000718
 __if1arb20              Addr.   g 0x00071A
 __if2arb10              Addr.   g 0x000748
 __pfdh7                 Addr.   g 0x0003DF
 __if1creql0             Addr.   g 0x000710
 __if2arb20              Addr.   g 0x00074A
 IBSR0                   EQU     s 0x000000AC
 __pdut12                Addr.   g 0x0005A0
 __if2creql0             Addr.   g 0x000740
 RCTCR                   EQU     s 0x00000408
 ADECR                   EQU     s 0x0000001E
 IBSR1                   EQU     s 0x000000B6
 __pfd1                  Addr.   g 0x0003D2
 EPSR01                  EQU     s 0x000004BD
 PCSR13                  EQU     s 0x000005A6
 IF2ARB1                 EQU     s 0x00000848
 __bgrh0                 Addr.   g 0x0000C7
 __dct0                  Addr.   g 0x000106
 PTMR15                  EQU     s 0x000005B4
 __epsr00                Addr.   g 0x0004BC
 __pcsr12                Addr.   g 0x00059E
 MSGVAL1H0               EQU     s 0x000007B1
 __if1dtb1h0             Addr.   g 0x000723
 __newdt1h1              Addr.   g 0x000891
 MSGVAL1H1               EQU     s 0x000008B1
 __pfal2                 Addr.   g 0x0003BE
 __if1dtb2h0             Addr.   g 0x000725
 __if2dtb1h0             Addr.   g 0x000753
 __if2dtb2h0             Addr.   g 0x000755
 __dmacs0                Addr.   g 0x000103
 __pfdl2                 Addr.   g 0x0003D4
 __tdr8                  Addr.   g 0x00054A
 __newdt1l1              Addr.   g 0x000890
 PFD7                    EQU     s 0x000003DE
 PUCR01                  EQU     s 0x000004A9
 __iccr0                 Addr.   g 0x0000B5
 ICS23                   EQU     s 0x00000046
 __pfah5                 Addr.   g 0x0003C9
 __ioa4                  Addr.   g 0x000124
 __tbrh                  Addr.   g 0x0003A3
 __pfdh5                 Addr.   g 0x0003DB
 __pucr09                Addr.   g 0x0004B1
 OCCP0                   EQU     s 0x0000002A
 IF2ARB20                EQU     s 0x0000074A
 __pdr09                 Addr.   g 0x000009
 OCCP1                   EQU     s 0x0000002C
 IF1ARB0                 EQU     s 0x00000718
 OCCP2                   EQU     s 0x00000030
 __mtcrbh                Addr.   g 0x0003F7
 __ebae2                 Addr.   g 0x0006F4
 OCCP3                   EQU     s 0x00000032
 __if1creqh1             Addr.   g 0x000811
 OCCP4                   EQU     s 0x00000036
 PCN4                    EQU     s 0x000000A2
 __if2creqh1             Addr.   g 0x000841
 OCCP5                   EQU     s 0x00000038
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    14

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __pfal0                 Addr.   g 0x0003B8
 __pier10                Addr.   g 0x00044E
 __gcn1h3                Addr.   g 0x000599
 OCCP6                   EQU     s 0x0000003C
 TREQR21                 EQU     s 0x00000882
 OCCP7                   EQU     s 0x0000003E
 __smcr                  Addr.   g 0x000400
 __newdt10               Addr.   g 0x000790
 DSRH                    EQU     s 0x00000391
 CUCR                    EQU     s 0x000004EA
 __pfdl0                 Addr.   g 0x0003D0
 __rdr8                  Addr.   g 0x00054A
 __newdt11               Addr.   g 0x000890
 SCR8                    EQU     s 0x00000549
 TREQR2L1                EQU     s 0x00000882
 PILR01                  EQU     s 0x00000459
 __eccr2                 Addr.   g 0x0000D8
 __pfah3                 Addr.   g 0x0003C3
 __treqr1                Addr.   g 0x000880
 IOA4                    EQU     s 0x00000124
 __tbr                   Addr.   g 0x0003A2
 __pilr10                Addr.   g 0x000462
 __pfdh3                 Addr.   g 0x0003D7
 ELVRH1                  EQU     s 0x0000005F
 TMCSR0                  EQU     s 0x00000060
 __eac3                  Addr.   g 0x0006E6
 PODR03                  EQU     s 0x00000483
 IF1ARB10                EQU     s 0x00000718
 __pdr07                 Addr.   g 0x000007
 __smr3                  Addr.   g 0x0000DE
 __ebae0                 Addr.   g 0x0006F2
 ELVRL1                  EQU     s 0x0000005E
 __cutdl                 Addr.   g 0x0004EC
 SSR1                    EQU     s 0x000000CD
 __epsr04                Addr.   g 0x0004C0
 CKFCR                   EQU     s 0x00000404
 MSGVAL1L0               EQU     s 0x000007B0
 TBR                     EQU     s 0x000003A2
 MSGVAL1L1               EQU     s 0x000008B0
 PFAH0                   EQU     s 0x000003BA
 PFAH1                   EQU     s 0x000003BD
 FMWC0                   EQU     s 0x000003F8
 IPCPH2                  EQU     s 0x00000049
 PFAH2                   EQU     s 0x000003C0
 FMWC1                   EQU     s 0x000003F9
 __dmacs4                Addr.   g 0x000123
 __pfcs3                 Addr.   g 0x0003B6
 PFAH3                   EQU     s 0x000003C3
 __eccr0                 Addr.   g 0x0000C4
 __pfah1                 Addr.   g 0x0003BD
 __pcn6                  Addr.   g 0x00056A
 PFAH4                   EQU     s 0x000003C6
 PFAH5                   EQU     s 0x000003C9
 __itmkl1                Addr.   g 0x0000BA
 IPCPL2                  EQU     s 0x00000048
 EIRR0                   EQU     s 0x00000059
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    15

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __intrl1                Addr.   g 0x000808
 PFAH6                   EQU     s 0x000003CC
 FMWC5                   EQU     s 0x000003FD
 EIRR1                   EQU     s 0x0000005D
 __pfdh1                 Addr.   g 0x0003D3
 __pier02                Addr.   g 0x000446
 PFAH7                   EQU     s 0x000003CF
 ICS67                   EQU     s 0x00000052
 __eas3                  Addr.   g 0x0006ED
 INTPND1H0               EQU     s 0x000007A1
 __pdr05                 Addr.   g 0x000005
 __bgr0                  Addr.   g 0x0000C6
 __cmprl                 Addr.   g 0x00041A
 INTPND1H1               EQU     s 0x000008A1
 __pcnl13                Addr.   g 0x0005AA
 MTCRAH                  EQU     s 0x000003F3
 NEWDT1L0                EQU     s 0x00000790
 __mtcrbl                Addr.   g 0x0003F6
 __if1dtb10              Addr.   g 0x000722
 MTCRBH                  EQU     s 0x000003F7
 IF2CREQ1                EQU     s 0x00000840
 IF2DTB21                EQU     s 0x00000854
 __scr2                  Addr.   g 0x0000D5
 __if1dtb20              Addr.   g 0x000724
 __if2dtb10              Addr.   g 0x000752
 PIER05                  EQU     s 0x00000449
 __pilr02                Addr.   g 0x00045A
 __if2dtb20              Addr.   g 0x000754
 PCN8                    EQU     s 0x0000057E
 __ssr7                  Addr.   g 0x000541
 __aecsr1                Addr.   g 0x000563
 DSRL                    EQU     s 0x00000390
 EPILR10                 EQU     s 0x00000476
 TREQR2H1                EQU     s 0x00000883
 __pfcs1                 Addr.   g 0x0003B2
 __rccsr                 Addr.   g 0x00040D
 __treqr1h1              Addr.   g 0x000881
 STATRL1                 EQU     s 0x00000802
 ADCS                    EQU     s 0x00000018
 __disel3                Addr.   g 0x000383
 __adcsl                 Addr.   g 0x000018
 __pcnh11                Addr.   g 0x000597
 __treqr1l1              Addr.   g 0x000880
 CTRLR0                  EQU     s 0x00000700
 __pdr03                 Addr.   g 0x000003
 __ipcpl2                Addr.   g 0x000048
 __dsrl                  Addr.   g 0x000390
 DISEL1                  EQU     s 0x00000381
 __tdr0                  Addr.   g 0x0000C2
 __cutdh                 Addr.   g 0x0004ED
 __gcn2l2                Addr.   g 0x000576
 IF1DTB11                EQU     s 0x00000822
 RDR3                    EQU     s 0x000000E0
 __epsr08                Addr.   g 0x0004C4
 AECSR0                  EQU     s 0x00000561
 TDR3                    EQU     s 0x000000E0
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    16

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __pfd4                  Addr.   g 0x0003D8
 PFAL0                   EQU     s 0x000003B8
 EPSR04                  EQU     s 0x000004C0
 PFAL1                   EQU     s 0x000003BB
 __dct3                  Addr.   g 0x00011E
 PFAL2                   EQU     s 0x000003BE
 PFAL3                   EQU     s 0x000003C1
 PFAL4                   EQU     s 0x000003C4
 __ipcp7                 Addr.   g 0x000056
 __itmkh1                Addr.   g 0x0000BB
 __intrh1                Addr.   g 0x000809
 __if1msk1l1             Addr.   g 0x000814
 PFAL5                   EQU     s 0x000003C7
 __podr00                Addr.   g 0x000480
 __treqr10               Addr.   g 0x000780
 __if1msk2l1             Addr.   g 0x000816
 __if2msk1l1             Addr.   g 0x000844
 PFAL6                   EQU     s 0x000003CA
 __pier06                Addr.   g 0x00044A
 __if2msk2l1             Addr.   g 0x000846
 __treqr11               Addr.   g 0x000880
 PFAL7                   EQU     s 0x000003CD
 __pdr01                 Addr.   g 0x000001
 __cmprh                 Addr.   g 0x00041B
 __intpnd1l1             Addr.   g 0x0008A0
 PUCR04                  EQU     s 0x000004AC
 INTPND1L0               EQU     s 0x000007A0
 __intpnd2l1             Addr.   g 0x0008A2
 CMCR                    EQU     s 0x00000418
 NEWDT1H0                EQU     s 0x00000791
 INTPND1L1               EQU     s 0x000008A0
 __rdr0                  Addr.   g 0x0000C2
 __ice23                 Addr.   g 0x000047
 ELVR0                   EQU     s 0x0000005A
 __ddr10                 Addr.   g 0x00043A
 __ocs2                  Addr.   g 0x00002E
 ELVR1                   EQU     s 0x0000005E
 EPILR09                 EQU     s 0x00000475
 __pilr06                Addr.   g 0x00045E
 EAC1                    EQU     s 0x000006E2
 SMR2                    EQU     s 0x000000D4
 TMR2                    EQU     s 0x0000006A
 __acsr0                 Addr.   g 0x000560
 DER                     EQU     s 0x00000394
 PLLCRL                  EQU     s 0x00000406
 __ipcp5                 Addr.   g 0x000050
 GCN2H0                  EQU     s 0x00000077
 __dssrl                 Addr.   g 0x000392
 ERRCNT1                 EQU     s 0x00000804
 __adcsh                 Addr.   g 0x000019
 __tcdt0                 Addr.   g 0x000020
 __pcnh15                Addr.   g 0x0005BB
 BTR0                    EQU     s 0x00000706
 ERRCNTL1                EQU     s 0x00000804
 GCN2L0                  EQU     s 0x00000076
 PILR04                  EQU     s 0x0000045C
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    17

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 STATRH0                 EQU     s 0x00000703
 __ipcph2                Addr.   g 0x000049
 __itbal0                Addr.   g 0x0000AE
 DSSR                    EQU     s 0x00000392
 __ics23                 Addr.   g 0x000046
 __ipcpl6                Addr.   g 0x000054
 __gcn2h2                Addr.   g 0x000577
 SMCR                    EQU     s 0x00000400
 __eirr1                 Addr.   g 0x00005D
 EPILR07                 EQU     s 0x00000473
 RDR7                    EQU     s 0x00000540
 PDUT11                  EQU     s 0x00000594
 IF1MCTR1                EQU     s 0x0000081C
 TMCSR3                  EQU     s 0x0000006C
 __ckfcrh                Addr.   g 0x000405
 PODR06                  EQU     s 0x00000486
 SSR9                    EQU     s 0x00000555
 __ismk1                 Addr.   g 0x0000BD
 TDR7                    EQU     s 0x00000540
 MSGVAL1                 EQU     s 0x000008B0
 WTHR                    EQU     s 0x000004E5
 IF2CREQH0               EQU     s 0x00000741
 IF2DTB2H0               EQU     s 0x00000755
 IF2CREQH1               EQU     s 0x00000841
 IF2DTB2H1               EQU     s 0x00000855
 __ipcp3                 Addr.   g 0x00004A
 __gcn23                 Addr.   g 0x00059A
 __podr04                Addr.   g 0x000484
 __escr8                 Addr.   g 0x00054D
 IPCPH5                  EQU     s 0x00000051
 __ddr09                 Addr.   g 0x000439
 __pcn9                  Addr.   g 0x000586
 __if1msk1h0             Addr.   g 0x000715
 __fmwc5                 Addr.   g 0x0003FD
 __if1msk2h0             Addr.   g 0x000717
 __if2msk1h0             Addr.   g 0x000745
 __occp6                 Addr.   g 0x00003C
 IPCPL5                  EQU     s 0x00000050
 __if2msk2h0             Addr.   g 0x000747
 __esir8                 Addr.   g 0x000550
 EPILR05                 EQU     s 0x00000471
 __pucr10                Addr.   g 0x0004B2
 __intpnd1h0             Addr.   g 0x0007A1
 __statr1                Addr.   g 0x000802
 __intpnd2h0             Addr.   g 0x0007A3
 EAC5                    EQU     s 0x000006EA
 MSGVAL20                EQU     s 0x000007B2
 __bgr3                  Addr.   g 0x0000E4
 __epilr00               Addr.   g 0x00046C
 DCT1                    EQU     s 0x0000010E
 __epilr01               Addr.   g 0x00046D
 IF1DTB1H0               EQU     s 0x00000723
 ICE23                   EQU     s 0x00000047
 TMR6                    EQU     s 0x00000072
 BGR3                    EQU     s 0x000000E4
 __epilr02               Addr.   g 0x00046E
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    18

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __pcn15                 Addr.   g 0x0005BA
 __if1cmsk1              Addr.   g 0x000812
 IF1DTB1H1               EQU     s 0x00000823
 __epilr03               Addr.   g 0x00046F
 __if2cmsk1              Addr.   g 0x000842
 PIER08                  EQU     s 0x0000044C
 __ipcp1                 Addr.   g 0x000044
 __epilr04               Addr.   g 0x000470
 __gcn21                 Addr.   g 0x00009A
 __dssrh                 Addr.   g 0x000393
 __epilr05               Addr.   g 0x000471
 __tmr2                  Addr.   g 0x00006A
 IBCR0                   EQU     s 0x000000AD
 __epilr06               Addr.   g 0x000472
 IBCR1                   EQU     s 0x000000B7
 __epilr07               Addr.   g 0x000473
 __ctrlr0                Addr.   g 0x000700
 ERRCNTH1                EQU     s 0x00000805
 __ddr07                 Addr.   g 0x000437
 __epilr08               Addr.   g 0x000474
 __ptmr11                Addr.   g 0x000590
 __if1dta11              Addr.   g 0x00081E
 __itbah0                Addr.   g 0x0000AF
 __epilr09               Addr.   g 0x000475
 __if1dta21              Addr.   g 0x000820
 __if2dta11              Addr.   g 0x00084E
 CUTR1L                  EQU     s 0x000004F0
 __ipcph6                Addr.   g 0x000055
 __if2dta21              Addr.   g 0x000850
 CUTR2L                  EQU     s 0x000004EE
 TREQR10                 EQU     s 0x00000780
 __occp4                 Addr.   g 0x000036
 PTMR10                  EQU     s 0x00000588
 __pcnl8                 Addr.   g 0x00057E
 __errcnth1              Addr.   g 0x000805
 EPILR03                 EQU     s 0x0000046F
 __adecr                 Addr.   g 0x00001E
 __prrr13                Addr.   g 0x000663
 __ckfcrl                Addr.   g 0x000404
 __tmrlr2                Addr.   g 0x00006A
 ADCSH                   EQU     s 0x00000019
 __testrh0               Addr.   g 0x00070B
 __errcntl1              Addr.   g 0x000804
 DISEL4                  EQU     s 0x00000384
 __tdr3                  Addr.   g 0x0000E0
 __wdtc                  Addr.   g 0x00040E
 __pcn13                 Addr.   g 0x0005AA
 __testrh1               Addr.   g 0x00080B
 PFD0                    EQU     s 0x000003D0
 IF2CREQL0               EQU     s 0x00000740
 IF2DTB2L0               EQU     s 0x00000754
 __idx                   Addr.   g 0x0003A1
 IF2CREQL1               EQU     s 0x00000840
 IF2DTB2L1               EQU     s 0x00000854
 EACH0                   EQU     s 0x000006E1
 ADCSL                   EQU     s 0x00000018
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    19

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __wtbrl0                Addr.   g 0x0004E0
 EACH1                   EQU     s 0x000006E3
 __pfd7                  Addr.   g 0x0003DE
 __pucr02                Addr.   g 0x0004AA
 EPSR07                  EQU     s 0x000004C3
 EACH2                   EQU     s 0x000006E5
 __podr08                Addr.   g 0x000488
 EACH3                   EQU     s 0x000006E7
 __ddr05                 Addr.   g 0x000435
 __if1mctr1              Addr.   g 0x00081C
 EACH4                   EQU     s 0x000006E9
 __if2mctr1              Addr.   g 0x00084C
 EACH5                   EQU     s 0x000006EB
 __tmcsr0                Addr.   g 0x000060
 __fmwc1                 Addr.   g 0x0003F9
 __if1cmskl0             Addr.   g 0x000712
 __occp2                 Addr.   g 0x000030
 __if2cmskl0             Addr.   g 0x000742
 __pcnl6                 Addr.   g 0x00056A
 EPILR01                 EQU     s 0x0000046D
 INTPND1                 EQU     s 0x000008A0
 NEWDT20                 EQU     s 0x00000792
 __gcn1l0                Addr.   g 0x000074
 __cmcr                  Addr.   g 0x000418
 __cutd                  Addr.   g 0x0004EC
 __errcnt0               Addr.   g 0x000704
 PUCR07                  EQU     s 0x000004AF
 SCR1                    EQU     s 0x000000CB
 __if1dta1l0             Addr.   g 0x00071E
 __errcnt1               Addr.   g 0x000804
 EAS2                    EQU     s 0x000006EC
 __rdr3                  Addr.   g 0x0000E0
 DCT5                    EQU     s 0x0000012E
 __pcnh9                 Addr.   g 0x000587
 __pcn11                 Addr.   g 0x000596
 __if1dta2l0             Addr.   g 0x000720
 __if2dta1l0             Addr.   g 0x00074E
 IF1DTB1L0               EQU     s 0x00000722
 __if2dta2l0             Addr.   g 0x000750
 CKSR                    EQU     s 0x00000401
 BGR7                    EQU     s 0x00000544
 IF1MCTRH0               EQU     s 0x0000071D
 IF1DTB1L1               EQU     s 0x00000822
 __ocs5                  Addr.   g 0x000035
 TESTR0                  EQU     s 0x0000070A
 IF1MCTRH1               EQU     s 0x0000081D
 OCS3                    EQU     s 0x0000002F
 __cocr0                 Addr.   g 0x000416
 __escr2                 Addr.   g 0x0000D9
 PCNH11                  EQU     s 0x00000597
 ICE67                   EQU     s 0x00000053
 __coer0                 Addr.   g 0x0007CE
 IF1MSK21                EQU     s 0x00000816
 IF1DTA21                EQU     s 0x00000820
 IF2MSK11                EQU     s 0x00000844
 IF2DTA11                EQU     s 0x0000084E
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    20

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __ddr03                 Addr.   g 0x000433
 __if1msk10              Addr.   g 0x000714
 __bapm4                 Addr.   g 0x000121
 __ptmr15                Addr.   g 0x0005B4
 __if1msk20              Addr.   g 0x000716
 __if2msk10              Addr.   g 0x000744
 __if2msk20              Addr.   g 0x000746
 __occp0                 Addr.   g 0x00002A
 GCN2H3                  EQU     s 0x0000059B
 INTPND20                EQU     s 0x000007A2
 __pcnl4                 Addr.   g 0x0000A2
 __esir2                 Addr.   g 0x0000DC
 TMCSRL3                 EQU     s 0x0000006C
 __cmpr                  Addr.   g 0x00041A
 __intpnd10              Addr.   g 0x0007A0
 __intpnd20              Addr.   g 0x0007A2
 __if1cmskh1             Addr.   g 0x000813
 PILR07                  EQU     s 0x0000045F
 GCN2L3                  EQU     s 0x0000059A
 __if1mctrl0             Addr.   g 0x00071C
 __if2cmskh1             Addr.   g 0x000843
 __tmrlr6                Addr.   g 0x000072
 TMCSRH6                 EQU     s 0x00000071
 GCN1H1                  EQU     s 0x00000099
 __pcnh7                 Addr.   g 0x000573
 __testrl0               Addr.   g 0x00070A
 __if2mctrl0             Addr.   g 0x00074C
 DMACS1                  EQU     s 0x0000010B
 __testrl1               Addr.   g 0x00080A
 PFD4                    EQU     s 0x000003D8
 __pcn1                  Addr.   g 0x000086
 PDUT14                  EQU     s 0x000005B0
 TMCSR6                  EQU     s 0x00000070
 __wtbrh0                Addr.   g 0x0004E1
 PODR09                  EQU     s 0x00000489
 EACL0                   EQU     s 0x000006E0
 __escr0                 Addr.   g 0x0000C5
 GCN1L1                  EQU     s 0x00000098
 __if1dta1h1             Addr.   g 0x00081F
 DSSRH                   EQU     s 0x00000393
 EACL1                   EQU     s 0x000006E2
 DCTH0                   EQU     s 0x00000107
 __pucr06                Addr.   g 0x0004AE
 __if1dta2h1             Addr.   g 0x000821
 __if2dta1h1             Addr.   g 0x00084F
 DSR                     EQU     s 0x00000390
 EACL2                   EQU     s 0x000006E4
 DCTH1                   EQU     s 0x0000010F
 __ddr01                 Addr.   g 0x000431
 __smr9                  Addr.   g 0x000552
 __if2dta2h1             Addr.   g 0x000851
 EACL3                   EQU     s 0x000006E6
 __bapm2                 Addr.   g 0x000111
 DCTH2                   EQU     s 0x00000117
 EACL4                   EQU     s 0x000006E8
 IF2CMSK1                EQU     s 0x00000842
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    21

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __adcs                  Addr.   g 0x000018
 PDUT0                   EQU     s 0x0000007C
 DCTH3                   EQU     s 0x0000011F
 DSSRL                   EQU     s 0x00000392
 EACL5                   EQU     s 0x000006EA
 PDUT1                   EQU     s 0x00000084
 DCTH4                   EQU     s 0x00000127
 __pcnl2                 Addr.   g 0x00008E
 __itba0                 Addr.   g 0x0000AE
 __ibcr1                 Addr.   g 0x0000B7
 __esir0                 Addr.   g 0x0000C8
 TMCSRL1                 EQU     s 0x00000064
 PDUT2                   EQU     s 0x0000008C
 DCTH5                   EQU     s 0x0000012F
 PCN1                    EQU     s 0x00000086
 PDUT3                   EQU     s 0x00000094
 PIER00                  EQU     s 0x00000444
 STATR1                  EQU     s 0x00000802
 __gcn1h0                Addr.   g 0x000075
 PDUT4                   EQU     s 0x000000A0
 PIER10                  EQU     s 0x0000044E
 __ssr2                  Addr.   g 0x0000D7
 PDUT5                   EQU     s 0x000000A8
 PDUT6                   EQU     s 0x00000568
 __pcnh5                 Addr.   g 0x0000AB
 PDUT7                   EQU     s 0x00000570
 __tccs1                 Addr.   g 0x000026
 PDUT8                   EQU     s 0x0000057C
 __romm                  Addr.   g 0x0003AE
 PDUT9                   EQU     s 0x00000584
 IF1MCTRL0               EQU     s 0x0000071C
 WDTC                    EQU     s 0x0000040E
 CILCR                   EQU     s 0x0000042D
 IF1MCTRL1               EQU     s 0x0000081C
 OCS7                    EQU     s 0x0000003B
 IOA1                    EQU     s 0x0000010C
 __if1mctrh1             Addr.   g 0x00081D
 __if2mctrh1             Addr.   g 0x00084D
 PCNL11                  EQU     s 0x00000596
 __rctcr                 Addr.   g 0x000408
 PRRR11                  EQU     s 0x00000661
 __bapm0                 Addr.   g 0x000101
 __pdut13                Addr.   g 0x0005A8
 BRPERL0                 EQU     s 0x0000070C
 __scr8                  Addr.   g 0x000549
 __pcnl0                 Addr.   g 0x00007E
 EDSU2L                  EQU     s 0x000003AC
 __ibsr1                 Addr.   g 0x0000B6
 __epsr01                Addr.   g 0x0004BD
 __pcsr13                Addr.   g 0x0005A6
 __ilr                   Addr.   g 0x0003A0
 __newdt2h1              Addr.   g 0x000893
 PCSR11                  EQU     s 0x00000592
 __pcnh3                 Addr.   g 0x000097
 TMCSRH2                 EQU     s 0x00000069
 __msgval1l1             Addr.   g 0x0008B0
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    22

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __idar0                 Addr.   g 0x0000B4
 __dmacs1                Addr.   g 0x00010B
 __newdt2l1              Addr.   g 0x000892
 __msgval2l1             Addr.   g 0x0008B2
 CKMR                    EQU     s 0x00000403
 PTMR13                  EQU     s 0x000005A4
 WTBR0                   EQU     s 0x000004E0
 WTBR1                   EQU     s 0x000004E2
 __if1dta0               Addr.   g 0x00071E
 __if2dta0               Addr.   g 0x00074E
 __if1dta1               Addr.   g 0x00081E
 DCTL0                   EQU     s 0x00000106
 __if2dta1               Addr.   g 0x00084E
 DCTL1                   EQU     s 0x0000010E
 __dssr                  Addr.   g 0x000392
 DCTL2                   EQU     s 0x00000116
 IF2ARB21                EQU     s 0x0000084A
 TMRLR2                  EQU     s 0x0000006A
 DCTL3                   EQU     s 0x0000011E
 __pcnl10                Addr.   g 0x00058E
 __ioa2                  Addr.   g 0x000114
 DCTL4                   EQU     s 0x00000126
 DCTL5                   EQU     s 0x0000012E
 IF1MSK2H0               EQU     s 0x00000717
 IF1DTA2H0               EQU     s 0x00000721
 IF2MSK1H0               EQU     s 0x00000745
 IF2DTA1H0               EQU     s 0x0000074F
 CTRLRL1                 EQU     s 0x00000800
 PCN5                    EQU     s 0x000000AA
 __brperh0               Addr.   g 0x00070D
 BRPERH1                 EQU     s 0x0000080D
 IF1MSK2H1               EQU     s 0x00000817
 IF1DTA2H1               EQU     s 0x00000821
 IF2MSK1H1               EQU     s 0x00000845
 IF2DTA1H1               EQU     s 0x0000084F
 __wtcksr                Addr.   g 0x0004E7
 __brperh1               Addr.   g 0x00080D
 __prrr8                 Addr.   g 0x0004DE
 __cucr                  Addr.   g 0x0004EA
 __cutr1                 Addr.   g 0x0004F0
 __pcnh1                 Addr.   g 0x000087
 TMCSRH0                 EQU     s 0x00000061
 __gcn12                 Addr.   g 0x000574
 __newdt20               Addr.   g 0x000792
 __newdt21               Addr.   g 0x000892
 SCR9                    EQU     s 0x00000553
 __ptmr9                 Addr.   g 0x000580
 __disel0                Addr.   g 0x000380
 IF2DTB0                 EQU     s 0x00000752
 IF1DTB1                 EQU     s 0x00000822
 GCN10                   EQU     s 0x00000074
 IOA5                    EQU     s 0x0000012C
 GCN11                   EQU     s 0x00000098
 __wtbr0                 Addr.   g 0x0004E0
 __eacl4                 Addr.   g 0x0006E8
 GCN12                   EQU     s 0x00000574
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    23

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 GCN13                   EQU     s 0x00000598
 IF1ARB11                EQU     s 0x00000818
 RDR0                    EQU     s 0x000000C2
 ESIR0                   EQU     s 0x000000C8
 PCNH14                  EQU     s 0x000005B3
 ESIR1                   EQU     s 0x000000D2
 SSR2                    EQU     s 0x000000D7
 __epsr05                Addr.   g 0x0004C1
 TDR0                    EQU     s 0x000000C2
 ESIR2                   EQU     s 0x000000DC
 __eac1                  Addr.   g 0x0006E2
 PODR01                  EQU     s 0x00000481
 PHDR08                  EQU     s 0x0000049C
 IF2CMSKH0               EQU     s 0x00000743
 ESIR3                   EQU     s 0x000000E6
 __prrr6                 Addr.   g 0x0004DC
 PCN10                   EQU     s 0x0000058E
 IF2CMSKH1               EQU     s 0x00000843
 __gcn10                 Addr.   g 0x000074
 PCN11                   EQU     s 0x00000596
 __smr1                  Addr.   g 0x0000CA
 __cutr                  Addr.   g 0x0004EE
 PCN12                   EQU     s 0x000005A2
 PCNH0                   EQU     s 0x0000007F
 __dmacs5                Addr.   g 0x00012B
 __ptmr7                 Addr.   g 0x00056C
 PCN13                   EQU     s 0x000005AA
 PCNH1                   EQU     s 0x00000087
 ESIR7                   EQU     s 0x00000546
 PCN14                   EQU     s 0x000005B2
 IF1CREQ0                EQU     s 0x00000710
 IF1DTB20                EQU     s 0x00000724
 IF2DTB10                EQU     s 0x00000752
 PCNH2                   EQU     s 0x0000008F
 ESIR8                   EQU     s 0x00000550
 PCN15                   EQU     s 0x000005BA
 PCNH3                   EQU     s 0x00000097
 __rcr                   Addr.   g 0x00040C
 __ader4                 Addr.   g 0x0004D4
 __msgval1h0             Addr.   g 0x0007B1
 ESIR9                   EQU     s 0x0000055A
 __elvrl0                Addr.   g 0x00005A
 PCNH4                   EQU     s 0x000000A3
 __pier03                Addr.   g 0x000447
 __eacl2                 Addr.   g 0x0006E4
 __msgval2h0             Addr.   g 0x0007B3
 IPCPH0                  EQU     s 0x00000043
 PCNH5                   EQU     s 0x000000AB
 TREQR0                  EQU     s 0x00000780
 DMACS4                  EQU     s 0x00000123
 RCCSRC                  EQU     s 0x0000040B
 PCNH6                   EQU     s 0x0000056B
 NEWDT11                 EQU     s 0x00000890
 __pcn4                  Addr.   g 0x0000A2
 PCNH7                   EQU     s 0x00000573
 IOAH0                   EQU     s 0x00000105
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    24

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __pcnl14                Addr.   g 0x0005B2
 PCNH8                   EQU     s 0x0000057F
 NEWDT1L1                EQU     s 0x00000890
 IPCPL0                  EQU     s 0x00000042
 IOAH1                   EQU     s 0x0000010D
 __pfam6                 Addr.   g 0x0003CB
 __each5                 Addr.   g 0x0006EB
 PCNH9                   EQU     s 0x00000587
 IOAH2                   EQU     s 0x00000115
 IF1MSK2L0               EQU     s 0x00000716
 IF1DTA2L0               EQU     s 0x00000720
 IF2MSK1L0               EQU     s 0x00000744
 IF2DTA1L0               EQU     s 0x0000074E
 IOAH3                   EQU     s 0x0000011D
 __pilr03                Addr.   g 0x00045B
 __brperl0               Addr.   g 0x00070C
 PCN9                    EQU     s 0x00000586
 IF1MSK2L1               EQU     s 0x00000816
 IF1DTA2L1               EQU     s 0x00000820
 IF2MSK1L1               EQU     s 0x00000844
 IF2DTA1L1               EQU     s 0x0000084E
 IOAH4                   EQU     s 0x00000125
 __prrr4                 Addr.   g 0x0004DA
 __brperl1               Addr.   g 0x00080C
 IOAH5                   EQU     s 0x0000012D
 CTRLRH0                 EQU     s 0x00000701
 __ptmr5                 Addr.   g 0x0000A4
 __scr0                  Addr.   g 0x0000C1
 __treqr2h1              Addr.   g 0x000883
 PIER03                  EQU     s 0x00000447
 __bapl5                 Addr.   g 0x000128
 __disel4                Addr.   g 0x000384
 __ader2                 Addr.   g 0x0004D2
 CKFCRH                  EQU     s 0x00000405
 __eacl0                 Addr.   g 0x0006E0
 __pcnh12                Addr.   g 0x0005A3
 __treqr2l1              Addr.   g 0x000882
 __btrl1                 Addr.   g 0x000806
 __ipcpl3                Addr.   g 0x00004A
 CUTR                    EQU     s 0x000004EE
 NEWDT1                  EQU     s 0x00000890
 __pfam4                 Addr.   g 0x0003C5
 __gcn2l3                Addr.   g 0x00059A
 __each3                 Addr.   g 0x0006E7
 __bgr9                  Addr.   g 0x000558
 ACSR0                   EQU     s 0x00000560
 PCNL14                  EQU     s 0x000005B2
 __epsr09                Addr.   g 0x0004C5
 ACSR1                   EQU     s 0x00000562
 __prrr2                 Addr.   g 0x0004D8
 IF2CMSKL0               EQU     s 0x00000742
 IF2ARB2H0               EQU     s 0x0000074B
 PFAM0                   EQU     s 0x000003B9
 DDR00                   EQU     s 0x00000430
 EBCS                    EQU     s 0x000006F5
 IF2CMSKL1               EQU     s 0x00000842
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    25

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 IF2ARB2H1               EQU     s 0x0000084B
 __dctl4                 Addr.   g 0x000126
 PFAM1                   EQU     s 0x000003BC
 DDR01                   EQU     s 0x00000431
 __ptmr3                 Addr.   g 0x000090
 __if1arb11              Addr.   g 0x000818
 PFAM2                   EQU     s 0x000003BF
 DDR02                   EQU     s 0x00000432
 PCNL0                   EQU     s 0x0000007E
 __if1arb21              Addr.   g 0x00081A
 __if2arb11              Addr.   g 0x000848
 PFAM3                   EQU     s 0x000003C2
 DDR03                   EQU     s 0x00000433
 PCNL1                   EQU     s 0x00000086
 __bapl3                 Addr.   g 0x000118
 __if2arb21              Addr.   g 0x00084A
 PFAM4                   EQU     s 0x000003C5
 DDR04                   EQU     s 0x00000434
 PRRR0                   EQU     s 0x000004D6
 PCNL2                   EQU     s 0x0000008E
 __ader0                 Addr.   g 0x0004D0
 PFAM5                   EQU     s 0x000003C8
 DDR05                   EQU     s 0x00000435
 PRRR1                   EQU     s 0x000004D7
 IF2MCTR0                EQU     s 0x0000074C
 __elvrh0                Addr.   g 0x00005B
 PCNL3                   EQU     s 0x00000096
 __pfd2                  Addr.   g 0x0003D4
 __podr01                Addr.   g 0x000481
 __phdr08                Addr.   g 0x00049C
 __treqr20               Addr.   g 0x000782
 PFAM6                   EQU     s 0x000003CB
 DDR06                   EQU     s 0x00000436
 EPSR02                  EQU     s 0x000004BE
 PRRR2                   EQU     s 0x000004D8
 PCSR14                  EQU     s 0x000005AE
 PCNL4                   EQU     s 0x000000A2
 __pier07                Addr.   g 0x00044B
 __treqr21               Addr.   g 0x000882
 PFAM7                   EQU     s 0x000003CE
 DDR07                   EQU     s 0x00000437
 PRRR3                   EQU     s 0x000004D9
 PCNL5                   EQU     s 0x000000AA
 __dct1                  Addr.   g 0x00010E
 PLLCR                   EQU     s 0x00000406
 WDTCP                   EQU     s 0x0000040F
 DDR08                   EQU     s 0x00000438
 PRRR4                   EQU     s 0x000004DA
 DDR09                   EQU     s 0x00000439
 PRRR5                   EQU     s 0x000004DB
 PCNL6                   EQU     s 0x0000056A
 __pdut8                 Addr.   g 0x00057C
 PRRR6                   EQU     s 0x000004DC
 PCNL7                   EQU     s 0x00000572
 NEWDT1H1                EQU     s 0x00000891
 ITMKH0                  EQU     s 0x000000B1
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    26

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 ESCR0                   EQU     s 0x000000C5
 IOAL0                   EQU     s 0x00000104
 __pfam2                 Addr.   g 0x0003BF
 __each1                 Addr.   g 0x0006E3
 PRRR7                   EQU     s 0x000004DD
 PCNL8                   EQU     s 0x0000057E
 PDR00                   EQU     s 0x00000000
 PCSR0                   EQU     s 0x0000007A
 ESCR1                   EQU     s 0x000000CF
 IOAL1                   EQU     s 0x0000010C
 PRRR8                   EQU     s 0x000004DE
 PCNL9                   EQU     s 0x00000586
 PDR01                   EQU     s 0x00000001
 TCCSH0                  EQU     s 0x00000023
 PCSR1                   EQU     s 0x00000082
 ESCR2                   EQU     s 0x000000D9
 IOAL2                   EQU     s 0x00000114
 PRRR9                   EQU     s 0x000004DF
 PDR02                   EQU     s 0x00000002
 PCSR2                   EQU     s 0x0000008A
 ESCR3                   EQU     s 0x000000E3
 IOAL3                   EQU     s 0x0000011C
 __pilr07                Addr.   g 0x00045F
 __prrr0                 Addr.   g 0x0004D6
 __tdr9                  Addr.   g 0x000554
 __pcsr8                 Addr.   g 0x00057A
 EAC2                    EQU     s 0x000006E4
 IF1ARB1H0               EQU     s 0x00000719
 PDR03                   EQU     s 0x00000003
 PCSR3                   EQU     s 0x00000092
 ITMKL0                  EQU     s 0x000000B0
 IOAL4                   EQU     s 0x00000124
 __bgrl8                 Addr.   g 0x00054E
 PUCR02                  EQU     s 0x000004AA
 IF1ARB1H1               EQU     s 0x00000819
 PDR04                   EQU     s 0x00000004
 PCSR4                   EQU     s 0x0000009E
 ITBAH0                  EQU     s 0x000000AF
 SMR3                    EQU     s 0x000000DE
 __dctl2                 Addr.   g 0x000116
 IOAL5                   EQU     s 0x0000012C
 __ptmr1                 Addr.   g 0x000080
 PDR05                   EQU     s 0x00000005
 TCCSL0                  EQU     s 0x00000022
 TMR3                    EQU     s 0x0000006E
 PCSR5                   EQU     s 0x000000A6
 BGR0                    EQU     s 0x000000C6
 __ioa5                  Addr.   g 0x00012C
 PDR06                   EQU     s 0x00000006
 ESCR7                   EQU     s 0x00000543
 PCSR6                   EQU     s 0x00000566
 __ocs0                  Addr.   g 0x000028
 PDR07                   EQU     s 0x00000007
 __bapl1                 Addr.   g 0x000108
 __msgval10              Addr.   g 0x0007B0
 ESCR8                   EQU     s 0x0000054D
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    27

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 PCSR7                   EQU     s 0x0000056E
 PDR08                   EQU     s 0x00000008
 ITBAL0                  EQU     s 0x000000AE
 __mtcra                 Addr.   g 0x0003F2
 __msgval20              Addr.   g 0x0007B2
 ESCR9                   EQU     s 0x00000557
 PCSR8                   EQU     s 0x0000057A
 PDR09                   EQU     s 0x00000009
 __dcth5                 Addr.   g 0x00012F
 PCSR9                   EQU     s 0x00000582
 BTR1                    EQU     s 0x00000806
 __baph4                 Addr.   g 0x000122
 PHDR10                  EQU     s 0x0000049E
 __ipcph3                Addr.   g 0x00004B
 __itbal1                Addr.   g 0x0000B8
 __pdut6                 Addr.   g 0x000568
 __if1arb1l0             Addr.   g 0x000718
 __ipcpl7                Addr.   g 0x000056
 __pfam0                 Addr.   g 0x0003B9
 __gcn2h3                Addr.   g 0x00059B
 __if1arb2l0             Addr.   g 0x00071A
 __if2arb1l0             Addr.   g 0x000748
 MCSRA                   EQU     s 0x000003F1
 __if2arb2l0             Addr.   g 0x00074A
 MCSRB                   EQU     s 0x000003F5
 __prrr10                Addr.   g 0x000660
 __btrh0                 Addr.   g 0x000707
 RDR8                    EQU     s 0x0000054A
 __rdr9                  Addr.   g 0x000554
 __pcsr6                 Addr.   g 0x000566
 __itmk1                 Addr.   g 0x0000BA
 __ioal5                 Addr.   g 0x00012C
 __intr1                 Addr.   g 0x000808
 ICR                     EQU     s 0x000003A0
 PILR02                  EQU     s 0x0000045A
 TDR8                    EQU     s 0x0000054A
 IF2ARB2L0               EQU     s 0x0000074A
 __dctl0                 Addr.   g 0x000106
 IF2ARB2L1               EQU     s 0x0000084A
 __ice01                 Addr.   g 0x000041
 TMCSR1                  EQU     s 0x00000064
 __bgrh9                 Addr.   g 0x000559
 __eac4                  Addr.   g 0x0006E8
 PODR04                  EQU     s 0x00000484
 TREQR1L0                EQU     s 0x00000780
 __dcth3                 Addr.   g 0x00011F
 __podr05                Addr.   g 0x000485
 TESTRL1                 EQU     s 0x0000080A
 __baph2                 Addr.   g 0x000112
 RCR                     EQU     s 0x0000040C
 __pdut4                 Addr.   g 0x0000A0
 __edsu2                 Addr.   g 0x0003AC
 WTCER                   EQU     s 0x000004E6
 __pcsr4                 Addr.   g 0x00009E
 IPCPH3                  EQU     s 0x0000004B
 CUTR1                   EQU     s 0x000004F0
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    28

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __ioal3                 Addr.   g 0x00011C
 __if1arb1h1             Addr.   g 0x000819
 CUTR2                   EQU     s 0x000004EE
 IF1ARB1L0               EQU     s 0x00000718
 MSGVAL21                EQU     s 0x000008B2
 __epilr10               Addr.   g 0x000476
 __pcn7                  Addr.   g 0x000572
 __if1arb2h1             Addr.   g 0x00081B
 __if2arb1h1             Addr.   g 0x000849
 IF1ARB1L1               EQU     s 0x00000818
 IF2MSK1                 EQU     s 0x00000844
 IF2DTA1                 EQU     s 0x0000084E
 __ics01                 Addr.   g 0x000040
 DCT2                    EQU     s 0x00000116
 __if2arb2h1             Addr.   g 0x00084B
 SMR7                    EQU     s 0x0000053E
 IPCPL3                  EQU     s 0x0000004A
 __enir1                 Addr.   g 0x00005C
 OCS0                    EQU     s 0x00000028
 __bgrh7                 Addr.   g 0x000545
 __dcth1                 Addr.   g 0x00010F
 __eas4                  Addr.   g 0x0006EE
 __bgr1                  Addr.   g 0x0000D0
 __ctrlr1                Addr.   g 0x000800
 NEWDT2L0                EQU     s 0x00000792
 __elvr0                 Addr.   g 0x00005A
 __baph0                 Addr.   g 0x000102
 __derl                  Addr.   g 0x000394
 __ptmr12                Addr.   g 0x00059C
 __if1dtb11              Addr.   g 0x000822
 INTRH0                  EQU     s 0x00000709
 __pdut2                 Addr.   g 0x00008C
 __itbah1                Addr.   g 0x0000B9
 __scr3                  Addr.   g 0x0000DF
 __if1dtb21              Addr.   g 0x000824
 __if2dtb11              Addr.   g 0x000852
 PIER06                  EQU     s 0x0000044A
 __ipcph7                Addr.   g 0x000057
 __if2dtb21              Addr.   g 0x000854
 __ssr8                  Addr.   g 0x00054B
 EDSU                    EQU     s 0x000003AF
 __tmr0                  Addr.   g 0x000062
 INTRL0                  EQU     s 0x00000708
 __pcsr2                 Addr.   g 0x00008A
 __cutr1h                Addr.   g 0x0004F1
 __bgrl2                 Addr.   g 0x0000DA
 __ioal1                 Addr.   g 0x00010C
 IF2MSK20                EQU     s 0x00000746
 IF2DTA20                EQU     s 0x00000750
 ISMK0                   EQU     s 0x000000B3
 IF1MSK0                 EQU     s 0x00000714
 IF1DTA0                 EQU     s 0x0000071E
 __tmrlr3                Addr.   g 0x00006E
 ISMK1                   EQU     s 0x000000BD
 PFD1                    EQU     s 0x000003D2
 TMISR                   EQU     s 0x000004FA
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    29

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __ioah4                 Addr.   g 0x000125
 ROMM                    EQU     s 0x000003AE
 TREQR1H0                EQU     s 0x00000781
 ISBA0                   EQU     s 0x000000B2
 CTRLR1                  EQU     s 0x00000800
 ISBA1                   EQU     s 0x000000BC
 __pucr03                Addr.   g 0x0004AB
 __pfal7                 Addr.   g 0x0003CD
 __podr09                Addr.   g 0x000489
 DISEL2                  EQU     s 0x00000382
 __tdr1                  Addr.   g 0x0000CC
 __pdut0                 Addr.   g 0x00007C
 __tmcsr1                Addr.   g 0x000064
 __pfdl7                 Addr.   g 0x0003DE
 AECSR1                  EQU     s 0x00000563
 __mcsrb                 Addr.   g 0x0003F5
 __btr0                  Addr.   g 0x000706
 __pfd5                  Addr.   g 0x0003DA
 __eccr9                 Addr.   g 0x000556
 EPSR05                  EQU     s 0x000004C1
 TESTRH0                 EQU     s 0x0000070B
 __pcsr0                 Addr.   g 0x00007A
 BTRH0                   EQU     s 0x00000707
 __gcn1l1                Addr.   g 0x000098
 __bgrl0                 Addr.   g 0x0000C6
 __dct4                  Addr.   g 0x000126
 __if1arb0               Addr.   g 0x000718
 IF1MSK10                EQU     s 0x00000714
 IF1DTA10                EQU     s 0x0000071E
 BTRH1                   EQU     s 0x00000807
 __if2arb0               Addr.   g 0x000748
 __if1arb1               Addr.   g 0x000818
 SCR2                    EQU     s 0x000000D5
 __if1dtb1l0             Addr.   g 0x000722
 __if2arb1               Addr.   g 0x000848
 EAS3                    EQU     s 0x000006ED
 __if1dtb2l0             Addr.   g 0x000724
 __if2dtb1l0             Addr.   g 0x000752
 __if2dtb2l0             Addr.   g 0x000754
 BGR8                    EQU     s 0x0000054E
 __isba1                 Addr.   g 0x0000BC
 __bgrh3                 Addr.   g 0x0000E5
 __ioah2                 Addr.   g 0x000115
 CMPR                    EQU     s 0x0000041A
 OCS4                    EQU     s 0x00000034
 __cilcr                 Addr.   g 0x00042D
 PUCR05                  EQU     s 0x000004AD
 __pfal5                 Addr.   g 0x0003C7
 NEWDT2H0                EQU     s 0x00000793
 __rdr1                  Addr.   g 0x0000CC
 __pdut10                Addr.   g 0x00058C
 __ocs3                  Addr.   g 0x00002F
 __tbrl                  Addr.   g 0x0003A2
 __pfdl5                 Addr.   g 0x0003DA
 INTPND21                EQU     s 0x000008A2
 ENIR0                   EQU     s 0x00000058
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    30

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __eccr7                 Addr.   g 0x000542
 __pcsr10                Addr.   g 0x00058A
 ENIR1                   EQU     s 0x0000005C
 __cutr1l                Addr.   g 0x0004F0
 __if1creql1             Addr.   g 0x000810
 __if2creql1             Addr.   g 0x000840
 IF1CMSK0                EQU     s 0x00000712
 ICS01                   EQU     s 0x00000040
 GCN2H1                  EQU     s 0x0000009B
 PFD5                    EQU     s 0x000003DA
 __bgrh1                 Addr.   g 0x0000D1
 __ioah0                 Addr.   g 0x000105
 __epsr10                Addr.   g 0x0004C6
 MSGVAL2H0               EQU     s 0x000007B3
 GCN2L1                  EQU     s 0x0000009A
 __if1dtb1h1             Addr.   g 0x000823
 PILR05                  EQU     s 0x0000045D
 MSGVAL2H1               EQU     s 0x000008B3
 __pfal3                 Addr.   g 0x0003C1
 __pucr07                Addr.   g 0x0004AF
 __testr0                Addr.   g 0x00070A
 __if1dtb2h1             Addr.   g 0x000825
 __if2dtb1h1             Addr.   g 0x000853
 __if2dtb2h1             Addr.   g 0x000855
 __pfdl3                 Addr.   g 0x0003D6
 PDUT12                  EQU     s 0x000005A0
 PODR07                  EQU     s 0x00000487
 __iccr1                 Addr.   g 0x0000BF
 __pfah6                 Addr.   g 0x0003CC
 PCN2                    EQU     s 0x0000008E
 __gcn1h1                Addr.   g 0x000099
 __smr7                  Addr.   g 0x00053E
 BTRL0                   EQU     s 0x00000706
 __pfdh6                 Addr.   g 0x0003DD
 BTRL1                   EQU     s 0x00000806
 EDSU2                   EQU     s 0x000003AC
 IF2ARB0                 EQU     s 0x00000748
 IF1ARB1                 EQU     s 0x00000818
 IPCPH6                  EQU     s 0x00000055
 __ssr0                  Addr.   g 0x0000C3
 IOA2                    EQU     s 0x00000114
 __pfal1                 Addr.   g 0x0003BB
 PFDH0                   EQU     s 0x000003D1
 IPCPL6                  EQU     s 0x00000054
 __pdut14                Addr.   g 0x0005B0
 PFDH1                   EQU     s 0x000003D3
 __pfdl1                 Addr.   g 0x0003D2
 PFDH2                   EQU     s 0x000003D5
 PFDH3                   EQU     s 0x000003D7
 WTSR                    EQU     s 0x000004E3
 __eccr3                 Addr.   g 0x0000E2
 __pfah4                 Addr.   g 0x0003C6
 PFDH4                   EQU     s 0x000003D9
 __epsr02                Addr.   g 0x0004BE
 __pcsr14                Addr.   g 0x0005AE
 PFDH5                   EQU     s 0x000003DB
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    31

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 PFDH6                   EQU     s 0x000003DD
 MTCRAL                  EQU     s 0x000003F2
 ECCR0                   EQU     s 0x000000C4
 __pfdh4                 Addr.   g 0x0003D9
 PFDH7                   EQU     s 0x000003DF
 MTCRBL                  EQU     s 0x000003F6
 __ice67                 Addr.   g 0x000053
 ECCR1                   EQU     s 0x000000CE
 PIER09                  EQU     s 0x0000044D
 ECCR2                   EQU     s 0x000000D8
 IF1ARB20                EQU     s 0x0000071A
 IF2ARB10                EQU     s 0x00000748
 __pdr08                 Addr.   g 0x000008
 ECCR3                   EQU     s 0x000000E2
 __dmacs2                Addr.   g 0x000113
 __tmr3                  Addr.   g 0x00006E
 ICCR0                   EQU     s 0x000000B5
 __mtcrah                Addr.   g 0x0003F3
 __ebae1                 Addr.   g 0x0006F3
 ICCR1                   EQU     s 0x000000BF
 __if1creqh0             Addr.   g 0x000711
 __if1dtb0               Addr.   g 0x000722
 __if2creqh0             Addr.   g 0x000741
 MSGVAL2L0               EQU     s 0x000007B2
 ICS45                   EQU     s 0x0000004C
 __pier00                Addr.   g 0x000444
 __if2dtb0               Addr.   g 0x000752
 __if1dtb1               Addr.   g 0x000822
 ECCR7                   EQU     s 0x00000542
 MSGVAL2L1               EQU     s 0x000008B2
 __if2dtb1               Addr.   g 0x000852
 ECCR8                   EQU     s 0x0000054C
 TREQR20                 EQU     s 0x00000782
 ECCR9                   EQU     s 0x00000556
 PTMR11                  EQU     s 0x00000590
 BAPH0                   EQU     s 0x00000102
 __pcnl11                Addr.   g 0x000596
 __eccr1                 Addr.   g 0x0000CE
 BAPH1                   EQU     s 0x0000010A
 __pfah2                 Addr.   g 0x0003C0
 BAPH2                   EQU     s 0x00000112
 BRPER0                  EQU     s 0x0000070C
 BAPH3                   EQU     s 0x0000011A
 __pilr00                Addr.   g 0x000458
 DISEL5                  EQU     s 0x00000385
 PCN6                    EQU     s 0x0000056A
 __ics67                 Addr.   g 0x000052
 BAPH4                   EQU     s 0x00000122
 __pfdh2                 Addr.   g 0x0003D5
 BAPH5                   EQU     s 0x0000012A
 TMRLR0                  EQU     s 0x00000062
 INTPND2H0               EQU     s 0x000007A3
 __pdr06                 Addr.   g 0x000006
 __ioa0                  Addr.   g 0x000104
 INTPND2H1               EQU     s 0x000008A3
 EPSR08                  EQU     s 0x000004C4
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    32

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __disel1                Addr.   g 0x000381
 GCN20                   EQU     s 0x00000076
 TCDT0                   EQU     s 0x00000020
 GCN21                   EQU     s 0x0000009A
 TCDT1                   EQU     s 0x00000024
 PFDL0                   EQU     s 0x000003D0
 GCN22                   EQU     s 0x00000576
 __edsu2h                Addr.   g 0x0003AD
 PFDL1                   EQU     s 0x000003D2
 GCN23                   EQU     s 0x0000059A
 __ipcpl0                Addr.   g 0x000042
 __pfcs2                 Addr.   g 0x0003B4
 PFDL2                   EQU     s 0x000003D4
 __gcn2l0                Addr.   g 0x000076
 __pfah0                 Addr.   g 0x0003BA
 PFDL3                   EQU     s 0x000003D6
 RDR1                    EQU     s 0x000000CC
 PFDL4                   EQU     s 0x000003D8
 PUCR08                  EQU     s 0x000004B0
 SSR3                    EQU     s 0x000000E1
 __epsr06                Addr.   g 0x0004C2
 PFDL5                   EQU     s 0x000003DA
 TDR1                    EQU     s 0x000000CC
 __pfdh0                 Addr.   g 0x0003D1
 PFDL6                   EQU     s 0x000003DC
 PFDL7                   EQU     s 0x000003DE
 CMPRH                   EQU     s 0x0000041B
 __ocs6                  Addr.   g 0x00003A
 TESTR1                  EQU     s 0x0000080A
 __pdr04                 Addr.   g 0x000004
 PCNH12                  EQU     s 0x000005A3
 __mtcral                Addr.   g 0x0003F2
 CMPRL                   EQU     s 0x0000041A
 IF1CREQ1                EQU     s 0x00000810
 IF1DTB21                EQU     s 0x00000824
 IF2DTB11                EQU     s 0x00000852
 __if1msk11              Addr.   g 0x000814
 __if1msk21              Addr.   g 0x000816
 __if2msk11              Addr.   g 0x000844
 EBM                     EQU     s 0x000006F0
 __elvrl1                Addr.   g 0x00005E
 __pier04                Addr.   g 0x000448
 __if2msk21              Addr.   g 0x000846
 __statrh0               Addr.   g 0x000703
 __statrh1               Addr.   g 0x000803
 __intpnd11              Addr.   g 0x0008A0
 CKSSR                   EQU     s 0x00000402
 __pfcs0                 Addr.   g 0x0003B0
 __intpnd21              Addr.   g 0x0008A2
 WTMR                    EQU     s 0x000004E4
 BAPL0                   EQU     s 0x00000100
 __pcnl15                Addr.   g 0x0005BA
 BAPL1                   EQU     s 0x00000108
 PILR08                  EQU     s 0x00000460
 STATRL0                 EQU     s 0x00000702
 BAPL2                   EQU     s 0x00000110
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    33

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 BAPL3                   EQU     s 0x00000118
 __pilr04                Addr.   g 0x00045C
 __podr10                Addr.   g 0x00048A
 GCN1H2                  EQU     s 0x00000575
 DMACS2                  EQU     s 0x00000113
 BAPL4                   EQU     s 0x00000120
 __pcn2                  Addr.   g 0x00008E
 SMR0                    EQU     s 0x000000C0
 BAPL5                   EQU     s 0x00000128
 PDUT15                  EQU     s 0x000005B8
 __pdr02                 Addr.   g 0x000002
 TMR0                    EQU     s 0x00000062
 INTPND2L0               EQU     s 0x000007A2
 __if1creq0              Addr.   g 0x000710
 GCN1L2                  EQU     s 0x00000574
 INTPND2L1               EQU     s 0x000008A2
 __brper0                Addr.   g 0x00070C
 __if2creq0              Addr.   g 0x000740
 __disel5                Addr.   g 0x000385
 __pcnh13                Addr.   g 0x0005AB
 __ipcph0                Addr.   g 0x000043
 __edsu2l                Addr.   g 0x0003AC
 PIER01                  EQU     s 0x00000445
 __ipcpl4                Addr.   g 0x00004E
 __gcn2h0                Addr.   g 0x000077
 __ssr3                  Addr.   g 0x0000E1
 __acsr1                 Addr.   g 0x000562
 __ipcp6                 Addr.   g 0x000054
 __if1msk1l0             Addr.   g 0x000714
 __if1msk2l0             Addr.   g 0x000716
 __if2msk1l0             Addr.   g 0x000744
 SSR7                    EQU     s 0x00000541
 __if2msk2l0             Addr.   g 0x000746
 __tcdt1                 Addr.   g 0x000024
 __pllcr                 Addr.   g 0x000406
 DDR10                   EQU     s 0x0000043A
 __pdr00                 Addr.   g 0x000000
 __intpnd1l0             Addr.   g 0x0007A0
 STATRH1                 EQU     s 0x00000803
 __intpnd2l0             Addr.   g 0x0007A2
 __bgr7                  Addr.   g 0x000544
 PCNL12                  EQU     s 0x000005A2
 PRRR12                  EQU     s 0x00000662
 EPILR08                 EQU     s 0x00000474
 IF2MCTR1                EQU     s 0x0000084C
 __elvrh1                Addr.   g 0x00005F
 __podr02                Addr.   g 0x000482
 __phdr09                Addr.   g 0x00049D
 __scr9                  Addr.   g 0x000553
 __dsrh                  Addr.   g 0x000391
 __pier08                Addr.   g 0x00044C
 __statrl0               Addr.   g 0x000702
 __tmr6                  Addr.   g 0x000072
 __statrl1               Addr.   g 0x000802
 __ipcp4                 Addr.   g 0x00004E
 PDR10                   EQU     s 0x0000000A
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    34

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __pfd0                  Addr.   g 0x0003D0
 EPSR00                  EQU     s 0x000004BC
 PCSR12                  EQU     s 0x0000059E
 EPSR10                  EQU     s 0x000004C6
 ERRCNT0                 EQU     s 0x00000704
 __pilr08                Addr.   g 0x000460
 __escr9                 Addr.   g 0x000557
 PTMR14                  EQU     s 0x000005AC
 EAC3                    EQU     s 0x000006E6
 ICE01                   EQU     s 0x00000041
 __newdt1h0              Addr.   g 0x000791
 BGR1                    EQU     s 0x000000D0
 __if1msk1h1             Addr.   g 0x000815
 __if1msk2h1             Addr.   g 0x000817
 __if2msk1h1             Addr.   g 0x000845
 __occp7                 Addr.   g 0x00003E
 __eirr0                 Addr.   g 0x000059
 __if2msk2h1             Addr.   g 0x000847
 PTMR0                   EQU     s 0x00000078
 __tdr7                  Addr.   g 0x000540
 __esir9                 Addr.   g 0x00055A
 __newdt1l0              Addr.   g 0x000790
 EPILR06                 EQU     s 0x00000472
 PTMR1                   EQU     s 0x00000080
 __dsr                   Addr.   g 0x000390
 __intpnd1h1             Addr.   g 0x0008A1
 PUCR00                  EQU     s 0x000004A8
 __ismk0                 Addr.   g 0x0000B3
 ADSR                    EQU     s 0x0000001C
 TMRLR3                  EQU     s 0x0000006E
 PTMR2                   EQU     s 0x00000088
 __intpnd2h1             Addr.   g 0x0008A3
 PUCR10                  EQU     s 0x000004B2
 __tccsl0                Addr.   g 0x000022
 PTMR3                   EQU     s 0x00000090
 __ioa3                  Addr.   g 0x00011C
 MSGVAL0                 EQU     s 0x000007B0
 PTMR4                   EQU     s 0x0000009C
 IF1CREQH0               EQU     s 0x00000711
 IF1DTB2H0               EQU     s 0x00000725
 IF2DTB1H0               EQU     s 0x00000753
 __ipcph4                Addr.   g 0x00004F
 PTMR5                   EQU     s 0x000000A4
 IF1CREQH1               EQU     s 0x00000811
 IF1DTB2H1               EQU     s 0x00000825
 IF2DTB1H1               EQU     s 0x00000853
 NMI                     EQU     s 0x000003A5
 PTMR6                   EQU     s 0x00000564
 __ipcp2                 Addr.   g 0x000048
 PTMR7                   EQU     s 0x0000056C
 __gcn22                 Addr.   g 0x000576
 __prrr11                Addr.   g 0x000661
 RDR9                    EQU     s 0x00000554
 PTMR8                   EQU     s 0x00000578
 __escr7                 Addr.   g 0x000543
 PTMR9                   EQU     s 0x00000580
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    35

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 __wtcrl                 Addr.   g 0x0004E8
 TDR9                    EQU     s 0x00000554
 __tmrlr0                Addr.   g 0x000062
 __ddr08                 Addr.   g 0x000438
 PLLCRH                  EQU     s 0x00000407
 TREQR11                 EQU     s 0x00000880
 __occp5                 Addr.   g 0x000038
 __rdr7                  Addr.   g 0x000540
 __esir7                 Addr.   g 0x000546
 __pcnl9                 Addr.   g 0x000586
 EPILR04                 EQU     s 0x00000470
 TREQR1L1                EQU     s 0x00000880
 __dirr                  Addr.   g 0x0003A4
 __pucr00                Addr.   g 0x0004A8
 PILR00                  EQU     s 0x00000458
 __podr06                Addr.   g 0x000486
 PILR10                  EQU     s 0x00000462
 MSGVAL10                EQU     s 0x000007B0
 PCNH15                  EQU     s 0x000005BB
 __tmisr                 Addr.   g 0x0004FA
 __pcn14                 Addr.   g 0x0005B2
 __ebcs                  Addr.   g 0x0006F5
 ELVRH0                  EQU     s 0x0000005B
 __eac2                  Addr.   g 0x0006E4
 PODR02                  EQU     s 0x00000482
 PHDR09                  EQU     s 0x0000049D
 __ipcp0                 Addr.   g 0x000042
 __gcn20                 Addr.   g 0x000076
 __smr2                  Addr.   g 0x0000D4
 ELVRL0                  EQU     s 0x0000005A
 __ddr06                 Addr.   g 0x000436
 DCT3                    EQU     s 0x0000011E
 __if1msk0               Addr.   g 0x000714
 SMR8                    EQU     s 0x00000548
 __if2msk0               Addr.   g 0x000744
 __if1cmskl1             Addr.   g 0x000812
 __if1msk1               Addr.   g 0x000814
 ILR                     EQU     s 0x000003A0
 __occp3                 Addr.   g 0x000032
 __if2cmskl1             Addr.   g 0x000842
 __if2msk1               Addr.   g 0x000844
 OCS1                    EQU     s 0x00000029
 IPCPH1                  EQU     s 0x00000045
 ICE45                   EQU     s 0x0000004D
 TMCSRL6                 EQU     s 0x00000070
 __pcnl7                 Addr.   g 0x000572
 EPILR02                 EQU     s 0x0000046E
 TREQR1                  EQU     s 0x00000880
 DMACS5                  EQU     s 0x0000012B
 __intpnd0               Addr.   g 0x0007A0
 NEWDT21                 EQU     s 0x00000892
 __pcn5                  Addr.   g 0x0000AA
 __ebm                   Addr.   g 0x0006F0
 __intpnd1               Addr.   g 0x0008A0
 __tccsh0                Addr.   g 0x000023
 NEWDT2L1                EQU     s 0x00000892
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    36

 === Module No. 7 (MB96348HS) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 IPCPL1                  EQU     s 0x00000044
 __ckmr                  Addr.   g 0x000403
 __ptmr13                Addr.   g 0x0005A4
 __if1dta1l1             Addr.   g 0x00081E
 WTCR                    EQU     s 0x000004E8
 CUTD                    EQU     s 0x000004EC
 __pcn12                 Addr.   g 0x0005A2
 __if1dta2l1             Addr.   g 0x000820
 __if2dta1l1             Addr.   g 0x00084E
 IF1CREQL0               EQU     s 0x00000710
 IF1DTB2L0               EQU     s 0x00000724
 IF2DTB1L0               EQU     s 0x00000752
 __if2dta2l1             Addr.   g 0x000850
 IF2MCTRH0               EQU     s 0x0000074D
 IF1CREQL1               EQU     s 0x00000810
 IF1DTB2L1               EQU     s 0x00000824
 IF2DTB1L1               EQU     s 0x00000852
 __eas2                  Addr.   g 0x0006EC
 IF2MCTRH1               EQU     s 0x0000084D
 __cocr1                 Addr.   g 0x000417
 IDX                     EQU     s 0x000003A1
 __escr3                 Addr.   g 0x0000E3
 __newdt0                Addr.   g 0x000790
 __cksr                  Addr.   g 0x000401
 __wtcrh                 Addr.   g 0x0004E9
 __coer1                 Addr.   g 0x0008CE
 IF2MSK21                EQU     s 0x00000846
 IF2DTA21                EQU     s 0x00000850

 === Module No. 8 (STARTUP) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 ICE89                   EQU     s 0x00000515
 ICE1011                 EQU     s 0x0000051B
 MTCRA                   EQU     s 0x000003F2
 MTCRB                   EQU     s 0x000003F6
 __userstack             Addr.   g 0x006694
 DIRDATA_S               Addr.   s 0x002240
 LABEL1                  Addr.   s 0xFE0610
 __systemstack           Addr.   g 0x005E94
 end                     Addr.   s 0xFE0675
 LABEL4                  Addr.   s 0xFE0648
 VRCR                    EQU     s 0x0000042C
 CKFCR                   EQU     s 0x00000404
 TCCSL2                  EQU     s 0x00000502
 no_PLL_yet              Addr.   s 0xFE0660
 CKSR                    EQU     s 0x00000401
 ICE67                   EQU     s 0x00000053
 LABEL2                  Addr.   s 0xFE0629
 __userstack_top         Addr.   g 0x006696
 CKMR                    EQU     s 0x00000403
 PLLCR                   EQU     s 0x00000406
 MCSRA                   EQU     s 0x000003F1
 MCSRB                   EQU     s 0x000003F5
 SSTACK_TOP              Addr.   s 0x006694
 USTACK_TOP              Addr.   s 0x006696
 _start                  Addr.   g 0xFE0594
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    37

 === Module No. 8 (STARTUP) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 ROMM                    EQU     s 0x000003AE
 TCCSL3                  EQU     s 0x00000506
 __systemstack_top       Addr.   g 0x006694
 ROMM_CONFIG             EQU     s 0x000000F7
 notresetyet             Addr.   s 0xFE0595
 CKSSR                   EQU     s 0x00000402
 ICS89                   EQU     s 0x00000514
 LABEL3                  Addr.   s 0xFE0638
 ICS1011                 EQU     s 0x0000051A
 ICE01                   EQU     s 0x00000041

 === Module No. 9 (vectors) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 InitIrqLevels           Func.   g 0xFE0677
 DefaultIRQHandler       Func.   g 0xFE069F

 === Module No. 11 (list) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 vListInitialise         Func.   g 0xFE06D6
 vListInitialiseItem     Func.   g 0xFE06F5
 vListInsertEnd          Func.   g 0xFE06FF
 vListInsert             Func.   g 0xFE072D
 uxListRemove            Func.   g 0xFE0775

 === Module No. 12 (queue) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 xQueueGenericReset      Func.   g 0xFE07A2
 xQueueGenericCreate     Func.   g 0xFE080F
 xQueueCreateMutex       Func.   g 0xFE085D
 xQueueGenericSend       Func.   g 0xFE08B5
 xQueueGenericSendFro>   Func.   g 0xFE09F2
 xQueueGenericReceive    Func.   g 0xFE0A9D
 xQueueReceiveFromISR    Func.   g 0xFE0BE0
 uxQueueMessagesWaiti>   Func.   g 0xFE0C4A
 uxQueueMessagesWaiti>   Func.   g 0xFE0C5D
 vQueueDelete            Func.   g 0xFE0C66
 ucQueueGetQueueNumbe>   Func.   g 0xFE0C7C
 vQueueSetQueueNumber    Func.   g 0xFE0C86
 ucQueueGetQueueType     Func.   g 0xFE0C93
 prvCopyDataToQueue      Func.   s 0xFE0C9D
 prvCopyDataFromQueue    Func.   s 0xFE0D05
 prvUnlockQueue          Func.   s 0xFE0D33
 prvIsQueueEmpty         Func.   s 0xFE0DA7
 xQueueIsQueueEmptyFr>   Func.   g 0xFE0DC1
 prvIsQueueFull          Func.   s 0xFE0DD1
 xQueueIsQueueFullFro>   Func.   g 0xFE0DEF

 === Module No. 13 (tasks) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 xTaskGenericCreate      Func.   g 0xFE0E03
 vTaskDelete             Func.   g 0xFE0FB8
 vTaskDelayUntil         Func.   g 0xFE101C
 vTaskDelay              Func.   g 0xFE10C2
 uxTaskPriorityGet       Func.   g 0xFE1138
 vTaskPrioritySet        Func.   g 0xFE1155
 vTaskSuspend            Func.   g 0xFE11F4
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    38

 === Module No. 13 (tasks) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 xTaskIsTaskSuspended    Func.   g 0xFE12A7
 vTaskResume             Func.   g 0xFE12E2
 xTaskResumeFromISR      Func.   g 0xFE1374
 vTaskStartScheduler     Func.   g 0xFE1405
 vTaskEndScheduler       Func.   g 0xFE1446
 vTaskSuspendAll         Func.   g 0xFE1450
 xTaskResumeAll          Func.   g 0xFE1455
 xTaskGetTickCount       Func.   g 0xFE15D8
 xTaskGetTickCountFro>   Func.   g 0xFE15E6
 uxTaskGetNumberOfTas>   Func.   g 0xFE15EA
 vTaskList               Func.   g 0xFE15EE
   pcStatusString        loc.    s 0x002470
   pcStatusString        loc.    s 0x002470
   pcStatusString        loc.    s 0x002470
   pcStatusString        loc.    s 0x002470
   pcStatusString        loc.    s 0x002470
 vTaskIncrementTick      Func.   g 0xFE195B
 vTaskSwitchContext      Func.   g 0xFE1A28
 vTaskPlaceOnEventLis>   Func.   g 0xFE1A72
 xTaskRemoveFromEvent>   Func.   g 0xFE1AF6
 vTaskSetTimeOutState    Func.   g 0xFE1B54
 xTaskCheckForTimeOut    Func.   g 0xFE1B67
 vTaskMissedYield        Func.   g 0xFE1BC0
 uxTaskGetTaskNumber     Func.   g 0xFE1BC5
 vTaskSetTaskNumber      Func.   g 0xFE1BD8
 prvIdleTask             Func.   s 0xFE1BE7
 prvInitialiseTCBVari>   Func.   s 0xFE1BF0
 prvInitialiseTaskLis>   Func.   s 0xFE1C3D
 prvCheckTasksWaiting>   Func.   s 0xFE1C8D
 prvAddCurrentTaskToD>   Func.   s 0xFE1CDF
 prvAllocateTCBAndSta>   Func.   s 0xFE1D2C
 prvListTaskWithinSin>   Func.   s 0xFE1D75
   pcStatusString        loc.    s 0x002470
 usTaskCheckFreeStack>   Func.   s 0xFE1E0C
 uxTaskGetStackHighWa>   Func.   g 0xFE1E28
 prvDeleteTCB            Func.   s 0xFE1E55
 xTaskGetCurrentTaskH>   Func.   g 0xFE1E70
 xTaskGetSchedulerSta>   Func.   g 0xFE1E74
 vTaskPriorityInherit    Func.   g 0xFE1E87
 vTaskPriorityDisinhe>   Func.   g 0xFE1EF9
 pxCurrentTCB            Var.    g 0x00228C
 pxReadyTasksLists       Var.    s 0x002434
 xDelayedTaskList1       Var.    s 0x00242A
 xDelayedTaskList2       Var.    s 0x002420
 pxDelayedTaskList       Var.    s 0x00241E
 pxOverflowDelayedTas>   Var.    s 0x00241C
 xPendingReadyList       Var.    s 0x002412
 xTasksWaitingTermina>   Var.    s 0x002408
 uxTasksDeleted          Var.    s 0x00228A
 xSuspendedTaskList      Var.    s 0x0023FE
 uxCurrentNumberOfTas>   Var.    s 0x002288
 xTickCount              Var.    s 0x002286
 uxTopUsedPriority       Var.    s 0x002284
 uxTopReadyPriority      Var.    s 0x002282
 xSchedulerRunning       Var.    s 0x002280
 uxSchedulerSuspended    Var.    s 0x00227E
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    39

 === Module No. 13 (tasks) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 uxMissedTicks           Var.    s 0x00227C
 xMissedYield            Var.    s 0x00227A
 xNumOfOverflows         Var.    s 0x002278
 uxTaskNumber            Var.    s 0x002276
 xNextTaskUnblockTime    Var.    s 0x002274

 === Module No. 15 (__std_lib_sbrk) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 sbrk                    Func.   g 0xFE1F41
   brk_siz               loc.    s 0x00228E
   _heap                 loc.    s 0x0024A2

 === Module No. 16 (port) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 pxPortInitialiseStac>   Func.   g 0xFE1F6E
 prvSetupRLT0Interrup>   Func.   s 0xFE2040
 xPortStartScheduler     Func.   g 0xFE2049
 vPortEndScheduler       Func.   g 0xFE2080
 prvRLT0_TICKISR         Func.   g 0xFE2081
 vPortYield              Func.   g 0xFE208D
 vPortYieldDelayed       Func.   g 0xFE20EB

 === Module No. 17 (heap_3) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 pvPortMalloc            Func.   g 0xFE2163
 vPortFree               Func.   g 0xFE217D

 === Module No. 18 (wireless) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 wirelessSend            Func.   g 0xFE2195
 wirelessDebugMessage    Func.   g 0xFE21C2
 wirelessFormattedDeb>   Func.   g 0xFE2211
 wirelessInit            Func.   g 0xFE226D
 wirelessSubscribe       Func.   g 0xFE229B
 isSubscribed            Func.   g 0xFE22E0
 wirelessUnsubscribe     Func.   g 0xFE2312
 wirelessReceive         Func.   g 0xFE234E
   subscribers           loc.    s 0x005B52
   msg                   loc.    s 0x005C06

 === Module No. 19 (bluetooth) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 Bluetooth_Layer1_Get>   Func.   s 0xFE238C
 Bluetooth_Init          Func.   g 0xFE23B1
 getNextMessageToSend    Func.   s 0xFE246B
 putStuffedByte          Func.   s 0xFE248C
 startSendingNextMess>   Func.   s 0xFE24AF
   message               loc.    s 0x005CA6
 UART7_RxISR             Func.   s 0xFE25DB
 UART7_TxISR             Func.   s 0xFE260B
 Bluetooth_Layer3_Rec>   Func.   s 0xFE262E
 Bluetooth_Send          Func.   g 0xFE274A
   btxRxedChars          loc.    s 0x005CA4
   sendSM                loc.    s 0x005C4E
   messagesLeftToSend    loc.    s 0x005CEB
   sendQueue             loc.    s 0x005C4C
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    40

 === Module No. 20 (amb8420) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 AMB8420_config          Func.   s 0xFE2770
 AMB8420_Init            Func.   g 0xFE2A66
 sendCommandBuffer       Func.   s 0xFE2B74
 AMB8420_CS_calc         Func.   s 0xFE2B8B
 getNextMessageToSend    Func.   s 0xFE2BBB
   prioCounter           loc.    s 0x002292
 AMB8420_startSending>   Func.   s 0xFE2C9E
   message               loc.    s 0x005D64
   prioCounter           loc.    s 0x002292
 AMB8420_Send            Func.   g 0xFE2E59
 UART8_TxISR             Func.   s 0xFE2EAF
 writeCommand            Func.   s 0xFE2ED2
 readCommand             Func.   s 0xFE2F4B
 AMB8420_GetChar_Asyn>   Func.   s 0xFE2FC5
 replyReceived           Func.   s 0xFE2FEA
 AMB8420_ReceiveMessa>   Func.   s 0xFE3044
   returnedState         loc.    s 0x005D54
 UART8_RxISR             Func.   s 0xFE31F9
   ambTask               loc.    s 0x005D52
   ambInitTask           loc.    s 0x005D50
   messagesLeftToSend    loc.    s 0x005DA9
   sendQueuePrio1        loc.    s 0x005D4E
   sendQueuePrio2        loc.    s 0x005D4C
   sendQueuePrio3        loc.    s 0x005D4A
   receiveSM             loc.    s 0x005D46
   sendSM                loc.    s 0x005CEC

 === Module No. 21 (adc) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 ADC_Init                Func.   g 0xFE322B
 ADC_GetValue            Func.   g 0xFE32D5
 irq_adc                 Func.   s 0xFE32FB
   adc_values            loc.    s 0x005DAA

 === Module No. 22 (api) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 drive_barrierFrontDe>   Func.   g 0xFE3332
 drive_barrierRearDet>   Func.   g 0xFE3345
 api_init                Func.   g 0xFE3358
 driven_before           Func.   g 0xFE33C4
 us_getFrontDistance     Func.   g 0xFE33D6
 us_getLeftDistance      Func.   g 0xFE33DA
 us_getRightDistance     Func.   g 0xFE33DE
 us_getSensorMapData     Func.   g 0xFE33E2
 api_StartScheduler      Func.   g 0xFE34F5
 os_registerProcess      Func.   g 0xFE34FC
 os_registerProcessSt>   Func.   g 0xFE351F
 os_registerProcessSt>   Func.   g 0xFE3549
 os_deleteTask           Func.   g 0xFE3576
 os_suspendTask          Func.   g 0xFE3582
 os_resumeTask           Func.   g 0xFE358E
 os_isTaskSuspended      Func.   g 0xFE359A
 os_wait                 Func.   g 0xFE35A6
 os_frequency            Func.   g 0xFE35B2
 os_getTime              Func.   g 0xFE35C2
 Sema                    Var.    g 0x002296
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    41

 === Module No. 22 (api) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 getOnlyDrivenMap        Var.    g 0x002294
 wakeLeft                Var.    g 0x005DF0
 wakeRight               Var.    g 0x005DEE
 wakeFront               Var.    g 0x005DEC
 wakeRear                Var.    g 0x005DEA

 === Module No. 23 (battery) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 Battery_Init            Func.   g 0xFE35C6
 Battery_GetVoltage      Func.   g 0xFE35F7
 battery_task            Func.   s 0xFE361D
   measures              loc.    s 0x005DF2
   nextPos               loc.    s 0x005E32

 === Module No. 24 (carid) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 CarId_Init              Func.   g 0xFE3679
 carid                   Var.    g 0x002298

 === Module No. 25 (drive) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 Drive_SetServo          Func.   g 0xFE3694
 Drive_SetMotor          Func.   g 0xFE36B5
 Drive_SetMotorForDis>   Func.   g 0xFE36FA
 Drive_Init              Func.   g 0xFE3750
 Drive_GetTotalDriven>   Func.   g 0xFE381A
 Drive_DistanceInterr>   Func.   s 0xFE384B
 Drive_Systick           Func.   s 0xFE388F
   ppg_motor_last_pos>   loc.    s 0x00229A
   lastPower             loc.    s 0x0022B9
   lastServo             loc.    s 0x0022B8
 Drive_Servo             Var.    g 0x0022BB
 Drive_Speed             Var.    g 0x0022BA
 Drive_TotalDistance     Var.    s 0x0022B4
 ppg_motor_values        Var.    s 0x00229E
 lastReloadValue         Var.    s 0x00229C

 === Module No. 26 (fram) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 FRAM_Init               Func.   g 0xFE3987
 FRAM_Available          Func.   g 0xFE3A11
 FRAM_Write              Func.   g 0xFE3A36
 FRAM_Read               Func.   g 0xFE3A9A
   enabledModules        loc.    s 0x005E34

 === Module No. 27 (i2c) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 I2C_BUS0_Init           Func.   g 0xFE3B0C
 I2C_BUS0_Acknowlegde    Func.   g 0xFE3B41
 I2C_BUS0_CheckAddres>   Func.   g 0xFE3B46
 I2C_BUS0_Start          Func.   g 0xFE3B77
 I2C_BUS0_Continue       Func.   g 0xFE3BAD
 I2C_BUS0_Stop           Func.   g 0xFE3BC9
 I2C_BUS0_Write          Func.   g 0xFE3BD8
 I2C_BUS0_Read           Func.   g 0xFE3BEC
 I2C_BUS0_LastRead       Func.   g 0xFE3BF9
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    42

 === Module No. 27 (i2c) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 I2C_BUS1_Init           Func.   g 0xFE3C06
 I2C_BUS1_Acknowlegde    Func.   g 0xFE3C41
 I2C_BUS1_CheckAddres>   Func.   g 0xFE3C46
 I2C_BUS1_Start          Func.   g 0xFE3C77
 I2C_BUS1_Continue       Func.   g 0xFE3CAD
 I2C_BUS1_Stop           Func.   g 0xFE3CC9
 I2C_BUS1_Write          Func.   g 0xFE3CD8
 I2C_BUS1_Read           Func.   g 0xFE3CEC
 I2C_BUS1_LastRead       Func.   g 0xFE3CF9

 === Module No. 28 (interrupts) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 interrupts_init         Func.   g 0xFE3D06
 interrupts_setHandle>   Func.   g 0xFE3D63
 interrupts_disableIn>   Func.   g 0xFE3E01
   ivt                   loc.    s 0x007200
   initialized           loc.    s 0x0022BC

 === Module No. 29 (linesensor) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 Linesensor_Init         Func.   g 0xFE3E77
 readSensorData          Func.   g 0xFE3E9B
 Linesensor_calcLineP>   Func.   s 0xFE3EBE
 Linesensor_Calibrate    Func.   g 0xFE40E2
 linefound               Var.    g 0x00231C
 averagePos              Var.    g 0x002316
 averagePosNew           Var.    g 0x002312
 averagePosOld           Var.    g 0x00230E
 final_sensor            Var.    g 0x002304
 sensor                  Var.    g 0x0022FA
 cal_sensor              Var.    g 0x0022F0
 calibrated_sensor       Var.    g 0x0022E6
 pos                     Var.    g 0x0022BE
 noline                  Var.    g 0x00231B
 bcLineFound             Var.    g 0x00231A
 xHandleCalcLinePos      Var.    g 0x005E36

 === Module No. 30 (printf_stdarg) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 prints                  Func.   s 0xFE4195
 print                   Func.   g 0xFE4252
 printf                  Func.   g 0xFE4588
 sprintf                 Func.   g 0xFE459D
 snprintf                Func.   g 0xFE45B4

 === Module No. 31 (regler) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 drive_barrierFrontDe>   Func.   g 0xFE45CB
 drive_barrierRearDet>   Func.   g 0xFE45DE
 api_init                Func.   g 0xFE45F1
 driven_before           Func.   g 0xFE465D
 us_getFrontDistance     Func.   g 0xFE466F
 us_getLeftDistance      Func.   g 0xFE4673
 us_getRightDistance     Func.   g 0xFE4677
 us_getSensorMapData     Func.   g 0xFE467B
 api_StartScheduler      Func.   g 0xFE478E
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    43

 === Module No. 31 (regler) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 os_registerProcess      Func.   g 0xFE4795
 os_registerProcessSt>   Func.   g 0xFE47B8
 os_registerProcessSt>   Func.   g 0xFE47E2
 os_deleteTask           Func.   g 0xFE480F
 os_suspendTask          Func.   g 0xFE481B
 os_resumeTask           Func.   g 0xFE4827
 os_isTaskSuspended      Func.   g 0xFE4833
 os_wait                 Func.   g 0xFE483F
 os_frequency            Func.   g 0xFE484B
 os_getTime              Func.   g 0xFE485B
 Regler_set_Kp           Func.   g 0xFE485F
 Regler_set_Ki           Func.   g 0xFE4876
 Regler_set_Kd           Func.   g 0xFE488D
 Regler_output           Func.   g 0xFE48A4
 Regler_get_sensor       Func.   g 0xFE48B0
 Regler_pid              Func.   g 0xFE48B4
   esum                  loc.    s 0x002322
   e_alt                 loc.    s 0x002320
   e_aelter              loc.    s 0x00231E
   e                     loc.    s 0x005E38
 Kp                      Var.    g 0x005E52
 Ki                      Var.    g 0x005E4A
 Kd                      Var.    g 0x005E42
 Sema                    Var.    g 0x002326
 getOnlyDrivenMap        Var.    g 0x002324
 wakeLeft                Var.    g 0x005E40
 wakeRight               Var.    g 0x005E3E
 wakeFront               Var.    g 0x005E3C
 wakeRear                Var.    g 0x005E3A

 === Module No. 32 (runtime_stats) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 RuntimeStats_Print      Func.   g 0xFE4952
 runTimerValue           Var.    g 0x005E5A

 === Module No. 33 (seg) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 Seg_Init                Func.   g 0xFE4953
 Seg_Hex                 Func.   g 0xFE495C
 Seg_Dec                 Func.   g 0xFE4977
   DEC7SEG               loc.    s 0x002328

 === Module No. 34 (serial) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 Serial_GetcharAsync     Func.   g 0xFE4992
 Serial_Getchar          Func.   g 0xFE49B7
 Serial_Init             Func.   g 0xFE49E8
 Serial_PutChar          Func.   g 0xFE4A5A
 UART0_RxISR             Func.   s 0xFE4A96
 UART0_TxISR             Func.   s 0xFE4AC5
   xRxedChars            loc.    s 0x005E62
   xCharsForTx           loc.    s 0x005E60
   sTHREEmpty            loc.    s 0x005E5E
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    44

 === Module No. 35 (speed) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 drive_barrierFrontDe>   Func.   g 0xFE4AF3
 drive_barrierRearDet>   Func.   g 0xFE4B06
 api_init                Func.   g 0xFE4B19
 driven_before           Func.   g 0xFE4B85
 us_getFrontDistance     Func.   g 0xFE4B97
 us_getLeftDistance      Func.   g 0xFE4B9B
 us_getRightDistance     Func.   g 0xFE4B9F
 us_getSensorMapData     Func.   g 0xFE4BA3
 api_StartScheduler      Func.   g 0xFE4CB6
 os_registerProcess      Func.   g 0xFE4CBD
 os_registerProcessSt>   Func.   g 0xFE4CE0
 os_registerProcessSt>   Func.   g 0xFE4D0A
 os_deleteTask           Func.   g 0xFE4D37
 os_suspendTask          Func.   g 0xFE4D43
 os_resumeTask           Func.   g 0xFE4D4F
 os_isTaskSuspended      Func.   g 0xFE4D5B
 os_wait                 Func.   g 0xFE4D67
 os_frequency            Func.   g 0xFE4D73
 os_getTime              Func.   g 0xFE4D83
 Kp                      Var.    g 0x005E7C
 Ki                      Var.    g 0x005E74
 Kd                      Var.    g 0x005E6C
 Sema                    Var.    g 0x00233A
 getOnlyDrivenMap        Var.    g 0x002338
 wakeLeft                Var.    g 0x005E6A
 wakeRight               Var.    g 0x005E68
 wakeFront               Var.    g 0x005E66
 wakeRear                Var.    g 0x005E64

 === Module No. 36 (trigonometry) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 sin_                    Func.   g 0xFE4D87
 cos_                    Func.   g 0xFE4D98
 uint16AngleToDegree     Func.   g 0xFE4DCB
 AnglePlus               Func.   g 0xFE4DF4
 AngleMinus              Func.   g 0xFE4E26
 uint16DegreeRelative>   Func.   g 0xFE4E49
 angleToDegree           Func.   g 0xFE4E92
 DegreeToUint16          Func.   g 0xFE4EAF
 cos_Taylor              Func.   g 0xFE4EFC
 sin_Taylor              Func.   g 0xFE50A2
   sinus                 loc.    s 0xFF81AC

 === Module No. 37 (us) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 Us_Init                 Func.   g 0xFE524A
 Us_Cmd_Wait             Func.   g 0xFE5291
 Us_Calibrate            Func.   g 0xFE52A1
 Us_IO_DDR               Func.   g 0xFE52BC
 Us_IO_Value             Func.   g 0xFE5303
 Us_Systick              Func.   s 0xFE534A
 irq_us2_Rx              Func.   g 0xFE5360
 Us_Data                 Var.    g 0x005E84
 rxBuffer2               Var.    s 0x005E8E
 timeout2                Var.    s 0x00233E
 cmdReplyWait2           Var.    s 0x00233C
Local Symbol List                                                                                   2014-11-11 19:55:42  Page:    45

 === Module No. 38 (us_i2c) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 I2CUs_Init              Func.   g 0xFE53E8
 I2CUs_Systick           Func.   g 0xFE547D
   moduleEnabled1        loc.    s 0x002342
   moduleEnabled2        loc.    s 0x002341
   moduleEnabled3        loc.    s 0x002340

 === Module No. 39 (util) ===
 --- symbol ---       -- kind --   --- val ---                                                                                     C
 abs8                    Func.   g 0xFE55EF
 abs16                   Func.   g 0xFE55FD
 abs32                   Func.   g 0xFE560A
 intToStr                Func.   g 0xFE5621
 wait                    Func.   g 0xFE569A
 wait1us                 Func.   g 0xFE56B2
 wait1ms                 Func.   g 0xFE56C4
