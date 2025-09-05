/*
 * 05 - ADC
 *
 *  Created on: July 2023
 *      Author: Eng. Nelson Yamaguti
 */

#include "Peripheral_Setup.h"


void Setup_GPIO(void){
    EALLOW;

    // ---------------------------------------- GPIO Mux Selection ---------------------------------- //
       // GMux =    0   |    0   |      0      |      0     |      1      |     1    |    1   |    3     //
       // Mux  =    0   |    1   |      2      |      3     |      1      |     2    |    3   |    3     //
       //      |  GPIO0 | EPWM1A |             |            |             |   SDAA   |        |          //
       //      |  GPIO2 | EPWM2A |             |            | OUTPUTXBAR1 |   SDAB   |        |          //
       //      |  GPIO4 | EPWM3A |             |            | OUTPUTXBAR3 |  CANTXA  |        |          //
       //      |  GPIO6 | EPWM4A | OUTPUTXBAR4 | EXTSYNCOUT |    EQEP3A   |  CANTXB  |        |          //
       //      |  GPIO8 | EPWM5A |    CANTXB   |  ADCSOCAO  |    EQEP3S   |  SCITXDA |        |          //
       //      | GPIO10 | EPWM6A |    CANRXB   |  ADCSOCBO  |    EQEP1A   |  SCITXDB |        | UPP-WAIT //
       //      | GPIO61 |  MFSRB |    EM1D23   |    EM2D7   | OUTPUTXBAR4 | SPISOMIB | SD2_C3 | SPISTEA  //
       // ---------------------------------------------------------------------------------------------  //


    // GPIO0 EPWM1A Pin40 J4
    GpioCtrlRegs.GPAGMUX1.bit.GPIO0 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;                    // EPWM1A (GPAGMUX1 = 0 e GPAMUX1 = 1)
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;                     // Internal Pull-Up disabled

    // GPIO1 EPWM1A Pin39 J4
    GpioCtrlRegs.GPAGMUX1.bit.GPIO1 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;                    // EPWM1A (GPAGMUX1 = 0 e GPAMUX1 = 1)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;                     // Internal Pull-Up disabled

    // GPIO2 EPWM2A Pin38 J4
     GpioCtrlRegs.GPAGMUX1.bit.GPIO2 = 0;
     GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;  // EPWM2A (GPAGMUX1 = 0 e GPAMUX1 = 1)
     GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;   // Internal Pull-Up disabled

     // GPIO3 EPWM2A Pin37 J4
      GpioCtrlRegs.GPAGMUX1.bit.GPIO3 = 0;
      GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;  // EPWM2A (GPAGMUX1 = 0 e GPAMUX1 = 1)
      GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;   // Internal Pull-Up disabled

     // GPIO4 EPWM3A Pin36 J4
     GpioCtrlRegs.GPAGMUX1.bit.GPIO4 = 0;
     GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;  // EPWM3A (GPAGMUX1 = 0 e GPAMUX1 = 1)
     GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;   // Internal Pull-Up disabled

     // GPIO5 EPWM2A Pin35 J4
      GpioCtrlRegs.GPAGMUX1.bit.GPIO5 = 0;
      GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;  // EPWM2A (GPAGMUX1 = 0 e GPAMUX1 = 1)
      GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;   // Internal Pull-Up disabled

     // GPIO6 EPWM4A Pin80 J8
     GpioCtrlRegs.GPAGMUX1.bit.GPIO6 = 0;
     GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;  // EPWM4A (GPAGMUX1 = 0 e GPAMUX1 = 1)
     GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;   // Internal Pull-Up disabled

     // GPIO8 EPWM5A Pin78 J8
     GpioCtrlRegs.GPAGMUX1.bit.GPIO8 = 0;
     GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;  // EPWM5A (GPAGMUX1 = 0 e GPAMUX1 = 1)
     GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;   // Internal Pull-Up disabled

     // GPIO10 Pin76 J8 - Flag da Potência
     GpioCtrlRegs.GPAGMUX1.bit.GPIO10 = 0;
     GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;
     GpioCtrlRegs.GPADIR.bit.GPIO10 = 1; // Configure GPIO10 as a Output
     GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1; // Disable Pull-Up Resistance

     // GPIO11 Pin75 J8 - Flag Pflow
     GpioCtrlRegs.GPAGMUX1.bit.GPIO11 = 0;
     GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;
     GpioCtrlRegs.GPADIR.bit.GPIO11 = 1; // Configure GPIO10 as a Output
     GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1; // Disable Pull-Up Resistance

     // GPIO66 Pin59 J8 - Flag Trigger
     GpioCtrlRegs.GPCGMUX1.bit.GPIO66 = 0;
     GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 0;
     GpioCtrlRegs.GPCDIR.bit.GPIO66 = 1; // Configure GPIO10 as a Output
     GpioCtrlRegs.GPCPUD.bit.GPIO66 = 1; // Disable Pull-Up Resistance

     // GPIO61 Switch
     GpioCtrlRegs.GPBGMUX2.bit.GPIO61 = 0;
     GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;
     GpioCtrlRegs.GPBDIR.bit.GPIO61 = 0;  // Config GPIO61 as a Input
     GpioCtrlRegs.GPBPUD.bit.GPIO61 = 1;  // Disable pull-Up Resistence

     // GPIO123 Switch
     GpioCtrlRegs.GPDGMUX2.bit.GPIO123 = 0;
     GpioCtrlRegs.GPDMUX2.bit.GPIO123 = 0;
     GpioCtrlRegs.GPDDIR.bit.GPIO123 = 0;  // Config GPIO61 as a Input
     GpioCtrlRegs.GPDPUD.bit.GPIO123 = 1;  // Disable pull-Up Resistence

     // GPIO58 Switch AQUISICAO
     GpioCtrlRegs.GPBGMUX2.bit.GPIO58 = 0;
     GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;
     GpioCtrlRegs.GPBDIR.bit.GPIO58 = 0;  // Config GPIO61 as a Input
     GpioCtrlRegs.GPBPUD.bit.GPIO58 = 1;  // Disable pull-Up Resistence


     EDIS;
}

