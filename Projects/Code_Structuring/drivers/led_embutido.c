#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

bool led_embutido_init(void) 
{
    if(cyw43_arch_init()) 
    {
        return false;
    }
    return true;
} 

void led_embutido_set(bool estado) 
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, estado);
}

bool led_embutido_get(void) 
{
    return cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN);
}
