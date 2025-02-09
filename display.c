#include <stdlib.h>//
#include <stdio.h>//
#include "pico/stdlib.h"//
#include "hardware/i2c.h"//
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/pio.h"
#include "lib/ssd1306.h"
#include "lib/font.h"
#include "lib/numeros.h"
#include "pico/time.h"
#include "ws2812.pio.h" // Biblioteca para controlar os LEDs WS2812

// Definições do display
#define LARGURA 128
#define ALTURA 64
#define PORTA_I2C i2c1
#define PINO_SDA 14
#define PINO_SCL 15
#define ENDERECO_OLED 0x3C
ssd1306_t display;

// Pinos
#define LED_VERMELHO 13
#define LED_VERDE 11
#define LED_AZUL 12
#define BOTAO_A 5
#define BOTAO_B 6
// Matriz 5x5 de LEDs WS2812
#define NUMERO_LEDS 25  
#define led_r 255
#define led_g 0
#define led_b 0
#define PINO_WS2812 7

// Variáveis de estado
bool estadoLedVerde = false;
bool estadoLedAzul = false;

int i=0;
int j=0;
char c;
char c0[21] = "                     ";
char buffer[12] = "           ";

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void set_number(int num) {
    uint32_t color = urgb_u32(led_r, led_g, led_b);
    for (int i = 0; i < NUMERO_LEDS; i++) {
        if (numeros[num][ordem[i]]) {
            put_pixel(color);
        } else {
            put_pixel(0);
        }
    }
}

void gpio_irq_handler(uint gpio, uint32_t eventos) {
    static absolute_time_t ultimo_tempo_a = 0;
    static absolute_time_t ultimo_tempo_b = 0;

    if (gpio == BOTAO_A) {
        static absolute_time_t ultimo_tempo_interrupcao = 0;
        absolute_time_t agora = get_absolute_time();

        // Debounce
        if (absolute_time_diff_us(ultimo_tempo_interrupcao, agora) > 200000) {
            estadoLedVerde = !estadoLedVerde;
            gpio_put(LED_VERDE, estadoLedVerde);

            // Atualização do display
            //ssd1306_fill(&display, false);
            ssd1306_draw_string(&display, estadoLedVerde ? "Led Verde On " : "Led Verde Off", 10, 50);
            ssd1306_send_data(&display);
            // Envio de mensagem ao Serial Monitor
            printf(estadoLedVerde ? "Led Verde On\n" : "Led Verde Off\n");
        }
        ultimo_tempo_interrupcao = agora;
    } else if (gpio == BOTAO_B) {
        static absolute_time_t ultimo_tempo_interrupcao = 0;
        absolute_time_t agora = get_absolute_time();

        // Debounce
        if (absolute_time_diff_us(ultimo_tempo_interrupcao, agora) > 200000) {
            estadoLedAzul = !estadoLedAzul;
            gpio_put(LED_AZUL, estadoLedAzul);

            // Atualização do display
            //ssd1306_fill(&display, false);
            ssd1306_draw_string(&display, estadoLedAzul ? "Led Azul  On " : "Led Azul Off", 10, 50);
            ssd1306_send_data(&display);

            // Envio de mensagem ao Serial Monitor
            printf(estadoLedAzul ? "Led Azul On\n" : "Led Azul Off\n");
        }
        ultimo_tempo_interrupcao = agora;
    }
}
// Inicialização dos botões com interrupções
void inicializar_botoes() {

    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
}
void inicializar_leds(){
        // Configuração dos pinos dos LEDs RGB
        gpio_init(LED_VERMELHO);
        gpio_init(LED_VERDE);
        gpio_init(LED_AZUL);
        gpio_set_dir(LED_VERMELHO, GPIO_OUT);
        gpio_set_dir(LED_VERDE, GPIO_OUT);
        gpio_set_dir(LED_AZUL, GPIO_OUT);
}

// Inicialização do I2C
void inicializar_i2c() {
    i2c_init(PORTA_I2C, 400 * 1000);
    gpio_set_function(PINO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SDA);
    gpio_pull_up(PINO_SCL);
}

// Inicialização do display
void inicializar_display() {
    ssd1306_init(&display, LARGURA, ALTURA, false, ENDERECO_OLED, PORTA_I2C);
    ssd1306_config(&display);
    ssd1306_fill(&display, false);
    ssd1306_send_data(&display);
}

// Inicialização dos LEDs WS2812
void inicializar_ws2812() {
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, PINO_WS2812, 800000, false);
}

void mostrar_aleatorio() {
    // Atualiza o índice com aleatoriedade controlada
    j = (j + 1 + 3) % 62;
    char caractere = letra[j];
    j++;
    i=(i+1)%12;
    i++;
    // Exibição no display
    //ssd1306_fill(&display, false);
    buffer[i] = caractere;
    ssd1306_draw_string(&display, buffer, 10, 30);
    ssd1306_send_data(&display);
    if (caractere >= '0' && caractere <= '9') {
        int numero = caractere - '0';
        set_number(numero); // Exibe o número na matriz de LEDs
    }

    // Exibição no console
    printf("Aleatorio: %c\n", caractere);
}

// // Função para tratar caracteres recebidos via USB
void tratar_usb2() {
    if (stdio_usb_connected()){ // Certifica-se de que o USB está conectado
        if (scanf(" %c", &c) == 1){ // Lê caractere da entrada padrão
            if(c == ' ' || c == '\n' || c == '\t'){

            }else{
                printf("Recebido: '%c'\n", c);
                c0[i]=c;
                i++;
                i =(i+1)%20;
            }
        }
    }

    // Exibição de número na matriz WS2812 (se for um número)
    if (c >= '0' && c <= '9') {
        int numero = c - '0';
        set_number(numero); // Exibe o número na matriz de LEDs
    }
    if(i==20){
        printf("Recebido: '%s'\n", c0); 
    }
    ssd1306_draw_string(&display, c0 , 90, 10);
    ssd1306_send_data(&display);
    printf("Caractere: %c", c0);
}

// Função para tratar caracteres recebidos via USB
void tratar_usb() {
    // Verifica se há caracteres disponíveis na entrada USB
    int caractere = getchar();

    if (caractere != PICO_ERROR_TIMEOUT) { // Se um caractere foi recebido

        i=(i+1)%12;
        i++;
        // Exibição no display
        //ssd1306_fill(&display, false);
        buffer[i] = caractere;
        ssd1306_draw_string(&display, buffer, 10, 30);
        ssd1306_send_data(&display);
        printf("Caractere: %c", caractere);

        // Exibição de número na matriz WS2812 (se for um número)
        if (caractere >= '0' && caractere <= '9') {
            int numero = caractere - '0';
            set_number(numero); // Exibe o número na matriz de LEDs
        }
    }
}

// Função principal
int main() {
    stdio_init_all(); // Inicializa a comunicação serial
    inicializar_i2c(); // Inicializa o I2C
    inicializar_display(); // Inicializa o display
    inicializar_ws2812(); // Inicializa os LEDs WS2812
    inicializar_botoes(); // Inicializa os botões
    inicializar_leds();// Inicializa os leds
    
    ssd1306_draw_string(&display, "by Santos", 30, 10);
    ssd1306_send_data(&display);

    while (true) {
        if (stdio_usb_connected()) {
            bool cor = true;
            cor = !cor;
            ssd1306_rect(&display, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
            tratar_usb(); // Verifica se há caracteres recebidos via USB
        }
        mostrar_aleatorio();
        sleep_ms(500); // Pequeno delay para evitar sobrecarga
    }
}