void Setup_ePWM(void){
    #define SYSCLK 200e6                                   // System Clock
    #define Fs 20e3                                        // Sampling Frequency

    EALLOW;
    CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;                      // Enable EPWM1 clock
    CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;                 // Enable EPWM2 clock
    CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;                 // Enable EPWM3 clock
    CpuSysRegs.PCLKCR2.bit.EPWM4 = 1;                 // Enable EPWM4 clock
    CpuSysRegs.PCLKCR2.bit.EPWM5 = 1;                 // Enable EPWM5 clock

    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;                  // Disable counter clock

    // ------------------- EPWM1 -------------------- //

    // TIME-BASE
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;         // Count up/down
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;                // Disable phase loading
    EPwm1Regs.TBPHS.bit.TBPHS = 0;
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;        // Disable PWM drive synchronization with external sync signal
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;               // High Speed TB Clock Prescale (SYSCLK = /1)
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;                  // TB Clock Prescale (SYSCLK = /1)
    EPwm1Regs.TBPHS.bit.TBPHSHR = 0;                       // Phase Shift is 0
    EPwm1Regs.TBCTR = 0x0000;                              // Clear counter
    EPwm1Regs.TBPRD = __divf32(SYSCLK, 4*Fs);              // TBPRD = ((200e6)/(4*25e3)) = 2000

    // COMPARE
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;      // CMPA value is loaded when the counter is still ZERO and PRD
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;            // Load registers every zero
    EPwm1Regs.CMPA.bit.CMPA = 0;                           // Duty cycle
//    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
//    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    // ACTION QUALIFIER saida pwm
    EPwm1Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;                   // PWM output pin in a Low logic level (0)
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;                     // PWM output pin in a High logic level (1)

    // DEAD-BAND
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;         // Enable Dead-band module
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;              // Active Hi complementary
    //EPwm1Regs.DBRED.bit.DBRED = 100;                     // RED = 20 TBCLKs
    //EPwm1Regs.DBFED.bit.DBFED = 100;                     // FED = 20 TBCLKs

    // Trigger ADC
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;                        // Enable SOC on A group
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;          // Generates an output signal when the
                                                           // EPWM counter reaches zero and PRD
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;                   // Trigger on every event

    // ------------------- EPWM2 -------------------- //

    // TIME-BASE
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Count up and down mode
    EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;            // Phase Register Enable
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;        // ePWM2 module to synchronize with ePWM1
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;          // High Speed TB Clock Prescale (SYSCLK = /1)
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;             // TB Clock Prescale (SYSCLK = /1)
    EPwm2Regs.TBCTL.bit.PHSDIR = TB_DOWN;                   // Phase Direction 1 (count up after sync)
    EPwm2Regs.TBPHS.bit.TBPHS = 2*EPwm1Regs.TBPRD/3;  // Create a phase delay (2*180 /3 = 120 );  // Create a phase delay (2*180 /3 = 120 )
    EPwm2Regs.TBCTR = 0x0000;                         // Clear counter
    EPwm2Regs.TBPRD = EPwm1Regs.TBPRD;                // EPWM2 with the same sampling frequency as EPWM1

    // COMPARE
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD; // CMPA value is loaded when the counter is still ZERO and PRD
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;       // Load registers every zero

    // ACTION QUALIFIER
    EPwm2Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;              // PWM output pin in a Low logic level (0)
    EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;                // PWM output pin in a High logic level (1)

    // DEAD-BAND
    EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;    // Enable Dead-band module
    EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;         // Active Hi complementary
    // EPwm2Regs.DBRED.bit.DBRED = 10;                // RED = 20 TBCLKs
    // EPwm2Regs.DBFED.bit.DBFED = 10;                // FED = 20 TBCLKs

    // -------------------- EPWM3 ------------------- //

    // TIME-BASE
    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Count up and down mode
    EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;            // Phase Register Enable
    EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;        // ePWM3 module to synchronize with ePWM1
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;          // High Speed TB Clock Prescale (SYSCLK = /1)
    EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;             // TB Clock Prescale (SYSCLK = /1)
    EPwm3Regs.TBCTL.bit.PHSDIR = TB_UP;                   // Phase Direction 1 (count down after sync)
    EPwm3Regs.TBPHS.bit.TBPHS = 2*EPwm1Regs.TBPRD/3;  // Create a phase delay (2*180 /3 = 120 );  // Create a phase delay (2*180 /3 = 120 )
    EPwm3Regs.TBCTR = 0x0000;                         // Clear counter
    EPwm3Regs.TBPRD = EPwm1Regs.TBPRD;                // EPWM3 with the same sampling frequency as EPWM1

    // COMPARE
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD; // CMPA value is loaded when the counter is still ZERO and PRD
    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;       // Load registers every zero

    // ACTION QUALIFIER
    EPwm3Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;              // PWM output pin in a Low logic level (0)
    EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;                // PWM output pin in a High logic level (1)

    // DEAD-BAND
    EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;    // Enable Dead-band module
    EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;         // Active Hi complementary
    // EPwm3Regs.DBRED.bit.DBRED = 10;                // RED = 20 TBCLKs
    // EPwm3Regs.DBFED.bit.DBFED = 10;                // FED = 20 TBCLKs

    // ------------------- EPWM4 -------------------- //

    // TIME-BASE
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Count up and down mode
    EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE;           // Phase Register Enable
    EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;       // ePWM2 module to synchronize with ePWM1
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;          // High Speed TB Clock Prescale (SYSCLK = /1)
    EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;             // TB Clock Prescale (SYSCLK = /1)
    EPwm4Regs.TBCTR = 0x0000;                         // Clear counter
    EPwm4Regs.TBPRD = EPwm1Regs.TBPRD;                // EPWM4 with the same sampling frequency as EPWM1

    // COMPARE
    EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD; // CMPA value is loaded when the counter is still ZERO and PRD
    EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;       // Load registers every zero

    // ACTION QUALIFIER
    EPwm4Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;              // PWM output pin in a Low logic level (0)
    EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;                // PWM output pin in a High logic level (1)

    // DEAD-BAND
    EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;    // Enable Dead-band module
    EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;         // Active Hi complementary
    // EPwm4Regs.DBRED.bit.DBRED = 10;                // RED = 20 TBCLKs
    // EPwm4Regs.DBFED.bit.DBFED = 10;                // FED = 20 TBCLKs

    // ------------------- EPWM5 -------------------- //

    // TIME-BASE
    EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;    // Count up and down mode
    EPwm5Regs.TBCTL.bit.PHSEN = TB_DISABLE;           // Phase Register Enable
    EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;       // ePWM3 module to synchronize with ePWM1
    EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;          // High Speed TB Clock Prescale (SYSCLK = /1)
    EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1;             // TB Clock Prescale (SYSCLK = /1)
    EPwm5Regs.TBCTR = 0x0000;                         // Clear counter
    EPwm5Regs.TBPRD = EPwm1Regs.TBPRD;                // EPWM3 with the same sampling frequency as EPWM1

    // COMPARE
    EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD; // CMPA value is loaded when the counter is still ZERO and PRD
    EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;       // Load registers every zero

    // ACTION QUALIFIER
    EPwm5Regs.AQCTLA.bit.ZRO = AQ_NO_ACTION;
    EPwm5Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;              // PWM output pin in a Low logic level (0)
    EPwm5Regs.AQCTLA.bit.CAD = AQ_SET;                // PWM output pin in a High logic level (1)

    // DEAD-BAND
    EPwm5Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;    // Enable Dead-band module
    EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;         // Active Hi complementary
    // EPwm5Regs.DBRED.bit.DBRED = 10;                // RED = 20 TBCLKs
    // EPwm5Regs.DBFED.bit.DBFED = 10;                // FED = 20 TBCLKs

    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;             // Enable counter clock
    EDIS;
}

