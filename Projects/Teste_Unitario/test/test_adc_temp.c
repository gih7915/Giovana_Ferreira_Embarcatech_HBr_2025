#include "unity.h"
#include "temp_sensor.h"

void setUp(void) {}
void tearDown(void) {}

void test_adc_to_celsius_known_value(void) {
    uint16_t adc_val = (uint16_t)(0.706f * 4095.0f / 3.3f);  // valor para 0.706V
    float temp = adc_to_celsius(adc_val);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 27.0f, temp); // margem de 0.5°C
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_adc_to_celsius_known_value);
    return UNITY_END();
}
