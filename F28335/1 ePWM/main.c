#include "Peripheral_Setup.h"

int main(void)
{
    InitSysCtrl();  // Inicializa o sistema
    Setup_GPIO();   // Inicializa o GPIO
    Setup_ePWM();   // Inicializa os módulos ePWM



    while(1)        // Loop infinito
    {
        // O PWM é gerado automaticamente pelo hardware
        // Nenhuma ação adicional é necessária aqui
    }
}
