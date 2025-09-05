#include "DSP28x_Project.h"
#include "DSP2833x_Device.h"
#include "Peripheral_Setup.h"

volatile float iL, Vo;      // Leitura dos ADCs

__interrupt void adc_isr(void);

int main(void) {
    InitSysCtrl();
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    EALLOW;
    PieVectTable.ADCINT = &adc_isr;
    EDIS;

    Setup_GPIO();
    Setup_ePWM();
    Setup_ADC();

    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;
    IER |= M_INT1;
    EINT;
    ERTM;

    while(1);
}

__interrupt void adc_isr(void) {
    // 1. Leitura de iL e Vo pelos ADCs
    iL = (AdcRegs.ADCRESULT0 >> 4) / 4095.0 * 3.3;      // Shift (>>) dos 4 bits menos significativos
    Vo = (AdcRegs.ADCRESULT1 >> 4) / 4095.0 * 3.3;      // [XXXX][DDDD DDDD DDDD] (X = lixo, D = dado)


    // 2. Ajuste do duty cycle do ePWM1A
    EPwm1Regs.CMPA.half.CMPA = EPwm1Regs.TBPRD * 0.5;   // 50% duty cycle


    // 3. Reset e acknowledge dos ADC0-4 (do SEQ1)
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;                   // Pode causar perda de conversões????
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
