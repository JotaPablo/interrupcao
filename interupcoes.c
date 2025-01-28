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

const uint32_t TEMPO = 100; 

static volatile uint32_t a = 0;
static volatile bool atualizarMatriz = false;
static volatile bool limiteExcedido = false;

void setNumeroMatriz(uint32_t numero);
void sinaliza_limite_excedido();


static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)
static void gpio_irq_handler(uint gpio, uint32_t events);


int main()
{
    stdio_init_all();

    gpio_init(RED_PIN);              // Inicializa o pino do LED
    gpio_set_dir(RED_PIN, GPIO_OUT); // Configura o pino como saída
    gpio_init(GREEN_PIN);              // Inicializa o pino do LED
    gpio_set_dir(GREEN_PIN, GPIO_OUT); // Configura o pino como saída
    gpio_init(BLUE_PIN);              // Inicializa o pino do LED
    gpio_set_dir(BLUE_PIN, GPIO_OUT); // Configura o pino como saída

    pwm_init_buzzer(BUZZER_PIN);

    npInit(LED_PIN);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(BUTTON_A);          // Habilita o pull-up interno

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(BUTTON_B);          // Habilita o pull-up interno

    
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

   
    setNumeroMatriz(a);
    while (true) {

        // Código para piscar o LED RGB e Atualizar matriz()
        for(int i = 0; i < 5; i++){

            if(limiteExcedido){
                sinaliza_limite_excedido();
                limiteExcedido = false;
            }

            if (atualizarMatriz) {
                setNumeroMatriz(a);  // Atualiza a matriz
                atualizarMatriz = false;  // Reseta a flag
            }

            gpio_put(RED_PIN, true);
            //gpio_put(GREEN_PIN, true);
            //gpio_put(BLUE_PIN, true);
            sleep_ms(TEMPO);

            gpio_put(RED_PIN, false);
            //gpio_put(GREEN_PIN, false);
            //gpio_put(BLUE_PIN, false);
            sleep_ms(TEMPO);
        }
    }
}


void gpio_irq_handler(uint gpio, uint32_t events)
{   uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_time > 200000) {
        last_time = current_time; // Atualiza o tempo do último evento

        if (gpio == BUTTON_A) {
            // Lógica para o botão A
            if (a == 9) limiteExcedido = true;
            else {
                a++;  // incrementa a variavel
                atualizarMatriz = true;
            }
        } else if (gpio == BUTTON_B) {
            // Lógica para o botão B (você pode modificar a ação aqui)
            if (a == 0) {
                limiteExcedido = true;
            } else {
                a--;  // Decrementa a variável
                atualizarMatriz = true;
            }
        }
    }
}


