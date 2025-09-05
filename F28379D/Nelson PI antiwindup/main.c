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

unsigned short int i = 0;
bool modo, aquisicao;
double tempo;
#define tIni __divf32(-1, 60)
double plot[333];

// ----------------------------------------- Ganhos de conversão ----------------------------------------- //

#define Qadc __divf32(3.3, 4096)                    // Gain of ADC Conversion [0 ~ 4095 (2^12-1) to 0 ~ 3 V]
#define K_scaling_Vref __divf32(500, 3.02)
#define K_scaling_Vc2 __divf32(500, 3.02)
#define K_scaling_i1 __divf32(360, 3.03)
#define K_scaling_vg __divf32(680, 3.03)
#define K_scaling_i2 __divf32(50, 3.03)

#define B_scaling_Vc2 0*50
#define B_scaling_i1 5*36
#define B_scaling_Vg 5*68
#define B_scaling_i2 5*5
#define B_scaling_ibat 5*24

// Gerais
#define Ts 50e-6
#define TBPRD 2500
#define uIni 0.5112


// ----------------------------------------- PI ----------------------------------------- //

// Parâmetros
#define kpi 0.0103
#define kii 23.8
#define kpv 4.62
#define kiv 102
#define NF_fc 120
#define NF_csi 0.1

double NF_wc, NF_a0, NF_x, NF_y;
double NF_a1, NF_a2, NF_b0, NF_b1, NF_b2;
double kv0, kv1, ki0, ki1;

// Sinais
double Vref = 400, Vc2Filt[3] = { 0, 0, 0 }, Vc2[3] = { 0, 0, 0 }, ev[2] = { 0, 0 };
double Iref[2] = { 0, 0 }, deltaIref, i1, ei[2] = { 0, 0 }, u1[2] = { 0, 0 }, d1, Sgn;
double phi_Ipll = 0;

//modificações
double  Iref_sat[2] = {0, 0};
double a0, a1, b0, b1, c;

#define Irefmax 20
#define Irefmin -20

// Pedro && Gabriel == s2

double eu[2] = {0,0}, Irefu[2] = { 0, 0 };
double k0pg, k1pg, kawpg;


// ----------------------------------------- PLL ----------------------------------------- //

// Parâmetros
#define fg 60
#define kosg __sqrt(2)
#define PLL_kp 40
#define PLL_ki 800
#define Vrms 220

bool Pflow;
double P = 3e3, wg, OSG_b_alpha, OSG_b_beta, OSG_a1, OSG_a2, PLL_ke0, PLL_ke1, Vp, Ip;

// Sinais
double vg[3] = { 0, 0, 0 }, alpha[3] = { 0, 0, 0 }, beta[3] = { 0, 0, 0 };
double vq, theta[2] = { 0, 0 }, e[2] = { 0, 0 }, w[2] = { 0, 0 }, Ipll;

// ----------------------------------------- R ----------------------------------------- //

// Parâmetros
#define kr 0.1
#define R_csi 0.1

double R_ar0, R_x, R_y;
double R_a, R_b0, R_b1, R_b2;

// Sinais
double i2, R_e[3] = { 0, 0, 0 }, u2[3] = { 0, 0, 0 }, PR_d[2];

__interrupt void isr_adc(void);                     // Declare ADC interrupt

