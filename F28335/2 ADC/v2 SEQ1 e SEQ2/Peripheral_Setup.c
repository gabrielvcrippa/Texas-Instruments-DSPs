// Todos os adcs estão inseridos, mas apenas SEQ1 e SEQ2 funcionando, falta implementar SEQ3 e SEQ4


#include "DSP28x_Project.h"
#include "DSP2833x_Device.h"
#include "Peripheral_Setup.h"


void Setup_GPIO(void) {
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1; // GPIO0 -> ePWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1; // GPIO1 -> ePWM1B
    EDIS;
}

void Setup_ePWM(void) {
    EALLOW;
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1; // Habilita o clock do ePWM1

    // Configuração do Time Base (comum para ePWM1A e ePWM1B)
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Modo de contagem
    EPwm1Regs.TBPRD = 750; // Período do SOC em 20kHz

    // Configuração do SOCA (para SEQ1)
    EPwm1Regs.ETSEL.bit.SOCAEN = 1; // Habilita SOCA
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO; // Trigger no ZERO
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST; // Gera SOC no primeiro evento

    // Configuração do SOCB (para SEQ2)
    EPwm1Regs.ETSEL.bit.SOCBEN = 1; // Habilita SOCB
    EPwm1Regs.ETSEL.bit.SOCBSEL = ET_CTR_ZERO; // Trigger no ZERO
    EPwm1Regs.ETPS.bit.SOCBPRD = ET_1ST; // Gera SOC no primeiro evento

    EDIS;
}

void Setup_ADC(void) {
    InitAdc(); // Inicializa o ADC

    EALLOW;
    // Configuração do clock do ADC (25 MHz)
    AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x3; // HSPCLK/4 = 25 MHz
    AdcRegs.ADCTRL1.bit.ACQ_PS = 0xF;   // Janela de aquisição = 24 ADCCLK

    // Configura SEQ1 para 16 conversões (CONV00-CONV15)
    AdcRegs.ADCMAXCONV.all = 0x000F; // 16 conversões (0xF = 16-1)

    // Mapeia os canais do SEQ1 (ADCINA0-ADCINA3)
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0; // ADCINA0
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1; // ADCINA1
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x2; // ADCINA2
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x3; // ADCINA3

    // Mapeia os canais do SEQ2 (ADCINA4-ADCINA7)
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 0x4; // ADCINA4
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 0x5; // ADCINA5
    AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 0x6; // ADCINA6
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 0x7; // ADCINA7

    // Mapeia os canais do SEQ3 (ADCINB0-ADCINB3)
    AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 0x8; // ADCINB0
    AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 0x9; // ADCINB1
    AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 0xA; // ADCINB2
    AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 0xB; // ADCINB3

    // Mapeia os canais do SEQ4 (ADCINB4-ADCINB7)
    AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 0xC; // ADCINB4
    AdcRegs.ADCCHSELSEQ4.bit.CONV13 = 0xD; // ADCINB5
    AdcRegs.ADCCHSELSEQ4.bit.CONV14 = 0xE; // ADCINB6
    AdcRegs.ADCCHSELSEQ4.bit.CONV15 = 0xF; // ADCINB7

    // Habilita trigger do ePWM para SEQ1 e SEQ2
    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1; // Habilita SOCA para SEQ1
    AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 = 1; // Habilita SOCB para SEQ2

    // Habilita interrupções para SEQ1 e SEQ2
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1; // Habilita interrupção do SEQ1
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 1; // Habilita interrupção do SEQ2
    EDIS;
}
