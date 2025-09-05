/************************************************************************************************************
 * Autor: Gabriel Vinicius Crippa
 * Aplicação: Controle do conversor CC-CC bidirecional em ambiente HIL
 * Plataforma: Texas Instruments F28379D integrado ao Typhoon HIL 402
 * Data: 2025
 * Observação: Este código contém dois controles cascata (Buck e Boost). Usar seção "SELECIONE O CONVERSOR".
 ************************************************************************************************************/


#include "Peripheral_Setup.h"
#include "math.h"

// ----------------------------------------- Fatores de correção ----------------------------------------- //
#define Qadc __divf32(3.3,4096)

#define AO2 0.925199986   // i1 PIN23
#define AO3 0.9275        // Vg PIN24
#define AO4 0.925999999   // Vc2 PIN25
#define AO5 0.928         // i2 PIN26
//#define AO7 0.922       // Ibat PIN27
//#define AO8 0.930899978 // Nao usada PIN28
//#define AO9 0.927       //Nao usada PIN29



// ----------------------------------------- Parâmetros Globais ----------------------------------------- //
#define Ts 10e-6
#define TBPRD 500
#define dMax 1.0
#define dMin 0.0

// Variáveis
double Vo = 0;
double iL = 0;
double d = 0.0;

bool controle = false;

static double ev[2];            // ev[0]: erro atual, ev[1]: erro anterior
static double ei[2];
static double iLref[2];         // saída de controle da malha externa
static double u[2];             // saída de controle da malha interna
static double k0v, k1v, k0i, k1i;



// ======================================= SELECIONE O CONVERSOR ======================================= //
// Comentar uma das duas configurações abaixo
#define CONVERSOR_BUCK
//#define CONVERSOR_BOOST


// ---------- Validação de seleção ----------
#if defined(CONVERSOR_BUCK) && defined(CONVERSOR_BOOST)
    #error "Selecione apenas UM conversor: BUCK ou BOOST"
#elif !defined(CONVERSOR_BUCK) && !defined(CONVERSOR_BOOST)
    #error "Nenhum conversor selecionado! Defina CONVERSOR_BUCK ou CONVERSOR_BOOST"
#endif


#ifdef CONVERSOR_BUCK
    // ========= Conversor Buck ===========
    #define Vref 12.0
    #define kp_v 0.26618
    #define ki_v 100
    #define kp_i 4
    #define ki_i 20000
    #define kaw 1.0
    // Scaling Buck para Typhoon HIL
    #define K_scaling_Vo __divf32(15,3.03)
    #define B_scaling_Vo 0.0
    #define K_scaling_iL __divf32(5,3.03)
    #define B_scaling_iL 0.0
#endif

#ifdef CONVERSOR_BOOST
    // ========= Conversor Boost ===========
    #define Vref 24.0
    #define kp_v 0.06
    #define ki_v 100
    #define kp_i 3
    #define ki_i 20000
    #define kaw 1.0
    // Scaling Boost para Typhoon HIL
    #define K_scaling_Vo __divf32(30,3.03)
    #define B_scaling_Vo 0.0
    #define K_scaling_iL __divf32(5,3.03)
    #define B_scaling_iL 0.0
#endif


__interrupt void isr_adc(void);                     // Declare ADC interrupt


int main(void)
{
    InitSysCtrl();                                  // Initialize System Control:
    DINT;                                           // Disable CPU interrupts
    InitPieCtrl();                                  // Initialize the PIE control registers to their default state
    IER = 0x0000;                                   // Disable CPU interrupts
    IFR = 0x0000;                                   // Clear all CPU interrupt flags:
    InitPieVectTable();                             // Initialize the PIE vector table

    Setup_GPIO();                                   // Set the GPIO
    Setup_ePWM();                                   // Set the ePWM
    Setup_ADC();                                    // Set the ADC

    EALLOW;
    PieVectTable.ADCA1_INT = &isr_adc;              // Redirect isr_adc function address to ADCA1_INT interrupt
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;              // Set IFR for INT1 in PIE group 1 -> row 1, column 1 (ADCA1)
    EDIS;
    IER |= M_INT1;                                  // Enable Interrupt group 1 (ADCA1)
    EINT;                                           // Enable Global interrupt INTM
    ERTM;                                           // Enable Global realtime interrupt DBGM

    // --------------------------------------------- INICIALIZAÇÃO --------------------------------------------- //
    // Coeficientes do controlador
    k0v = kp_v + (ki_v * Ts / 2);
    k1v = (ki_v * Ts / 2) - kp_v;

    k0i = kp_i + (ki_i * Ts / 2);
    k1i = (ki_i * Ts / 2) - kp_i;

    // Variáveis
    ev[0] = ev[1] = 0.0;
    ei[0] = ei[1] = 0.0;
    iLref[0] = iLref[1] = 0.0;
    u[0] = u[1] = 0.0;

}

__interrupt void isr_adc(void)
{
    // Entradas
    Vo = ((float) AdcbResultRegs.ADCRESULT0) * Qadc * AO4 * 1.0145; //1.0145 é outro fator de correção
    Vo = Vo * K_scaling_Vo - B_scaling_Vo;

    iL = ((float) AdccResultRegs.ADCRESULT0) * Qadc * AO3;
    iL = iL * K_scaling_iL - B_scaling_iL;

    controle = GpioDataRegs.GPDDAT.bit.GPIO123;                     // Verifica se o controle será ativado
    if (controle){
        // ---------- MALHA EXTERNA DE TENSÃO ----------
        // 1. Erro de tensão atual
        ev[0] = Vref - Vo;

        // 2. Saída do PI da malha externa é a referência da malha interna
        iLref[0] = k0v * ev[0] + k1v * ev[1] + iLref[1];

        // 3. Limitação de corrente para proteção do hardware
        if (iLref[0] > 5.0) iLref[0] = 5.0;

        // 4. Atualização de variáveis da malha externa
        ev[1] = ev[0];
        iLref[1] = iLref[0];


        // ---------- MALHA INTERNA DE CORRENTE COM ANTI-WINDUP ----------
        // 1. Erro de corrente atual
        ei[0] = iLref[0] - iL;

        // 2. Saída ideal do PI (antes da saturação e sem anti-windup)
        double u_ideal = (k0i * ei[0] + k1i * ei[1]) + u[1];

        // 3. Saída real do PI (saturada)
        double usat = u_ideal;
        if (usat > dMax) {
            usat = dMax;
        } else if (usat < dMin) {
            usat = dMin;
        }

        // 4. Erro de saturação
        double erro_saturacao = u_ideal - usat;

        // 5. Estado do integrador para o próximo passo de tempo (com anti-windup)
        u[1] = u_ideal - (kaw * erro_saturacao);

        // 6. Atualização de variáveis da malha interna
        ei[1] = ei[0];

        // 7. Saída do bloco C
        d = usat*TBPRD;
        EPwm1Regs.CMPA.bit.CMPA = d;

        // Teste MA (verificar se pelo SCADA consigo ativar e desativar o controlador, PWM2 não está endereçado a nada)
        EPwm2Regs.CMPA.bit.CMPA = 0.5*TBPRD;
    } else{
        EPwm1Regs.CMPA.bit.CMPA = 0.0;
        EPwm2Regs.CMPA.bit.CMPA = 0.0; // Teste MA

        // Reinicializa os estados dos controladores para a próxima partida
        ev[0] = ev[1] = 0.0;
        ei[0] = ei[1] = 0.0;
        iLref[0] = iLref[1] = 0.0;
        u[0] = u[1] = 0.0;
    }

    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          // Clear INT1 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
