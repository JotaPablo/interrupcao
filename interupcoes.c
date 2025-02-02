#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "neopixel.h"
#include "buzzer.h"

#define RED_PIN 13
#define GREEN_PIN 11
#define BLUE_PIN 12

#define BUTTON_A 5
#define BUTTON_B 6

const uint32_t INTERVALO_BUZZER = 500; //ms
static volatile uint32_t time_buzzer_activated = 0; // Armazena o tempo que foi ligado o buzzer
static volatile bool buzzer_ativo = false; // Armazena se o buzzer está ativo


static volatile uint32_t Numero = 0; // Variavel global para guardar o número que deve ser escrito na matriz de led
static volatile bool atualizarMatriz = false; // Flag para se preciso atualizar a matriz de leds
static volatile bool limiteExcedido = false; // Flag acioda se houve a tentativa de incrementar o número 9 ou de decrementar o 0

//Desenha um número na matriz de led (De 0 á 9), podendo escolher qual a cor do número e qual a cor do fundo
void exibirNumeroComFundo(uint32_t numero, 
                             uint8_t num_r, uint8_t num_g, uint8_t num_b,
                             uint8_t bg_r, uint8_t bg_g, uint8_t bg_b);

static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)
static void gpio_irq_handler(uint gpio, uint32_t events); // Função para gerenciar as interrupções dos botões


static volatile bool led_ativo = false; // Guarda o estado atual do led
// Esta função alterna o estado do LED
void turn_led(uint LED_GPIO);


int main()
{
    stdio_init_all();

    gpio_init(RED_PIN);              // Inicializa o pino do LED
    gpio_set_dir(RED_PIN, GPIO_OUT); // Configura o pino como saída
    gpio_init(GREEN_PIN);              // Inicializa o pino do LED
    gpio_set_dir(GREEN_PIN, GPIO_OUT); // Configura o pino como saída
    gpio_init(BLUE_PIN);              // Inicializa o pino do LED
    gpio_set_dir(BLUE_PIN, GPIO_OUT); // Configura o pino como saída

    pwm_init_buzzer(BUZZER_PIN); // Inicia as configurações do Buzzer

    npInit(LED_PIN); // Inicia as configurações da Matriz de Led

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(BUTTON_A);          // Habilita o pull-up interno

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(BUTTON_B);          // Habilita o pull-up interno

    // Configurações para as interrupções
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    //struct repeating_timer timer;
    //Configura para chamar semaforo_callback a cada 100 ms fazendo o LED piscar 5 vezes em um segundo
    //add_repeating_timer_ms(100, turn_led_callback, NULL, &timer);
    
    // Exibe o número verde com fundo preto(leds desligados)
    exibirNumeroComFundo(Numero,
                                    0, 10, 0,
                                    0, 0, 0);
    while (true) {

        uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());

        
        if(limiteExcedido){
            limiteExcedido = false; // Reseta a flag
            turn_on_buzzer(BUZZER_PIN, 2000); // Liga o Buzzer
            buzzer_ativo = true; // Marca Buzzer como Ativado
            time_buzzer_activated = to_ms_since_boot(get_absolute_time()); // Atualiza qual o tempo que o buzzer foi ativado
            // Coloca o número atual como vvermelho
            exibirNumeroComFundo(Numero,
                                10, 0, 0,
                                0, 0, 0);
        }

        // Se o buzzer tiver ativo e tiverem se passado 500 ms
        if(buzzer_ativo && tempo_atual - time_buzzer_activated >= INTERVALO_BUZZER){
            turn_off_buzzer(BUZZER_PIN); // Desliga o Buzzer
            buzzer_ativo = false; // Marca o buzzer como desativado
            // Coloca o número atual como verde novamente
            exibirNumeroComFundo(Numero,
                                0, 10, 0,
                                0, 0, 0);
        }

        if (atualizarMatriz) {
            // Coloca o número atual como verde
            exibirNumeroComFundo(Numero,
                                0, 10, 0,
                                0, 0, 0); // Atualiza Matriz
            atualizarMatriz = false;  // Reseta a flag
            // Se eu pedir pra atualizar mas o intervalo do buzzer ainda não acabou, o desativa antes
            if(buzzer_ativo){
                turn_off_buzzer(BUZZER_PIN);
                buzzer_ativo = false;
            }
        }

        
        turn_led(RED_PIN); //Altera o estado do led

        sleep_ms(100); // Permite que o LED pisque 5 vezes por segundo
    }
}


