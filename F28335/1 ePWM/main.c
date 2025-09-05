#include "Peripheral_Setup.h"

int main(void)
{
    InitSysCtrl();  // Inicializa o sistema
    Setup_GPIO();   // Inicializa o GPIO
    Setup_ePWM();   // Inicializa os m�dulos ePWM



    while(1)        // Loop infinito
    {
        // O PWM � gerado automaticamente pelo hardware
        // Nenhuma a��o adicional � necess�ria aqui
    }
}
