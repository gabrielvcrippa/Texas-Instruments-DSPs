#include "DSP28x_Project.h"
#include "DSP2833x_Device.h"
#include "Peripheral_Setup.h"


void Setup_GPIO(void) {
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1; // GPIO0 -> ePWM1A
    EDIS;
}

void Setup_ePWM(void) {
    EALLOW;
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;

    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Modo de contagem
    EPwm1Regs.TBPRD = 750; // Período do SOC em 20kHz
    EPwm1Regs.ETSEL.bit.SOCAEN = 1; // Habilita SOCA
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO; // Trigger no ZERO
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST; // Gera SOC no primeiro evento
    EDIS;
}

void Setup_ADC(void) {
    InitAdc();

    EALLOW;
    AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x3; // HSPCLK/4 = 25 MHz -> Clock do ADC
    AdcRegs.ADCTRL1.bit.ACQ_PS = 0xF;   // Janela de aquisição = 24 ADCCLK

    // Configura 3 conversões (CONV00-CONV02)
    AdcRegs.ADCMAXCONV.all = 0x0002; // Usaremos 3 conversões, do CONV00 ao CONV02, uma para cada ADC

    // Mapeia os canais (A0, A1, A2)
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0; // ADCINA0
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1; // ADCINA1
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x2; // ADCINA2

    // Habilita trigger do ePWM e interrupção
    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;
    EDIS;
}
