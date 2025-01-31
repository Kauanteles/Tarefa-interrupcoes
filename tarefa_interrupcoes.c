#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "ws2812.pio.h"

// Definições de hardware
#define WS2812_PIN 7
#define NUM_PIXELS 25
#define IS_RGBW false
#define LED_R_PIN 13
#define LED_G_PIN 11
#define LED_B_PIN 12
#define BUTTON_A 5
#define BUTTON_B 6
#define TEMPO_LED_RGB 100 //(100ms por troca de estado on/off, 2 trocas são 1 piscada que dura 200ms, totalizando 5 ciclos on/off após 1000ms ou seja 1 segundo)
#define DEBOUNCE_TIME 200000 // 200ms
#define INTENSIDADE_LED 0.1 // Redução do brilho para 10%

// Variáveis globais
static volatile uint32_t last_time_a = 0;
static volatile uint32_t last_time_b = 0;
static volatile int numero_atual = 0;
static volatile bool estado_led_r = false;

// Representação dos números 0 a 9 na matriz 5x5
const bool numeros[10][NUM_PIXELS] = {
    {0,1,1,1,0, 1,0,0,0,1, 1,0,0,0,1, 1,0,0,0,1, 0,1,1,1,0}, // 0
    {0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,1, 0,1,1,0,0, 0,0,1,0,0}, // 1
    {1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,1, 1,1,1,1,1}, // 2
    {1,1,1,1,1, 0,0,0,0,1, 0,1,1,1,1, 0,0,0,0,1, 1,1,1,1,1}, // 3
    {1,0,0,0,0, 0,0,0,0,1, 1,1,1,1,1, 1,0,0,0,1, 1,0,0,0,1}, // 4
    {1,1,1,1,1, 0,0,0,0,1, 1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,1}, // 5
    {1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,1}, // 6
    {0,0,0,0,1, 0,1,0,0,0, 0,0,1,0,0, 0,0,0,1,0, 1,1,1,1,1}, // 7
    {1,1,1,1,1, 1,0,0,0,1, 0,1,1,1,0, 1,0,0,0,1, 1,1,1,1,1}, // 8
    {1,1,1,1,1, 0,0,0,0,1, 1,1,1,1,1, 1,0,0,0,1, 1,1,1,1,1}  // 9
};

// Inicialização do WS2812
static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r * INTENSIDADE_LED) << 8) | ((uint32_t)(g * INTENSIDADE_LED) << 16) | (uint32_t)(b * INTENSIDADE_LED);
}
void exibir_numero(int num) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        put_pixel(numeros[num][i] ? urgb_u32(0, 0, 200) : 0);
    }
}

// Função de interrupção para os botões
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (gpio == BUTTON_A && current_time - last_time_a > DEBOUNCE_TIME) {
        last_time_a = current_time;
        // Verifica se já está no 9, caso contrário, incrementa
        if (numero_atual < 9) {
            numero_atual = (numero_atual + 1) % 10;
            exibir_numero(numero_atual);
        }
    }
    if (gpio == BUTTON_B && current_time - last_time_b > DEBOUNCE_TIME) {
        last_time_b = current_time;
        // Verifica se já está no 0, caso contrário, decrementa
        if (numero_atual > 0) {
            numero_atual = (numero_atual + 9) % 10;
            exibir_numero(numero_atual);
        }
    }
}

int main() {
    stdio_init_all();
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    
    // Configuração dos LEDs
    gpio_init(LED_R_PIN);
    gpio_set_dir(LED_R_PIN, GPIO_OUT);
    gpio_init(LED_G_PIN);
    gpio_set_dir(LED_G_PIN, GPIO_OUT);
    gpio_init(LED_B_PIN);
    gpio_set_dir(LED_B_PIN, GPIO_OUT);
    
    // Configuração dos botões
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    
    // Inicializa a matriz de LEDs com 0
    exibir_numero(0);
    
    // Loop principal para piscar o LED vermelho
    while (true) {
        estado_led_r = !estado_led_r;
        gpio_put(LED_R_PIN, estado_led_r);
        sleep_ms(TEMPO_LED_RGB);
    }
}
