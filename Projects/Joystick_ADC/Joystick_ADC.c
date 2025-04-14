#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

// Tamanho do display
#define ssd1306_height 32
#define ssd1306_width 128

// Definições dos pinos
#define VRX_PIN 26
#define VRY_PIN 27
#define ADC_CHANNEL_X 1
#define ADC_CHANNEL_Y 0

#define I2C_SDA 14
#define I2C_SCL 15
#define DISPLAY_I2C i2c1
#define DISPLAY_I2C_SPEED 400000

void display_init() {
    i2c_init(DISPLAY_I2C, DISPLAY_I2C_SPEED);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();
}

void show_joystick_on_display(uint16_t x, uint16_t y) {
    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&frame_area);

    uint8_t buffer[ssd1306_buffer_length];
    memset(buffer, 0, ssd1306_buffer_length);

    char msg_x[20], msg_y[20];
    sprintf(msg_x, "X:%4d", x);
    sprintf(msg_y, "Y:%4d", y);

    // Ajuste de posição vertical (deixe espaço entre as linhas)
    ssd1306_draw_string(buffer, 0, 0, msg_x);   // linha superior
    ssd1306_draw_string(buffer, 64, 0, msg_y);  // mesma linha, mais à direita

    render_on_display(buffer, &frame_area);
}

void setup_joystick() {
    adc_init();
    adc_gpio_init(VRX_PIN);
    adc_gpio_init(VRY_PIN);
}

void read_joystick(uint16_t *x, uint16_t *y) {
    adc_select_input(ADC_CHANNEL_X);
    sleep_us(2);
    *x = adc_read();

    adc_select_input(ADC_CHANNEL_Y);
    sleep_us(2);
    *y = adc_read();
}

int main() {
    stdio_init_all();
    setup_joystick();
    display_init();

    uint16_t x_val, y_val;

    while (true) {
        read_joystick(&x_val, &y_val);

        // Exibe no terminal
        printf("Joystick X: %d\tY: %d\n", x_val, y_val);

        // Exibe no display
        show_joystick_on_display(x_val, y_val);

        sleep_ms(300);
    }

    return 0;
}
/*
Perguntas e Respostas com base na observação experimental
Quando o joystick está no centro:
Valor de X = 2040, Y = 1996
Se você empurra 
para cima: Y = 4090
Para baixo: Y =  25
Para a direita: X =4090 
Para a esquerda: X = 25
*/