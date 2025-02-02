# Contador Interativo com Matriz LED - BitDogLab

[![Video Demo](https://img.shields.io/badge/Video_Demo-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://youtu.be/KJeYjjQQT4g)

Projeto desenvolvido para a Unidade 4 do curso, demonstrando o uso de interrupções GPIO e controle de periféricos na placa BitDogLab com RP2040.

## 📋 Descrição
Sistema interativo que combina:
- Matriz LED 5x5 WS2812 para display numérico
- LED RGB para feedback visual
- Botões com tratamento de bouncing
- Buzzer para feedback sonoro

![Demonstração](https://img.youtube.com/vi/KJeYjjQQT4g/0.jpg)

## 🎯 Funcionalidades
- **Contador numérico (0-9)**
  - Incremento com Botão A (GPIO 5)
  - Decremento com Botão B (GPIO 6)
  - Display estilo 7 segmentos na matriz LED
- **Sistema de feedback**
  - LED vermelho piscando 5 vezes por segundo (GPIO 13)
  - Buzzer ativo em operações inválidas
  - Cores dinâmicas na matriz LED:
    - Verde: operação normal
    - Vermelho: limite excedido
- **Técnicas implementadas**
  - Debouncing software
  - Interrupções GPIO
  - Controle PWM para buzzer

## 🛠️ Estrutura do Projeto

| Arquivo            | Descrição                                            |
|--------------------|----------------------------------------------------|
| `buzzer.c`         | Controle das funções do buzzer                     |
| `buzzer.h`         | Declaração das funções relacionadas ao buzzer      |
| `interrupcoes.c`   | Implementação de interrupções GPIO                 |
| `neopixel.c`       | Controle da matriz LED 5x5 WS2812                  |
| `neopixel.h`       | Declaração das funções para controle de LEDs       |
| `ws2818b.pio`      | Código para controle de PIO para LEDs             |
| `diagram.json`     | Arquivo de configuração para simulação no Wokwi   |
| `wokwi.toml`       | Configurações do simulador Wokwi                  |
| `pico_sdk_import.cmake` | Configuração para uso do SDK do RP2040         |
| `CMakeLists.txt`   | Arquivo de build do CMake                          |

## ⚙️ Instalação
1. **Pré-requisitos**
   - Clonar o repositório:
     ```bash
     git clone <URL_DO_REPOSITORIO>
     cd <NOME_DO_REPOSITORIO>
     ```
   - Instalar o **Visual Studio** Code com as seguintes extensões:
     - **Raspberry Pi Pico SDK**
     - **Wokwi**

2. **Compilação**
   - Compile o projeto no terminal:
     ```bash
     mkdir build
     cd build
     cmake ..
     make
     ```

3. **Execução**
   - **Na placa física:** 
     - Conecte a placa ao computador em modo BOOTSEL.
     - Copie o arquivo `.uf2` gerado na pasta `build` para o dispositivo identificado como `RPI-RP2`.
   - **No simulador Wokwi:** 
     - Abra o arquivo `diagram.json` no Wokwi e execute a simulação.

## 🛠️ Hardware
| Componente       | GPIO | Função                  |
|------------------|------|-------------------------|
| Matriz LED 5x5   | 7    | Display numérico        |
| LED Vermelho     | 13   | Pisca continuamente     |
| LED Verde        | 11   | Controle secundário     |
| LED Azul         | 12   | Controle secundário     |
| Botão A          | 5    | Incrementar número      |
| Botão B          | 6    | Decrementar número      |
| Buzzer           | 21   | Feedback sonoro         |

## 🎥 Vídeo Demonstrativo
[![Assista ao vídeo no YouTube](https://img.youtube.com/vi/KJeYjjQQT4g/hqdefault.jpg)](https://www.youtube.com/watch?v=KJeYjjQQT4g)


Caso tenha mais dúvidas ou precise de algo adicional, me avise! 🚀
