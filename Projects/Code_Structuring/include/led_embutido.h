#ifndef LED_EMBUTIDO_H

#define LED_EMBUTIDO_H

#include <stdbool.h>

bool led_embutido_init(void);
void led_embutido_set(bool ligar);
bool led_embutido_get(void);

#endif
