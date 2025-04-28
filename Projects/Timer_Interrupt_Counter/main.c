#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "include/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

// Definições dos pinos dos botões
#define PINO_BOTAO_INICIAR 5
#define PINO_BOTAO_ACAO    6

// Definições dos pinos de I2C
const uint PINO_SDA = 14;
const uint PINO_SCL = 15;

// Variáveis globais
volatile int valor_contador = 0;
volatile int quantidade_acoes = 0;
volatile bool contagem_ativa = false;
volatile bool flag_botao_iniciar = false;
volatile bool flag_botao_acao = false;
struct repeating_timer temporizador;

// Buffer do display e área de renderização
uint8_t buffer_display[SSD1306_BUF_LEN];
struct render_area area_tela = {
    start_col : 0,
    end_col : SSD1306_WIDTH - 1,
    start_page : 0,
    end_page : SSD1306_NUM_PAGES - 1
};

// Protótipos de funções
void interrupcao_botao_iniciar(uint gpio, uint32_t events);
void interrupcao_botao_acao(uint gpio, uint32_t events);
void atualizar_tela();
bool callback_contagem(struct repeating_timer *t);

int main() {
    stdio_init_all(); // Inicializa comunicação serial

    // Inicializa barramento I2C
    i2c_init(i2c1, SSD1306_I2C_CLK * 1000);
    gpio_set_function(PINO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SDA);
    gpio_pull_up(PINO_SCL);

    // Inicializa o display OLED
    SSD1306_init();
    calc_render_area_buflen(&area_tela);
    memset(buffer_display, 0, SSD1306_BUF_LEN);
    render(buffer_display, &area_tela);

    // Configura os botões como entrada
    gpio_init(PINO_BOTAO_INICIAR);
    gpio_set_dir(PINO_BOTAO_INICIAR, GPIO_IN);
    gpio_pull_up(PINO_BOTAO_INICIAR);

    gpio_init(PINO_BOTAO_ACAO);
    gpio_set_dir(PINO_BOTAO_ACAO, GPIO_IN);
    gpio_pull_up(PINO_BOTAO_ACAO);

    // Ativa interrupções nos botões
    gpio_set_irq_enabled_with_callback(PINO_BOTAO_INICIAR, GPIO_IRQ_EDGE_FALL, true, &interrupcao_botao_iniciar);
    gpio_set_irq_enabled(PINO_BOTAO_ACAO, GPIO_IRQ_EDGE_FALL, true);

    // Inicializa valores iniciais
    valor_contador = 0;
    quantidade_acoes = 0;
    contagem_ativa = false;
    atualizar_tela();

    // Loop principal
    while (true) {
        if (flag_botao_iniciar) {
            flag_botao_iniciar = false;

            // Cancela o temporizador anterior (se houver)
            cancel_repeating_timer(&temporizador);

            // Reinicia contagem
            valor_contador = 9;
            quantidade_acoes = 0;
            contagem_ativa = true;
            atualizar_tela();

            // Cria temporizador de 1 segundo para contagem regressiva
            add_repeating_timer_ms(1000, callback_contagem, NULL, &temporizador);
        }

        if (flag_botao_acao && contagem_ativa) {
            flag_botao_acao = false;
            quantidade_acoes++;
            atualizar_tela();
        }

        tight_loop_contents(); // Evita que o sistema durma
    }

    return 0;
}

// Função de interrupção do botão de iniciar
void interrupcao_botao_iniciar(uint gpio, uint32_t events) {
    if (gpio == PINO_BOTAO_INICIAR) {
        sleep_ms(50); // Debounce simples
        flag_botao_iniciar = true;
    }
}

// Função de interrupção do botão de ação
void interrupcao_botao_acao(uint gpio, uint32_t events) {
    if (gpio == PINO_BOTAO_ACAO) {
        sleep_ms(50); // Debounce simples
        flag_botao_acao = true;
    }
}

// Função chamada pelo temporizador para decrementar o contador
bool callback_contagem(struct repeating_timer *t) {
    if (contagem_ativa) {
        valor_contador--;
        atualizar_tela();

        if (valor_contador <= 0) {
            contagem_ativa = false;
            return false; // Para o temporizador
        }
    }
    return true; // Continua o temporizador
}

// Atualiza o conteúdo do display OLED
void atualizar_tela() {
    char linha_superior[20];
    char linha_inferior[20];

    // Limpa o buffer de tela
    memset(buffer_display, 0, SSD1306_BUF_LEN);

    // Prepara strings para o display
    snprintf(linha_superior, sizeof(linha_superior), "Contador: %d", valor_contador);
    snprintf(linha_inferior, sizeof(linha_inferior), "Acoes: %d", quantidade_acoes);

    // Escreve no buffer
    WriteString(buffer_display, 0, 0, linha_superior);
    WriteString(buffer_display, 0, 16, linha_inferior);
    render(buffer_display, &area_tela);

    // Envia também para o terminal serial
    printf("\n--- Estado Atual ---\n");
    printf("Contador: %d\n", valor_contador);
    printf("Acoes B: %d\n", quantidade_acoes);
    printf("--------------------\n");
}
