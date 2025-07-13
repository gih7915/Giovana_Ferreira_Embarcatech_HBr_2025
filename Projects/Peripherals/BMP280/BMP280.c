#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "inc/bmp280.h"
#include "inc/ssd1306.h"

//Configurações do Sensor
#define BMP280_SDA_PIN 0 //i2c0
#define BMP280_SCL_PIN 1 //i2c0
#define BMP280_ADDR 0x76

//Configurações do Display
#define OLED_SDA_PIN 14  
#define OLED_SCL_PIN 15
#define OLED_ADDR 0x3C   

// Instâncias I2C separadas
i2c_inst_t *bmp280_i2c = i2c0;
i2c_inst_t *oled_i2c = i2c1;

void display_message(const char* line1, const char* line2) 
{
    uint8_t buffer[ssd1306_buffer_length];
    struct render_area area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&area);

    memset(buffer, 0, sizeof(buffer));

    // Posições fixas para melhor legibilidade
    ssd1306_draw_string(buffer, 0, 20, line1);
    ssd1306_draw_string(buffer, 0, 40, line2);

    render_on_display(buffer, &area);
}

void init_oled() {
    // Configura I2C para o display OLED
    i2c_init(oled_i2c, 400000);  // Higher speed for OLED
    gpio_set_function(OLED_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA_PIN);
    gpio_pull_up(OLED_SCL_PIN);
    
    // Inicializa o display
    ssd1306_init();
}

void init_bmp280() {
    // Configura I2C para o BMP280
    i2c_init(bmp280_i2c, 100000);  // Lower speed for sensor
    gpio_set_function(BMP280_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(BMP280_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(BMP280_SDA_PIN);
    gpio_pull_up(BMP280_SCL_PIN);
    
    // Inicializa o sensor
    bmp280_init();
}

int main() {
    stdio_init_all();
    sleep_ms(2000);  // Espera para inicialização do USB

    // Inicializa hardware
    init_oled();
    init_bmp280();

    while (true) {
        // Lê os valores do sensor
        sensors_t s = bmp280_get_all(BMP280_ADDR);
        
        // Prepara strings para o display
        char temp_str[20];
        char press_str[20];
        snprintf(temp_str, sizeof(temp_str), "Temp: %.1f C", s.temperature);
        snprintf(press_str, sizeof(press_str), "Press: %.1f hPa", s.pressure/100.0);
        
        // Exibe no display OLED
        display_message(temp_str, press_str);
        
        // Exibe no terminal 
        printf("%s | %s\n", temp_str, press_str);
        
        sleep_ms(1000);
    }
    return 0;
}