void setNumeroMatriz(uint32_t numero){

    uint32_t brilho = 255;
    
    npClear();

    switch(numero){
        case 0:
            // Linha superior
            npSetLED(getIndex(1, 4), 0, brilho, 0);
            npSetLED(getIndex(2, 4), 0, brilho, 0);
            npSetLED(getIndex(3, 4), 0, brilho, 0);

            // Linhas do meio (lados esquerdo e direito)
            npSetLED(getIndex(1, 3), 0, brilho, 0);
            npSetLED(getIndex(3, 3), 0, brilho, 0);

            npSetLED(getIndex(1, 2), 0, brilho, 0);
            npSetLED(getIndex(3, 2), 0, brilho, 0);

            npSetLED(getIndex(1, 1), 0, brilho, 0);
            npSetLED(getIndex(3, 1), 0, brilho, 0);

            // Linha inferior
            npSetLED(getIndex(1, 0), 0, brilho, 0);
            npSetLED(getIndex(2, 0), 0, brilho, 0);
            npSetLED(getIndex(3, 0), 0, brilho, 0);

            // Atualiza a matriz de LEDs
            npWrite();
            break;
        
        case 1:
            // Coluna central
            npSetLED(getIndex(2, 4), 0, brilho, 0); // Topo
            npSetLED(getIndex(2, 3), 0, brilho, 0);
            npSetLED(getIndex(2, 2), 0, brilho, 0);
            npSetLED(getIndex(2, 1), 0, brilho, 0);
            npSetLED(getIndex(2, 0), 0, brilho, 0); // Base

            // Pequeno traço superior à esquerda
            npSetLED(getIndex(1, 3), 0, brilho, 0);

            //Base
            npSetLED(getIndex(1, 0), 0, brilho, 0);
            npSetLED(getIndex(3, 0), 0, brilho, 0);

            // Atualiza a matriz de LEDs
            npWrite();
            break;
        
        case 2:
            // Linha superior
            npSetLED(getIndex(1, 4), 0, brilho, 0);
            npSetLED(getIndex(2, 4), 0, brilho, 0);
            npSetLED(getIndex(3, 4), 0, brilho, 0);

            // Meio
            npSetLED(getIndex(3, 3), 0, brilho, 0);
            npSetLED(getIndex(3, 2), 0, brilho, 0);
            npSetLED(getIndex(2, 2), 0, brilho, 0);
            npSetLED(getIndex(1, 2), 0, brilho, 0);
            npSetLED(getIndex(1, 1), 0, brilho, 0);

            // Linha inferior
            npSetLED(getIndex(1, 0), 0, brilho, 0);
            npSetLED(getIndex(2, 0), 0, brilho, 0);
            npSetLED(getIndex(3, 0), 0, brilho, 0);
            npWrite();
            break;
        
        case 3:
            // Linha superior
            npSetLED(getIndex(1, 4), 0, brilho, 0);
            npSetLED(getIndex(2, 4), 0, brilho, 0);
            npSetLED(getIndex(3, 4), 0, brilho, 0);

            // Linha do meio
            npSetLED(getIndex(3, 3), 0, brilho, 0);
            npSetLED(getIndex(3, 2), 0, brilho, 0);
            npSetLED(getIndex(3, 1), 0, brilho, 0);
            npSetLED(getIndex(2, 2), 0, brilho, 0);

            // Linha inferior
            npSetLED(getIndex(1, 0), 0, brilho, 0);
            npSetLED(getIndex(2, 0), 0, brilho, 0);
            npSetLED(getIndex(3, 0), 0, brilho, 0);
            npWrite();
            break;
        case 4:
            // Coluna esquerda
            npSetLED(getIndex(1, 4), 0, brilho, 0);
            npSetLED(getIndex(1, 3), 0, brilho, 0);
            npSetLED(getIndex(1, 2), 0, brilho, 0);

            // Linha do meio
            npSetLED(getIndex(2, 2), 0, brilho, 0);
            npSetLED(getIndex(3, 2), 0, brilho, 0);

            // Coluna direita
            npSetLED(getIndex(3, 4), 0, brilho, 0);
            npSetLED(getIndex(3, 3), 0, brilho, 0);
            npSetLED(getIndex(3, 2), 0, brilho, 0);
            npSetLED(getIndex(3, 1), 0, brilho, 0);
            npSetLED(getIndex(3, 0), 0, brilho, 0);
            npWrite();
            break;
        
        case 5:
            // Linha superior
            npSetLED(getIndex(1, 4), 0, brilho, 0);
            npSetLED(getIndex(2, 4), 0, brilho, 0);
            npSetLED(getIndex(3, 4), 0, brilho, 0);

            // Linha do meio
            npSetLED(getIndex(1, 3), 0, brilho, 0);
            npSetLED(getIndex(1, 2), 0, brilho, 0);
            npSetLED(getIndex(2, 2), 0, brilho, 0);
            npSetLED(getIndex(3, 2), 0, brilho, 0);
            npSetLED(getIndex(3, 1), 0, brilho, 0);

            // Linha inferior
            npSetLED(getIndex(1, 0), 0, brilho, 0);
            npSetLED(getIndex(2, 0), 0, brilho, 0);
            npSetLED(getIndex(3, 0), 0, brilho, 0);
            npWrite();
            break;

        case 6:
            // Linha superior
            npSetLED(getIndex(1, 4), 0, brilho, 0);
            npSetLED(getIndex(2, 4), 0, brilho, 0);
            npSetLED(getIndex(3, 4), 0, brilho, 0);

            // Linha do meio
            npSetLED(getIndex(1, 3), 0, brilho, 0);
            npSetLED(getIndex(2, 2), 0, brilho, 0);
            npSetLED(getIndex(3, 1), 0, brilho, 0);
            npSetLED(getIndex(3, 2), 0, brilho, 0);


            // Linha inferior
            npSetLED(getIndex(1, 0), 0, brilho, 0);
            npSetLED(getIndex(2, 0), 0, brilho, 0);
            npSetLED(getIndex(3, 0), 0, brilho, 0);

            // Coluna esquerda
            npSetLED(getIndex(1, 3), 0, brilho, 0);
            npSetLED(getIndex(1, 2), 0, brilho, 0);
            npSetLED(getIndex(1, 1), 0, brilho, 0);
            npWrite();
            break;
        
        case 7:
            // Linha superior
            npSetLED(getIndex(1, 4), 0, brilho, 0);
            npSetLED(getIndex(2, 4), 0, brilho, 0);
            npSetLED(getIndex(3, 4), 0, brilho, 0);

            // Coluna direita
            npSetLED(getIndex(3, 3), 0, brilho, 0);
            npSetLED(getIndex(3, 2), 0, brilho, 0);
            npSetLED(getIndex(3, 1), 0, brilho, 0);
            npSetLED(getIndex(3, 0), 0, brilho, 0);

            npWrite();
            break;

        case 8:
             // Linha superior
            npSetLED(getIndex(1, 4), 0, brilho, 0);
            npSetLED(getIndex(2, 4), 0, brilho, 0);
            npSetLED(getIndex(3, 4), 0, brilho, 0);

            // Linhas do meio (lados esquerdo e direito)
            npSetLED(getIndex(1, 3), 0, brilho, 0);
            npSetLED(getIndex(3, 3), 0, brilho, 0);

            npSetLED(getIndex(1, 2), 0, brilho, 0);
            npSetLED(getIndex(3, 2), 0, brilho, 0);

            npSetLED(getIndex(1, 1), 0, brilho, 0);
            npSetLED(getIndex(3, 1), 0, brilho, 0);

            npSetLED(getIndex(2, 2), 0, brilho, 0);

            // Linha inferior
            npSetLED(getIndex(1, 0), 0, brilho, 0);
            npSetLED(getIndex(2, 0), 0, brilho, 0);
            npSetLED(getIndex(3, 0), 0, brilho, 0);

            // Atualiza a matriz de LEDs
            npWrite();
            break;
        
        case 9:
            // Linha superior
            npSetLED(getIndex(1, 4), 0, brilho, 0);
            npSetLED(getIndex(2, 4), 0, brilho, 0);
            npSetLED(getIndex(3, 4), 0, brilho, 0);

            // Linha do meio
            npSetLED(getIndex(1, 3), 0, brilho, 0);
            npSetLED(getIndex(2, 2), 0, brilho, 0);
            npSetLED(getIndex(1, 2), 0, brilho, 0);
            npSetLED(getIndex(3, 2), 0, brilho, 0);


            // Linha inferior
            npSetLED(getIndex(1, 0), 0, brilho, 0);
            npSetLED(getIndex(2, 0), 0, brilho, 0);
            npSetLED(getIndex(3, 0), 0, brilho, 0);

            // Coluna direita
            npSetLED(getIndex(3, 3), 0, brilho, 0);
            npSetLED(getIndex(3, 2), 0, brilho, 0);
            npSetLED(getIndex(3, 1), 0, brilho, 0);
            npWrite();
            break;
        
    }
}
void sinaliza_limite_excedido(){

    uint32_t brilho = 255;
    npClear();

    if(a == 0){
        //Transforma o 0 em vermelho
        // Linha superior
        npSetLED(getIndex(1, 4), 255, 0, 0);
        npSetLED(getIndex(2, 4), 255, 0, 0);
        npSetLED(getIndex(3, 4), 255, 0, 0);

        // Linhas do meio (lados esquerdo e direito)
        npSetLED(getIndex(1, 3), 255, 0, 0);
        npSetLED(getIndex(3, 3), 255, 0, 0);

        npSetLED(getIndex(1, 2), 255, 0, 0);
        npSetLED(getIndex(3, 2), 255, 0, 0);

        npSetLED(getIndex(1, 1), 255, 0, 0);
        npSetLED(getIndex(3, 1), 255, 0, 0);

        // Linha inferior
        npSetLED(getIndex(1, 0), 255, 0, 0);
        npSetLED(getIndex(2, 0), 255, 0, 0);
        npSetLED(getIndex(3, 0), 255, 0, 0);

        // Atualiza a matriz de LEDs
        npWrite();

        //Faz um beep
        beep(BUZZER_PIN, 2000, 500);

        //Transforma o 0 em verde novamente
        // Linha superior
        npSetLED(getIndex(1, 4), 0, brilho, 0);
        npSetLED(getIndex(2, 4), 0, brilho, 0);
        npSetLED(getIndex(3, 4), 0, brilho, 0);

        // Linhas do meio (lados esquerdo e direito)
        npSetLED(getIndex(1, 3), 0, brilho, 0);
        npSetLED(getIndex(3, 3), 0, brilho, 0);

        npSetLED(getIndex(1, 2), 0, brilho, 0);
        npSetLED(getIndex(3, 2), 0, brilho, 0);

        npSetLED(getIndex(1, 1), 0, brilho, 0);
        npSetLED(getIndex(3, 1), 0, brilho, 0);

        // Linha inferior
        npSetLED(getIndex(1, 0), 0, brilho, 0);
        npSetLED(getIndex(2, 0), 0, brilho, 0);
        npSetLED(getIndex(3, 0), 0, brilho, 0);

        // Atualiza a matriz de LEDs
        npWrite();

    }
    //Se a é igual a 9
    else{
        // Transforma o 0 em vermelho
        // Linha superior
        npSetLED(getIndex(1, 4), 255, 0, 0);
        npSetLED(getIndex(2, 4), 255, 0, 0);
        npSetLED(getIndex(3, 4), 255, 0, 0);

        // Linha do meio
        npSetLED(getIndex(1, 3), 255, 0, 0);
        npSetLED(getIndex(2, 2), 255, 0, 0);
        npSetLED(getIndex(1, 2), 255, 0, 0);
        npSetLED(getIndex(3, 2), 255, 0, 0);

        // Linha inferior
        npSetLED(getIndex(1, 0), 255, 0, 0);
        npSetLED(getIndex(2, 0), 255, 0, 0);
        npSetLED(getIndex(3, 0), 255, 0, 0);

        // Coluna direita
        npSetLED(getIndex(3, 3), 255, 0, 0);
        npSetLED(getIndex(3, 2), 255, 0, 0);
        npSetLED(getIndex(3, 1), 255, 0, 0);

        // Atualiza a matriz de LEDs
        npWrite();

        // Faz um beep
        beep(BUZZER_PIN, 2000, 500);

        // Transforma o 0 em verde novamente
        // Linha superior
        npSetLED(getIndex(1, 4), 0, brilho, 0);
        npSetLED(getIndex(2, 4), 0, brilho, 0);
        npSetLED(getIndex(3, 4), 0, brilho, 0);

        // Linha do meio
        npSetLED(getIndex(1, 3), 0, brilho, 0);
        npSetLED(getIndex(2, 2), 0, brilho, 0);
        npSetLED(getIndex(1, 2), 0, brilho, 0);
        npSetLED(getIndex(3, 2), 0, brilho, 0);

        // Linha inferior
        npSetLED(getIndex(1, 0), 0, brilho, 0);
        npSetLED(getIndex(2, 0), 0, brilho, 0);
        npSetLED(getIndex(3, 0), 0, brilho, 0);

        // Coluna direita
        npSetLED(getIndex(3, 3), 0, brilho, 0);
        npSetLED(getIndex(3, 2), 0, brilho, 0);
        npSetLED(getIndex(3, 1), 0, brilho, 0);

        // Atualiza a matriz de LEDs
        npWrite();

    }



}