void gpio_irq_handler(uint gpio, uint32_t events)
{   uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_time > 200000) {
        last_time = current_time; // Atualiza o tempo do último evento

        if (gpio == BUTTON_A) {
            // Lógica para o botão A
            if (Numero == 9) limiteExcedido = true;
            else {
                Numero++;  // incrementa a variavel
                atualizarMatriz = true;
            }
        } else if (gpio == BUTTON_B) {
            // Lógica para o botão B (você pode modificar a ação aqui)
            if (Numero == 0) {
                limiteExcedido = true;
            } else {
                Numero--;  // Decrementa a variável
                atualizarMatriz = true;
            }
        }
    }
}

// Função para Alterna Estado do Led
void turn_led(uint LED_GPIO){
    gpio_put(LED_GPIO, led_ativo);
    led_ativo = !led_ativo;
}


void exibirNumeroComFundo(uint32_t numero, 
                             uint8_t num_r, uint8_t num_g, uint8_t num_b,
                             uint8_t bg_r, uint8_t bg_g, uint8_t bg_b) {
    // Preenche todo o fundo primeiro
    for(int i = 0; i < 25; i++) {
        npSetLED(i, bg_r, bg_g, bg_b);
    }

    switch(numero){
        case 0:
            npSetLED(getIndex(1,4), num_r,num_g,num_b);
            npSetLED(getIndex(2,4), num_r,num_g,num_b);
            npSetLED(getIndex(3,4), num_r,num_g,num_b);
            npSetLED(getIndex(1,3), num_r,num_g,num_b);
            npSetLED(getIndex(3,3), num_r,num_g,num_b);
            npSetLED(getIndex(1,2), num_r,num_g,num_b);
            npSetLED(getIndex(3,2), num_r,num_g,num_b);
            npSetLED(getIndex(1,1), num_r,num_g,num_b);
            npSetLED(getIndex(3,1), num_r,num_g,num_b);
            npSetLED(getIndex(1,0), num_r,num_g,num_b);
            npSetLED(getIndex(2,0), num_r,num_g,num_b);
            npSetLED(getIndex(3,0), num_r,num_g,num_b);
            break;

        case 1:
            npSetLED(getIndex(2,4), num_r,num_g,num_b);
            npSetLED(getIndex(1,3), num_r,num_g,num_b);
            npSetLED(getIndex(2,3), num_r,num_g,num_b);
            npSetLED(getIndex(2,2), num_r,num_g,num_b);
            npSetLED(getIndex(2,1), num_r,num_g,num_b);
            npSetLED(getIndex(1,0), num_r,num_g,num_b);
            npSetLED(getIndex(2,0), num_r,num_g,num_b);
            npSetLED(getIndex(3,0), num_r,num_g,num_b);
            break;

        case 2:
            npSetLED(getIndex(1,4), num_r,num_g,num_b);
            npSetLED(getIndex(2,4), num_r,num_g,num_b);
            npSetLED(getIndex(3,4), num_r,num_g,num_b);
            npSetLED(getIndex(3,3), num_r,num_g,num_b);
            npSetLED(getIndex(3,2), num_r,num_g,num_b);
            npSetLED(getIndex(2,2), num_r,num_g,num_b);
            npSetLED(getIndex(1,2), num_r,num_g,num_b);
            npSetLED(getIndex(1,1), num_r,num_g,num_b);
            npSetLED(getIndex(1,0), num_r,num_g,num_b);
            npSetLED(getIndex(2,0), num_r,num_g,num_b);
            npSetLED(getIndex(3,0), num_r,num_g,num_b);
            break;

        case 3:
            npSetLED(getIndex(1,4), num_r,num_g,num_b);
            npSetLED(getIndex(2,4), num_r,num_g,num_b);
            npSetLED(getIndex(3,4), num_r,num_g,num_b);
            npSetLED(getIndex(3,3), num_r,num_g,num_b);
            npSetLED(getIndex(3,2), num_r,num_g,num_b);
            npSetLED(getIndex(2,2), num_r,num_g,num_b);
            npSetLED(getIndex(3,1), num_r,num_g,num_b);
            npSetLED(getIndex(1,0), num_r,num_g,num_b);
            npSetLED(getIndex(2,0), num_r,num_g,num_b);
            npSetLED(getIndex(3,0), num_r,num_g,num_b);
            break;

        case 4:
            npSetLED(getIndex(1,4), num_r,num_g,num_b);
            npSetLED(getIndex(3,4), num_r,num_g,num_b);
            npSetLED(getIndex(1,3), num_r,num_g,num_b);
            npSetLED(getIndex(3,3), num_r,num_g,num_b);
            npSetLED(getIndex(1,2), num_r,num_g,num_b);
            npSetLED(getIndex(2,2), num_r,num_g,num_b);
            npSetLED(getIndex(3,2), num_r,num_g,num_b);
            npSetLED(getIndex(3,1), num_r,num_g,num_b);
            npSetLED(getIndex(3,0), num_r,num_g,num_b);
            break;

        case 5:
            npSetLED(getIndex(1,4), num_r,num_g,num_b);
            npSetLED(getIndex(2,4), num_r,num_g,num_b);
            npSetLED(getIndex(3,4), num_r,num_g,num_b);
            npSetLED(getIndex(1,3), num_r,num_g,num_b);
            npSetLED(getIndex(1,2), num_r,num_g,num_b);
            npSetLED(getIndex(2,2), num_r,num_g,num_b);
            npSetLED(getIndex(3,2), num_r,num_g,num_b);
            npSetLED(getIndex(3,1), num_r,num_g,num_b);
            npSetLED(getIndex(1,0), num_r,num_g,num_b);
            npSetLED(getIndex(2,0), num_r,num_g,num_b);
            npSetLED(getIndex(3,0), num_r,num_g,num_b);
            break;

        case 6:
            npSetLED(getIndex(1,4), num_r,num_g,num_b);
            npSetLED(getIndex(2,4), num_r,num_g,num_b);
            npSetLED(getIndex(3,4), num_r,num_g,num_b);
            npSetLED(getIndex(1,3), num_r,num_g,num_b);
            npSetLED(getIndex(1,2), num_r,num_g,num_b);
            npSetLED(getIndex(2,2), num_r,num_g,num_b);
            npSetLED(getIndex(3,2), num_r,num_g,num_b);
            npSetLED(getIndex(1,1), num_r,num_g,num_b);
            npSetLED(getIndex(3,1), num_r,num_g,num_b);
            npSetLED(getIndex(1,0), num_r,num_g,num_b);
            npSetLED(getIndex(2,0), num_r,num_g,num_b);
            npSetLED(getIndex(3,0), num_r,num_g,num_b);
            break;

        case 7:
            npSetLED(getIndex(1,4), num_r,num_g,num_b);
            npSetLED(getIndex(2,4), num_r,num_g,num_b);
            npSetLED(getIndex(3,4), num_r,num_g,num_b);
            npSetLED(getIndex(3,3), num_r,num_g,num_b);
            npSetLED(getIndex(3,2), num_r,num_g,num_b);
            npSetLED(getIndex(3,1), num_r,num_g,num_b);
            npSetLED(getIndex(3,0), num_r,num_g,num_b);
            break;

        case 8:
            npSetLED(getIndex(1,4), num_r,num_g,num_b);
            npSetLED(getIndex(2,4), num_r,num_g,num_b);
            npSetLED(getIndex(3,4), num_r,num_g,num_b);
            npSetLED(getIndex(1,3), num_r,num_g,num_b);
            npSetLED(getIndex(3,3), num_r,num_g,num_b);
            npSetLED(getIndex(1,2), num_r,num_g,num_b);
            npSetLED(getIndex(2,2), num_r,num_g,num_b);
            npSetLED(getIndex(3,2), num_r,num_g,num_b);
            npSetLED(getIndex(1,1), num_r,num_g,num_b);
            npSetLED(getIndex(3,1), num_r,num_g,num_b);
            npSetLED(getIndex(1,0), num_r,num_g,num_b);
            npSetLED(getIndex(2,0), num_r,num_g,num_b);
            npSetLED(getIndex(3,0), num_r,num_g,num_b);
            break;

        case 9:
            npSetLED(getIndex(1,4), num_r,num_g,num_b);
            npSetLED(getIndex(2,4), num_r,num_g,num_b);
            npSetLED(getIndex(3,4), num_r,num_g,num_b);
            npSetLED(getIndex(1,3), num_r,num_g,num_b);
            npSetLED(getIndex(3,3), num_r,num_g,num_b);
            npSetLED(getIndex(1,2), num_r,num_g,num_b);
            npSetLED(getIndex(2,2), num_r,num_g,num_b);
            npSetLED(getIndex(3,2), num_r,num_g,num_b);
            npSetLED(getIndex(3,1), num_r,num_g,num_b);
            npSetLED(getIndex(1,0), num_r,num_g,num_b);
            npSetLED(getIndex(2,0), num_r,num_g,num_b);
            npSetLED(getIndex(3,0), num_r,num_g,num_b);
            break;

        default: break;
    }
    npWrite();
}