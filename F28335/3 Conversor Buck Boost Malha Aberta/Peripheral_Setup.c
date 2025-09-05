#include "DSP28x_Project.h"
#include "DSP2833x_Device.h"
#include "Peripheral_Setup.h"

void Setup_GPIO(void) {
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;             // GPIO0 -> ePWM1A
    EDIS;
}

void Setup_ePWM(void) {
    // Ativação e configuração do ePWM1A para gerar PWM e dar trigger em adc_isr()

    #define SYS_CLK 150e6
    #define Fs 100e3

    EALLOW;
    /* -------------------------------------------------------------------------- */
    /* ---------------------- CONFIGURAÇÕES ESSENCIAIS -------------------------- */
    /* -------------------------------------------------------------------------- */

    // 1. Habilitação de Clock
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;

    // 2. Configuração Básica do Time-Base
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // Modo triangular (UP-DOWN)
    EPwm1Regs.TBPRD = SYS_CLK/(2*Fs);               // Período para frequência desejada
    EPwm1Regs.TBPHS.half.TBPHS = 0;                 // Fase inicial zerada
    EPwm1Regs.TBCTR = 0;                            // Inicia contador em 0

    // 3. Configuração de Compare e Action Qualifier
    EPwm1Regs.CMPA.half.CMPA = EPwm1Regs.TBPRD*0.5; // Duty cycle inicial de 50%
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;            // Limpa ePWM1A na subida (CMPA)
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;              // Seta ePWM1A na descida (CMPA)

    /* -------------------------------------------------------------------------- */
    /* ---------------------- CONFIGURAÇÕES REDUNDANTES  ------------------------ */
    /* -------------------------------------------------------------------------- */

    // Time-Base defaults
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;         // Padrão: fase desabilitada
    EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;          // Padrão: shadow register ativo
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // Padrão: divisor 1
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;           // Padrão: divisor 1

    /* -------------------------------------------------------------------------- */
    /* ---------------------- CONFIGURAÇÕES AVANÇADAS --------------------------- */
    /* -------------------------------------------------------------------------- */

    // 1. Configuração do SOC para ADC
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;                 // Habilita SOC para ADC
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;      // Dispara no ZERO do contador
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;            // Gera SOC no primeiro evento

//    // 2. Sincronização entre módulos PWM
//    EPwm1Regs.TBCTL.bit.SYNCOSEL = 2;               // Outros PWMs podem sincronizar com ePWM1
//
//    // 3. Configuração do Dead-Band
//    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // Dead-Band habilitado
//    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;       // Configuração para inversores
//    EPwm1Regs.DBRED = 10;                           // Rising Edge Delay
//    EPwm1Regs.DBFED = 10;                           // Falling Edge Delay
    EDIS;
}

void Setup_ADC(void) {
    // Configuração dos 2 ADCs usados para ler "iL" e "Vbuck" ou "Vboost"
    InitAdc();

    EALLOW;
    // 1. Configuração de clock e tempo de aquisição
    AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x3;      // HSPCLK/4 = 25 MHz (para HSPCLK = 100MHz)
    AdcRegs.ADCTRL1.bit.ACQ_PS = 0xF;        // Janela de aquisição = 24 ADCCLK (960ns @25MHz)

    // 2. Configuração da sequência de conversão
    AdcRegs.ADCMAXCONV.all = 0x0001;          // 2 conversões (CONV00 e CONV01)
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;    // ADCINA0 -> Corrente (iL)
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x2;    // ADCINA2 -> Tensão (Vbuck)   // ADCINA1 -> Vboost

    // 3. Trigger e interrupção
    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;   // Trigger por ePWM1 (SOCA)
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;     // Habilita interrupção após sequência
    EDIS;
}
