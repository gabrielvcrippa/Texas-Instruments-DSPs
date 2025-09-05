#include "DSP28x_Project.h"
#include "DSP2833x_Device.h"
#include "Peripheral_Setup.h"

volatile double adc0, adc1, adc2, adc3, adc4, adc5, adc6, adc7, adc8, adc9, adc10, adc11, adc12, adc13, adc14, adc15; // Leitura dos ADCs

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
    adc3 = (AdcRegs.ADCRESULT3 >> 4) * 3.3 / 4096.0; // ADCINA3
    adc4 = (AdcRegs.ADCRESULT4 >> 4) * 3.3 / 4096.0; // ADCINA4
    adc5 = (AdcRegs.ADCRESULT5 >> 4) * 3.3 / 4096.0; // ADCINA5
    adc6 = (AdcRegs.ADCRESULT6 >> 4) * 3.3 / 4096.0; // ADCINA6
    adc7 = (AdcRegs.ADCRESULT7 >> 4) * 3.3 / 4096.0; // ADCINA7
    adc8 = (AdcRegs.ADCRESULT8 >> 4) * 3.3 / 4096.0; // ADCINB0
    adc9 = (AdcRegs.ADCRESULT9 >> 4) * 3.3 / 4096.0; // ADCINB0
    adc10 = (AdcRegs.ADCRESULT10 >> 4) * 3.3 / 4096.0; // ADCINB0
    adc11 = (AdcRegs.ADCRESULT11 >> 4) * 3.3 / 4096.0; // ADCINB0
    adc12 = (AdcRegs.ADCRESULT12 >> 4) * 3.3 / 4096.0; // ADCINB0
    adc13 = (AdcRegs.ADCRESULT13 >> 4) * 3.3 / 4096.0; // ADCINB0
    adc14 = (AdcRegs.ADCRESULT14 >> 4) * 3.3 / 4096.0; // ADCINB0
    adc15 = (AdcRegs.ADCRESULT15 >> 4) * 3.3 / 4096.0; // ADCINB0

    // Reset e acknowledge
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
    AdcRegs.ADCTRL2.bit.RST_SEQ2 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