void Setup_ADC(void){
    Uint16 acqps = 14;
    EALLOW;

    CpuSysRegs.PCLKCR13.bit.ADC_A = 1;                     // Enable ADC clock
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;                  // Set pulse on cycle prior to result
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;                     // Power up the ADC
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6;                     // Set ADCCLK divider to /4 ((200 MHz)/4 = 50 MHz)
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    CpuSysRegs.PCLKCR13.bit.ADC_C = 1;                     // Enable ADC clock
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;                  // Set pulse on cycle prior to result
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;                     // Power up the ADC
    AdccRegs.ADCCTL2.bit.PRESCALE = 6;                     // Set ADCCLK divider to /4 ((200 MHz)/4 = 50 MHz)
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    CpuSysRegs.PCLKCR13.bit.ADC_B = 1;                     // Enable ADC clock
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;                  // Set pulse on cycle prior to result
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;                     // Power up the ADC
    AdcbRegs.ADCCTL2.bit.PRESCALE = 6;                     // Set ADCCLK divider to /4 ((200 MHz)/4 = 50 MHz)
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    DELAY_US(1000);                                        // Delay for 1ms to allow ADC time to power up

    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 14;                    // SOC0 will convert ADCINA14 (Pin 23 | Typhoon AO2) -> Vc2
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps;                 // SOC0 will use sample duration of 15 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = TRIG_SEL_ePWM1_SOCA; // SOC0 will begin conversion on ePWM1 SOCA

    AdccRegs.ADCSOC0CTL.bit.CHSEL = 3;                     // SOC0 will convert ADCINC3 (Pin 24 | Typhoon AO3) -> i1
    AdccRegs.ADCSOC0CTL.bit.ACQPS = acqps;                 // SOC0 will use sample duration of 15 SYSCLK cycles
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = TRIG_SEL_ePWM1_SOCA; // SOC0 will begin conversion on ePWM1 SOCA

    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 3;                     // SOC0 will convert ADCINB3 (Pin 25 | Typhoon AO4) -> Vg
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = acqps;                 // SOC0 will use sample duration of 15 SYSCLK cycles
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = TRIG_SEL_ePWM1_SOCA; // SOC0 will begin conversion on ePWM1 SOCA

    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 3;                     // SOC1 will convert ADCINA3 (Pin 26 | Typhoon AO5) -> i(RLi)
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = acqps;                 // SOC1 will use sample duration of 15 SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = TRIG_SEL_ePWM1_SOCA; // SOC1 will begin conversion on ePWM1 SOCA

    AdccRegs.ADCSOC1CTL.bit.CHSEL = 2;                     // SOC1 will convert ADCINC2 (Pin 27 | Typhoon AO7) -> iBat
    AdccRegs.ADCSOC1CTL.bit.ACQPS = acqps;                 // SOC1 will use sample duration of 15 SYSCLK cycles
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL = TRIG_SEL_ePWM1_SOCA; // SOC1 will begin conversion on ePWM1 SOCA

// --------------Entradas não utilizadas-------------------//

    AdcbRegs.ADCSOC1CTL.bit.CHSEL = 2;                     // SOC1 will convert ADCINB2 (Pin 28 | Typhoon AO8)
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = acqps;                 // SOC1 will use sample duration of 15 SYSCLK cycles
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = TRIG_SEL_ePWM1_SOCA; // SOC1 will begin conversion on ePWM1 SOCA

    AdcaRegs.ADCSOC2CTL.bit.CHSEL = 2;                     // SOC2 will convert ADCINA2 (Pin 29 | Typhoon AO9)
    AdcaRegs.ADCSOC2CTL.bit.ACQPS = acqps;                 // SOC2 will use sample duration of 15 SYSCLK cycles
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = TRIG_SEL_ePWM1_SOCA; // SOC2 will begin conversion on ePWM1 SOCA

    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0x1;               // end of SOC1 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;                   // enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;                 // make sure INT1 flag is cleared

    EDIS;
}
