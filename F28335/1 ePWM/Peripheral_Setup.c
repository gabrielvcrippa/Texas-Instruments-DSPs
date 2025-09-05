#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "Peripheral_Setup.h"

void Setup_GPIO(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;  // Habilita o clock para o módulo GPIO

    // Configuração GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;  // GPIO0 -> ePWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;  // GPIO1 -> ePWM1B
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;  // GPIO2 -> ePWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;  // GPIO3 -> ePWM2B
    EDIS;
}

void Setup_ePWM(void)
{
    #define SYS_CLK 150e6
    #define Fs 100e3

    EALLOW;

    //------------------------------ ePWM1 ------------------------------
    EPwm1Regs.TBPRD = SYS_CLK/(2*Fs);                   // Período = 1000 ciclos de clock
    EPwm1Regs.TBPHS.half.TBPHS = 0;                     // Fase inicial = 0
    EPwm1Regs.TBCTR = 0;                                // Contador inicia em 0

    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;      // Portadora triangular (updown)
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;             // Desabilita o carregamento de fase
    EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;              // Habilita o carregamento shadow do período
    EPwm1Regs.TBCTL.bit.SYNCOSEL = 2;                   // ePWM1 gera um sinal de sincronização
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;            // Divisor de clock = 1
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;               // Divisor de clock = 1

    // DEAD-BAND para controle de inversores
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;      // Enable Dead-band module
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;           // Active Hi complementary
    // Configura o valor de dead-band (RED e FED)
    EPwm1Regs.DBRED = 10;                               // RED = 10? TBCLKs
    EPwm1Regs.DBFED = 10;                               // FED = 10? TBCLKs


    // Configura o duty cycle (50%)
    EPwm1Regs.CMPA.half.CMPA = EPwm1Regs.TBPRD*0.5;     // Comparador A = 500 (50% de 1000)

    // Configura a ação do PWM no comparador A
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;                // Seta o pino ePWM1A quando TBCTR = CMPA (crescente)
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;                  // Limpa o pino ePWM1A quando TBCTR = CMPA (decrescente)



    //------------------------------ ePWM2 ------------------------------
    EPwm2Regs.TBPRD = EPwm1Regs.TBPRD;                  // EPWM2 usa mesma frequência de amostragem do EPWM1
    EPwm2Regs.TBPHS.half.TBPHS = 2*EPwm1Regs.TBPRD/3;   // Cria delay de fase de 120º (2*180/3);
    EPwm2Regs.TBCTR = 0;

    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;              // Habilita carregamento de fase para sincronizar com ePWM1
    EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;          // Sincroniza com ePWM1
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    // DEAD-BAND
    EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    // Configura o valor de dead-band (RED e FED)
    EPwm2Regs.DBRED = 10;
    EPwm2Regs.DBFED = 10;


    // Configura o duty cycle (50%)
    EPwm2Regs.CMPA.half.CMPA = EPwm1Regs.TBPRD*0.5;

    // Configura a ação do PWM no comparador A
    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;


    EDIS;
}
