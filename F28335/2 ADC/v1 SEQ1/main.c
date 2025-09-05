#include "DSP28x_Project.h"
#include "DSP2833x_Device.h"
#include "Peripheral_Setup.h"

volatile double adc0, adc1, adc2; // Leitura dos ADCs

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
    adc0 = (AdcRegs.ADCRESULT0 >> 4) * 3.3 / 4096.0;  // ADCA0
    adc1 = (AdcRegs.ADCRESULT1 >> 4) * 3.3 / 4096.0;  // ADCA1
    adc2 = (AdcRegs.ADCRESULT2 >> 4) * 3.3 / 4096.0;  // ADCA2

    // Reset e acknowledge
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