int main(void){
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

    // --------------------------------------------- PI --------------------------------------------- //

    NF_wc = 2 * M_PI * NF_fc;
    NF_a0 = 4 + NF_wc * Ts * (4 * NF_csi + NF_wc * Ts);
    NF_x = __divf32(2 * (pow(NF_wc * Ts, 2) - 4), NF_a0);
    NF_y = __divf32((4 + pow(NF_wc * Ts, 2)), NF_a0);
    NF_a1 = NF_x;
    NF_a2 = 1 - __divf32(8 * NF_wc * Ts * NF_csi, NF_a0);
    NF_b0 = NF_y;
    NF_b1 = NF_x;
    NF_b2 = NF_y;

    kv0 = kpv + 0.5 * kiv * Ts;
    kv1 = 0.5 * kiv * Ts - kpv;
    ki0 = kpi + 0.5 * kii * Ts;
    ki1 = 0.5 * kii * Ts - kpi;


//    a0 = kiv*Ts + 2;
//    a1 = __divf32((kiv*Ts - 2),a0);
//    b0 = __divf32((kiv*Ts + 2*kpv),a0);
//    b1 = __divf32((kiv*Ts - 2*kpv),a0);
//    c = __divf32((kiv*Ts),a0);

    // --------------------------------------------- PLL --------------------------------------------- //

    wg = 2 * M_PI * fg;
    OSG_b_alpha = __divf32(2 * kosg * wg * Ts,
                           (4 + 2 * kosg * wg * Ts + pow(wg * Ts, 2)));
    OSG_b_beta = __divf32(kosg * pow(wg * Ts, 2),
                          (4 + 2 * kosg * wg * Ts + pow(wg * Ts, 2)));
    OSG_a1 = __divf32((2 * pow(wg * Ts, 2) - 8),
                      (4 + 2 * kosg * wg * Ts + pow(wg * Ts, 2)));
    OSG_a2 = __divf32((4 - 2 * kosg * wg * Ts + pow(wg * Ts, 2)),
                      (4 + 2 * kosg * wg * Ts + pow(wg * Ts, 2)));
    PLL_ke0 = PLL_kp + 0.5 * PLL_ki * Ts;
    PLL_ke1 = 0.5 * PLL_ki * Ts - PLL_kp;

    Vp = __sqrt(2) * Vrms;
    Ip = __divf32(2 * P, Vp);

    // --------------------------------------------- R --------------------------------------------- //

    wg = 2 * M_PI * fg;
    R_ar0 = 4 + pow(wg * Ts, 2);
    R_x = __divf32(4 * R_csi * wg * Ts, R_ar0);
    R_y = 2 - __divf32(16, R_ar0);
    R_a = R_y;
    R_b0 = 1 + R_x;
    R_b1 = R_y;
    R_b2 = 1 - R_x;

    // ------------------------------------------- DAQ -------------------------------------------- //
    tempo = tIni*2;

    // Pedro y Gabryell

    k0pg = kpv+0.5*kiv*Ts;
    k1pg = 0.5*(kiv*Ts) - kpv;
    kawpg = kiv*Ts*0.5;
}

