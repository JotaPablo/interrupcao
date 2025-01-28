#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Definição do pino do buzzer
#define BUZZER_PIN 21

// Funções para controle do buzzer
void pwm_init_buzzer(uint pin);
void beep(uint pin, uint frequency, uint duration_ms);

#endif // BUZZER_H
