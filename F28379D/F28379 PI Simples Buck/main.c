#include "Peripheral_Setup.h"
#include "math.h"

// ----------------------------------------- Fatores de correção ----------------------------------------- //

#define AO2 0.925199986   // i1 PIN23
#define AO3 0.9275        // Vg PIN24
#define AO4 0.925999999   // Vc2 PIN25
#define AO5 0.928         // i2 PIN26

//#define AO7 0.922       // Ibat PIN27
//#define AO8 0.930899978 // Nao usada PIN28
//#define AO9 0.927       //Nao usada PIN29



// ----------------------------------------- Ganhos de conversão ----------------------------------------- //
#define Qadc __divf32(3.3, 4096)
#define K_scaling_Vbuck __divf32(15, 3.03)
#define B_scaling_Vbuck 0


// Controle
#define b2 0.2306
#define b1 -0.4582
#define b0 0.2277
#define a1 -1.9048
#define a0 0.9048

// Variáveis de controle
#define Vref 12.0
#define dMin 0.1
#define dMax 0.9
#define Ts 10e-6
#define TBPRD 2500

double Vbuck = 0;
double d = 0.5;
double e[3] = {0,0,0}, u[3] = {0,0,0};
bool controle = false;



__interrupt void isr_adc(void);                     // Declare ADC interrupt

int main(void)
{
    InitSysCtrl();                                 // Initialize System Control:
    DINT;
    // Disable CPU interrupts
    InitPieCtrl(); // Initialize the PIE control registers to their default state
    IER = 0x0000;                                   // Disable CPU interrupts
    IFR = 0x0000;                              // Clear all CPU interrupt flags:
    InitPieVectTable();                       // Initialize the PIE vector table

    Setup_GPIO();                                   // Set the GPIO
    Setup_ePWM();                                   // Set the ePWM
    Setup_ADC();                                    // Set the ADC

    EALLOW;
    PieVectTable.ADCA1_INT = &isr_adc; // Redirect isr_adc function address to ADCA1_INT interrupt
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1; // Set IFR for INT1 in PIE group 1 -> row 1, column 1 (ADCA1)
    EDIS;
    IER |= M_INT1;                           // Enable Interrupt group 1 (ADCA1)

    EINT;
    // Enable Global interrupt INTM
    ERTM;
    // Enable Global realtime interrupt DBGM

    // --------------------------------------------- INICIALIZAÇÃO --------------------------------------------- //


}

__interrupt void isr_adc(void)
{
    // INPUTS
    Vbuck = ((float) AdcbResultRegs.ADCRESULT0) * Qadc * AO4;
    Vbuck = Vbuck * K_scaling_Vbuck - B_scaling_Vbuck;

    controle = GpioDataRegs.GPDDAT.bit.GPIO123; // Verifica se o controle será ativado
    if (controle){
        e[0] = Vref - Vbuck;
        u[0] = b2*e[0] + b1*e[1] + b0*e[2] - a1*u[1] - a0*u[2];

        // SATURATION
        d = u[0]*TBPRD;
        //if (d >= TBPRD) d = TBPRD;
        //if (d <= 0) d = 0;

        EPwm1Regs.CMPA.bit.CMPA = d;
    } else{
        EPwm1Regs.CMPA.bit.CMPA = 0;
    }

    e[2] = e[1];
    e[1] = e[0];
    u[2] = u[1];
    u[1] = u[0];



    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // Clear INT1 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
