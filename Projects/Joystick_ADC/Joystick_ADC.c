#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

//Definição Display
#define I2C_SDA 14
#define I2C_SCL 15
#define DISPLAY_I2C i2c1 //(Existem 2) 
#define DISPLAY_I2C_SPEED 400000 //Velocidade 400 KhZ

//Definição Joystick
#define VRX_PIN 26
#define VRY_PIN 27
#define ADC_CHANNEL_X 0
#define ADC_CHANNEL_Y 1

//Comunicação Display OLED via I2C
void display_init()
{
    i2c_init(DISPLAY_I2C, DISPLAY_I2C_SPEED); //Inicializa o barramento I2C
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); //Configura os pinos SDA para a função i2c
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); //Configura os pinos SCL para a função i2c
    gpio_pull_up(I2C_SCL); //Ativa os resistores de Pull-up
    gpio_pull_up(I2C_SDA);
    ssd1306_init(); //Inicializa o Display
}

//Exibe os valores do Joystick no display
void show_joystick_on_display(uint16_t x, uint16_t y)
{ 
    //Define a área do display onde o conteúdo será desenhado
    struct render_area frame_area = 
    {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area); //Calcula o tamanho do buffer necessário para desenhar.

    //Cria e zera um buffer de memória que vai conter os dados a serem exibidos
    uint8_t buffer[ssd1306_buffer_length];
    memset(buffer, 0, ssd1306_buffer_length);  // Limpa o buffer

    //Tranforma os valores de X e Y em Strings
    char msg_x [20];
    char msg_y [20];
    sprintf(msg_x, "X: %4d", x);
    sprintf(msg_y, " Y: %4d", y);

    //Desenha as mensagens no Buffer
    ssd1306_draw_string(buffer, 0, 0, msg_x); // Desenha msg_x no canto superior esquerdo
    ssd1306_draw_string(buffer, 64, 0, msg_y); // Desenha msg_y na metade direita da tela

    render_on_display(buffer, &frame_area); // Mostra o buffer no display
}
//Inicialização do Joystick
void setup_joystick()
{
    adc_init(); //Inicializa o ADC
    adc_gpio_init(VRX_PIN); // Habilita o ADC no pino 26
    adc_gpio_init(VRY_PIN); // Habilita o ADC no pino 27
}

//Leitura do Joystick
void read_joystick(uint16_t *x, uint16_t *y)
{
    adc_select_input(ADC_CHANNEL_X); //Seleciona o canal X
    sleep_us(2); //Pausa
    *x = adc_read(); //Lê valor analógico

    adc_select_input(ADC_CHANNEL_Y);
    sleep_us(2);
    *y = adc_read();
}

int main()
{
    stdio_init_all();
    setup_joystick();
    display_init();

    uint16_t x_val, y_val;

    while(true)
    {
        read_joystick(&x_val, &y_val); //Lê o joystick

        show_joystick_on_display(x_val, y_val); //Exibe no Display

        sleep_ms(300);  // Aguarda 300 ms
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
