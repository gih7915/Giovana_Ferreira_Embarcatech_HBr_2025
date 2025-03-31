#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/adc.h"
#include "ws2818b.pio.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

// Definições de constantes
#define LED_COUNT 25
#define LED_PIN 7
#define MATRIX_WIDTH 5
#define MATRIX_HEIGHT 5
#define BUTTON_A 5
#define BUTTON_B 6
#define DEADZONE 500
#define MAX_LENGTH 16
#define SEQUENCE_LENGTH 10

// Pinos do joystick
#define VRX_PIN 26
#define VRY_PIN 27
#define ADC_CHANNEL_X 1
#define ADC_CHANNEL_Y 0

// Constantes para o display OLED
#define I2C_SDA 14
#define I2C_SCL 15
#define DISPLAY_I2C i2c1
#define DISPLAY_I2C_SPEED 400000

// Estrutura para representar um LED RGB
typedef struct {
    uint8_t G, R, B;
} npLED_t;

// Estrutura para representar uma posição
typedef struct {
    int x, y;
} Position;

// Variáveis globais
static npLED_t leds[LED_COUNT];
static PIO np_pio = pio0;
static uint sm;

static Position cursor[MAX_LENGTH];
static int length = 1;

static int sequence[SEQUENCE_LENGTH];
static int current_sequence_length = 1;
static int current_step = 0;
static int acertos = 0;

// Cores para os LEDs (R, G, B)
static const uint8_t colors[SEQUENCE_LENGTH][3] = {
    {163, 85, 85},   // Vermelho
    {75, 209, 144},  // Verde
    {108, 109, 168}, // Azul
    {194, 186, 97},  // Amarelo
    {163, 85, 136},  // Magenta
    {149, 210, 245}, // Ciano
    {150, 98, 179},  // Roxo
    {222, 165, 104}, // Laranja
    {128, 128, 128}, // Cinza
    {82, 156, 87}    // Verde escuro
};

// Funções para o display OLED
void display_init() {
    i2c_init(DISPLAY_I2C, DISPLAY_I2C_SPEED);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();
}

void display_message(const char *line1, const char *line2) {
    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&frame_area);

    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);

    ssd1306_draw_string(ssd, 5, 10, line1);
    ssd1306_draw_string(ssd, 5, 20, line2);

    render_on_display(ssd, &frame_area);
}

// Funções para os LEDs
static int get_led_index(int x, int y) {
    return (y % 2 == 0) ? (y * MATRIX_WIDTH + x) : (y * MATRIX_WIDTH + (MATRIX_WIDTH - 1 - x));
}

void npInit(uint pin) {
    uint offset = pio_add_program(np_pio, &ws2818b_program);
    sm = pio_claim_unused_sm(np_pio, false);
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
}

void npSetLED(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    int index = get_led_index(x, y);
    if (index >= 0 && index < LED_COUNT) {
        leds[index].R = r;
        leds[index].G = g;
        leds[index].B = b;
    }
}

void npClear() {
    memset(leds, 0, sizeof(leds));
}

void npwrite() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100);
}

// Funções para o joystick
void setup_joystick() {
    adc_init();
    adc_gpio_init(VRX_PIN);
    adc_gpio_init(VRY_PIN);
}

void joystick_read_axis(uint16_t *x, uint16_t *y) {
    adc_select_input(ADC_CHANNEL_Y);
    sleep_us(2);
    *y = adc_read();
    adc_select_input(ADC_CHANNEL_X);
    sleep_us(2);
    *x = adc_read();
}

void move_cursor() {
    uint16_t valor_x, valor_y;
    joystick_read_axis(&valor_x, &valor_y);
    int new_x = cursor[0].x;
    int new_y = cursor[0].y;

    // Lógica para o eixo X
    if (valor_x < 1000 && (2048 - valor_x) > DEADZONE && new_x < MATRIX_WIDTH - 1) new_x++;
    else if (valor_x > 3000 && (valor_x - 2048) > DEADZONE && new_x > 0) new_x--;

    // Lógica para o eixo Y
    if (valor_y < 1000 && (2048 - valor_y) > DEADZONE && new_y > 0) new_y--;
    else if (valor_y > 3000 && (valor_y - 2048) > DEADZONE && new_y < MATRIX_HEIGHT - 1) new_y++;

    // Atualiza a posição do cursor
    for (int i = length - 1; i > 0; i--) {
        cursor[i] = cursor[i - 1];
    }

    cursor[0].x = new_x;
    cursor[0].y = new_y;
}

