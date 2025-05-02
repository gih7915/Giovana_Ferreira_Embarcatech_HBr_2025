//Hal é a ponte entre o app e o hardware real
#include "led_embutido.h"

bool led_initiated = false;

void hal_led_toggle(void) 
{
    if(!led_initiated) 
    {
        led_initiated = led_embutido_init();
    }

    led_embutido_set(!led_embutido_get());
}
