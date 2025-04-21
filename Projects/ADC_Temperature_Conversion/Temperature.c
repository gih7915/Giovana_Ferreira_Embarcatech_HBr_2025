#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "inc/ssd1306.h"

//Definição do Display
#define I2C_SDA 14
#define I2C_SCL 15
#define DISPLAY_I2C i2c1

//Definição do Sensor de Temperatura
#define SENSOR_TEMP 4

//Fator de conversão de leitura do ADC para tensão.
#define CONVERSION_FACTOR (3.3f / (1 << 12))       //AINDA NAO ENTENDI

//Função para converter para Celsius
float convert_celsius(uint16_t raw) //raw é o valor lido do ADC(0-4095)
{
    float voltage = raw * CONVERSION_FACTOR; //Converte RAW para a tensão real(0,0v - 3,3v)
    float temperature_celsius =  27.0f - (voltage - 0.706f) / 0.001721f;

    return temperature_celsius;
}

//Função para inicializar o Display
void display_init()
{
    i2c_init(DISPLAY_I2C, ssd1306_i2c_clock * 1000); //Inicializa o barramento I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); //Configura os pinos SCL para a função i2c
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); //Configura os pinos SDA para a função i2c
    gpio_pull_up(I2C_SCL); //Ativa os resistores de Pull-up
    gpio_pull_up(I2C_SDA); //Ativa os resistores de Pull-up
    ssd1306_init(); //Inicializa o Display
}

// Função para mostrar a temp no display
void show_temp_on_display(uint8_t *buffer, float temp)
{
    //Define a área do display onde o conteúdo será desenhado
    struct render_area frame_area = 
    {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };

    memset(buffer, 0, ssd1306_width * ssd1306_n_pages); //Zera o buffer antes de desenhar

    char string[32]; //para guardar a temp

    ssd1306_draw_string(buffer, 0, 0, "TEMP DO RP240"); //Desenha a mensagem no display
    sprintf(string, "%.2f C", temp);
    ssd1306_draw_string(buffer, 0, 16, string); //Desenha a mensagen no display

    calculate_render_area_buffer_length(&frame_area);  //Calcula o tamanho do buffer necessário para desenhar.
    render_on_display(buffer, &frame_area); // Mostra o buffer no display
}

int main()
{
    stdio_init_all();
    display_init();
    adc_init();
    adc_set_temp_sensor_enabled(true); //Ativa o sensor de temperatura interno.
    adc_select_input(SENSOR_TEMP);

    uint8_t buffer[ssd1306_width * ssd1306_n_pages];
    
    while (true) {
        uint16_t raw = adc_read();

        float temp = convert_celsius(raw);

        show_temp_on_display(buffer, temp);

        sleep_ms(500); 
    }

}