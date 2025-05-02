#include "hal_led.h"
#include "pico/stdlib.h"

int main(void) 
{
    while (true) 
    {
        hal_led_toggle(); //Led acende
        sleep_ms(500); //Pausa de 0.5 segundos
    }
    return 0;
}