__interrupt void isr_adc(void)
{

    // Manual
    if (modo == 0)
    {
        u1[0] = uIni;
        d1 = u1[0] * TBPRD;
        if (d1 >= TBPRD)
            d1 = TBPRD;
        if (d1 <= 0)
            d1 = 0;

        EPwm1Regs.CMPA.bit.CMPA = d1;    // Sending the duty cycle to comparator
        EPwm2Regs.CMPA.bit.CMPA = d1;    // Sending the duty cycle to comparator
        EPwm3Regs.CMPA.bit.CMPA = d1;    // Sending the duty cycle to comparator

        // Inicializacao das variaveis
        Vc2[0] = ((float) AdcbResultRegs.ADCRESULT0) * Qadc * AO4;
        Vc2[0] = Vc2[0] * K_scaling_Vc2 - B_scaling_Vc2;

        i1 = ((float) AdcaResultRegs.ADCRESULT0) * Qadc * AO2;
        i1 = K_scaling_i1 * i1 - B_scaling_i1;

        vg[0] = ((float) AdccResultRegs.ADCRESULT0) * Qadc * AO3;
        vg[0] = K_scaling_vg * vg[0] - B_scaling_Vg;

        i2 = ((float) AdcaResultRegs.ADCRESULT1) * Qadc * AO5;
        i2 = K_scaling_i2 * i2 - B_scaling_i2;

        Vc2[2] = Vc2[1];
        Vc2[1] = Vc2[0];
        u1[1] = u1[0];

        Iref[0] = i1;
        Iref[1] = Iref[0];
        modo = GpioDataRegs.GPDDAT.bit.GPIO123;

    }

    // Automatico
    if (modo == 1)
    {

        // --------------------------------------------- PI --------------------------------------------- //

        // Read the ADC result for the ADCINB3 channel (Pin 25 | Typhoon AO4)
        Vc2[0] = ((float) AdcbResultRegs.ADCRESULT0) * Qadc * AO4;
        Vc2[0] = Vc2[0] * K_scaling_Vc2 - B_scaling_Vc2;

        // Read the ADC result for the ADCIN14 channel (Pin 23 | Typhoon AO2)
        i1 = ((float) AdcaResultRegs.ADCRESULT0) * Qadc * AO2;
        i1 = K_scaling_i1 * i1 - B_scaling_i1;

        Vc2Filt[0] = NF_b0 * Vc2[0] + NF_b1 * Vc2[1] + NF_b2 * Vc2[2] - NF_a1 * Vc2Filt[1] - NF_a2 * Vc2Filt[2];
//        Vc2Filt[0] = Vc2[0];
        ev[0] = Vref - Vc2Filt[0];

        //Pedro e Gabriel s2

//        if (i1 > 180) i1 = 180;
//        if (i1 < -180) i1 = -180;

        Irefu[0] = k0pg*ev[0] + k1pg*ev[1] + kawpg*(eu[0]+eu[1]) + Irefu[1];
        if (Irefu[0] >= Irefmax) Iref[0] = Irefmax;
        else if (Irefu[0] <= Irefmin ) Iref[0] = Irefmin;
        else Iref[0] = Irefu[0];
        eu[0] = Iref[0] - Irefu[0];


//        Iref[0] = kv0 * ev[0] + kv1 * ev[1] + Iref[1];
//        if (Iref[0] >= Irefmax || Iref[0] <= Irefmin)
//            deltaIref = 0;
//        else
//            deltaIref = kv0 * ev[0] + kv1 * ev[1];
//        Iref[0] = deltaIref + Iref[1];


        // Enviando sinal Iref
        Sgn = __divf32(TBPRD,70)*(Iref[0] + 35);

        if (Sgn > TBPRD)
            Sgn = TBPRD;
        if (Sgn < 0)
            Sgn = 0;

        EPwm6Regs.CMPA.bit.CMPA = Sgn;

        ei[0] = i1 - Iref[0];
        u1[0] = ki0 * ei[0] + ki1 * ei[1] + u1[1];

        Vc2[2] = Vc2[1];
        Vc2[1] = Vc2[0];
        Vc2Filt[2] = Vc2Filt[1];
        Vc2Filt[1] = Vc2Filt[0];
        ev[1] = ev[0];
        Iref[1] = Iref[0];

        //Pedro e Gaybriel
        eu[1] = eu[0];
        Irefu[1] = Irefu[0];

//        Iref_sat[1] = Iref_sat[0]; // alteração

        ei[1] = ei[0];
        u1[1] = u1[0];

        d1 = u1[0] * TBPRD;

        if (d1 >= TBPRD)
            d1 = TBPRD;
        if (d1 <= 0)
            d1 = 0;

        EPwm1Regs.CMPA.bit.CMPA = d1;    // Sending the duty cycle to comparator
        EPwm2Regs.CMPA.bit.CMPA = d1;    // Sending the duty cycle to comparator
        EPwm3Regs.CMPA.bit.CMPA = d1;    // Sending the duty cycle to comparator





        // --------------------------------------------- PLL --------------------------------------------- //

        // Read the ADC result for the ADCINC3 channel (Pin 24 | Typhoon AO3)
        vg[0] = ((float) AdccResultRegs.ADCRESULT0) * Qadc * AO3;
        vg[0] = K_scaling_vg * vg[0] - B_scaling_Vg;

        alpha[0] = OSG_b_alpha * (vg[0] - vg[2])
                - (OSG_a1 * alpha[1] + OSG_a2 * alpha[2]);
        beta[0] = OSG_b_beta * (vg[0] + 2 * vg[1] + vg[2])
                - (OSG_a1 * beta[1] + OSG_a2 * beta[2]);

        alpha[2] = alpha[1];
        alpha[1] = alpha[0];

        beta[2] = beta[1];
        beta[1] = beta[0];

        vq = __cos(theta[0]) * beta[0] - __sin(theta[0]) * alpha[0];

        e[0] = 0 - vq;

        w[0] = PLL_ke0 * e[0] + PLL_ke1 * e[1] + w[1];

        e[1] = e[0];

        w[1] = w[0];

        w[0] = w[0] + wg;

        theta[0] = Ts * w[0] + theta[1];

        if (theta[0] >= 2 * M_PI)
            theta[0] = 0;

        Ip = __divf32(2 * P, Vp);

        aquisicao = GpioDataRegs.GPBDAT.bit.GPIO58;

        if (aquisicao && theta[0] == 0)
        {
            tempo += __divf32(1,60);
            GpioDataRegs.GPCDAT.bit.GPIO66 = 1; // trigger para Capture
            if (tempo < 0.5) {Pflow = 1; P = 3000;}
            if (tempo >= (0.5 + tIni) && tempo < 1) Pflow = 0;
            if (tempo >= (1 + 2*tIni) && tempo < 1.5) Pflow = 1;
            if (tempo >= (1.5 + 2*tIni) && tempo < 2) P = 1500;
            if (tempo >= (2 + 2*tIni) && tempo < 2.5) P = 3000;

        }

        if (P == 1500)
            GpioDataRegs.GPADAT.bit.GPIO10 = 0;
        else
            GpioDataRegs.GPADAT.bit.GPIO10 = 1;

        if (!aquisicao)
        {
            Pflow = GpioDataRegs.GPBDAT.bit.GPIO61;
            tempo = tIni*2;
            GpioDataRegs.GPCDAT.bit.GPIO66 = 0; //reseta trigger
        }

        if (!Pflow)
        {
            phi_Ipll = 0;
            GpioDataRegs.GPADAT.bit.GPIO11 = 0;
        }
        else
        {
            phi_Ipll = M_PI;
            GpioDataRegs.GPADAT.bit.GPIO11 = 1;
        }

        Ipll = Ip * __cos(theta[0] + phi_Ipll);

        // i = (i == 332) ? 0 : (i+1);
        // plot[i] = Ipll;                              // plot Ipll in real-time

        vg[2] = vg[1];
        vg[1] = vg[0];

        theta[1] = theta[0];

        // --------------------------------------------- R --------------------------------------------- //

        // Read the ADC result for the ADCINA3 channel (Pin 26 | Typhoon AO5)
        i2 = ((float) AdcaResultRegs.ADCRESULT1) * Qadc * AO5;
        i2 = K_scaling_i2 * i2 - B_scaling_i2;

        R_e[0] = Ipll - i2;
        u2[0] = kr * (R_b0 * R_e[0] + R_b1 * R_e[1] + R_b2 * R_e[2]) - R_a * u2[1] - u2[2];

        PR_d[0] = 1 + u2[0];
        PR_d[0] *= 0.5 * TBPRD;
        PR_d[1] = 1 - u2[0];
        PR_d[1] *= 0.5 * TBPRD;

        if (PR_d[0] > TBPRD)
            PR_d[0] = TBPRD;
        if (PR_d[0] < 0)
            PR_d[0] = 0;

        if (PR_d[1] > TBPRD)
            PR_d[1] = TBPRD;
        if (PR_d[1] < 0)
            PR_d[1] = 0;

        R_e[2] = R_e[1];
        R_e[1] = R_e[0];

        u2[2] = u2[1];
        u2[1] = u2[0];

        EPwm4Regs.CMPA.bit.CMPA = PR_d[0];
        EPwm5Regs.CMPA.bit.CMPA = PR_d[1];

        modo = GpioDataRegs.GPDDAT.bit.GPIO123;
    }

    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // Clear INT1 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