// Funções para os botões
void setup_buttons() {
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
}

bool is_button_pressed(uint button_pin) {
    return !gpio_get(button_pin);
}

// Funções do jogo
void generate_sequence() {
    for (int i = 0; i < SEQUENCE_LENGTH; i++) {
        sequence[i] = rand() % LED_COUNT;
    }
}

void show_sequence() {
    for (int i = 0; i < current_sequence_length; i++) {
        int led_index = sequence[i];
        npSetLED(led_index % MATRIX_WIDTH, led_index / MATRIX_WIDTH, 
                colors[i][0] / 10, colors[i][1] / 10, colors[i][2] / 10);
        npwrite();
        sleep_ms(500);
        npClear();
        npwrite();
        sleep_ms(200);
    }
}

bool check_player_input() {
    for (int i = 0; i < current_sequence_length; i++) {
        bool pressed_correctly = false;

        while (!pressed_correctly) {
            move_cursor();
            npClear();
            npSetLED(cursor[0].x, cursor[0].y, 2, 2, 2);
            npwrite();

            if (is_button_pressed(BUTTON_A)) {
                sleep_ms(200); // Debounce
                int led_index = cursor[0].y * MATRIX_WIDTH + cursor[0].x;

                if (led_index == sequence[i]) {
                    pressed_correctly = true;
                } else {
                    return false;
                }
            }
            sleep_ms(100);
        }
    }
    return true;
}

void show_congrats_message(int level) {
    const char *messages[][2] = {
        {"UAU CONTINUE", "DESSE JEITO!"},
        {"VOCE E", "IMPRESSIONANTE!"},
        {"VOCE ESTA", "QUASE LA!!!"},
        {"Voce ganhou!", "Parabens!"}
    };
    
    int index = (level - 3) / 2; // Mapeia 3,5,7 para 0,1,2
    if (level == 10) index = 3;
    
    display_message(messages[index][0], messages[index][1]);
    sleep_ms(2000);
}

void game_loop() {
    while (true) {
        generate_sequence();
        current_sequence_length = 1;

        while (current_sequence_length <= SEQUENCE_LENGTH) {
            show_sequence();

            if (!check_player_input()) {
                display_message("Voce perdeu!", "Tente novamente");
                npClear();
                npwrite();
                sleep_ms(2000);
                break;
            }

            acertos++;
            
            if (acertos == 3 || acertos == 5 || acertos == 7 || acertos == 10) {
                show_congrats_message(acertos);
                
                if (acertos == 10) {
                    display_message("A - FINALIZAR", "B - CONTINUAR");
                    npClear();
                    npwrite();

                    while (true) {
                        if (is_button_pressed(BUTTON_A)) return;
                        if (is_button_pressed(BUTTON_B)) {
                            acertos = 0;
                            break;
                        }
                        sleep_ms(100);
                    }
                }
            }
            current_sequence_length++;
        }
    }
}

int main() {
    stdio_init_all();
    npInit(LED_PIN);
    setup_joystick();
    setup_buttons();
    display_init();

    cursor[0].x = 2;
    cursor[0].y = 2;

    // Mensagens iniciais
    display_message("Bem-vindos ao", "Luminus Memory");
    sleep_ms(4000);
    display_message("Aperte A para", "iniciar o jogo");
    
    while (!is_button_pressed(BUTTON_A)) {
        sleep_ms(100);
    }
    
    // Tutorial
    display_message("MOVA O JOYSTICK", "PARA NAVEGAR");
    sleep_ms(4000);
    display_message("APERTE A PARA", "ESCOLHER O LED");
    sleep_ms(4000);
    display_message("PREPARAARR", "JAA");
    sleep_ms(3000);

    game_loop();

    return 0;
}