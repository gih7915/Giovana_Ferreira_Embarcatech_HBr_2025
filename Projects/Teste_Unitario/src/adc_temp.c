#include "temp_sensor.h"

float adc_to_celsius(uint16_t adc_val) {
    // Conversão de valor ADC (12 bits, 0-4095) para tensão
    float voltage = (adc_val * 3.3f) / 4095.0f;

    // Conversão da tensão para temperatura em °C
    float temperature = 27.0f - ((voltage - 0.706f) / 0.001721f);

    return temperature;
}
