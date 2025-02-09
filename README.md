# Projeto interface de comunicação serial com RP2040 e I2C 

Esse projeto faz uso do display que tem quase todos os caracteres incluidos, e faz uso da matriz de led que imprime os numeros de 0 a 9, e também o push button que tem como funcionalidade alternar o estado led RGB e informar o estado do led pelo serial monitor e pelo display.

## Hardware 🛠️

- Microcontrolador RP2040 (Raspberry Pi Pico).
- pushbotton.
- SSD1306 OLED DISPLAY.
- Display NeoPixel RGB 5x5.
- LED RGB.
- Placa BitDogLab

## Software 💻

* **SDK do Raspberry Pi Pico:** O SDK (Software Development Kit) do Pico, que inclui as bibliotecas e ferramentas necessárias para desenvolver e compilar o código. [Instruções de instalação](https://www.raspberrypi.com/documentation/pico/getting-started/)
* **CMake:** Um sistema de construção multiplataforma usado para gerar os arquivos de construção do projeto.
* **Compilador C/C++:**  Um compilador C/C++ como o GCC (GNU Compiler Collection).
* **Git:** (Opcional) Para clonar o repositório do projeto.


### O código está dividido em vários arquivos para melhor organização:

- **`Comunicacao_serial.c`**: Código que tem como função gerar quase todos os caracteres no display, gerar animações dos numeros de 0 a 9 na matriz de Led, e com o uso de interrupção alternar o estado do led RGB com push button.
- - **`font.h`:** possibilitar o uso de caracteres no SSD1306 OLED DISPLAY.
- **`CMakeLists.txt`:** Define a estrutura do projeto para o CMake.



## Como Compilar e Executar ⚙️

1. **Instale o SDK do Raspberry Pi Pico:** Siga as instruções no site oficial do Raspberry Pi.
2. **Clone este repositório:** https://github.com/wesiley/I2C_CEPEDI
3. **Navegue até o diretório do projeto:** `cd I2C_CEPEDI`
4. **Compile o projeto:** `cmake -B build && cmake --build build`
5. **Copie para o Pico:** Copie o conteúdo da pasta `build` (gerada após a compilação) para o Raspberry Pi Pico. O programa iniciará automaticamente.


## Funcionamento do Loop Principal 🔄 
```
void handle_char() {
    // Verifica se há caracteres disponíveis na entrada USB
    int receivedChar = getchar();

    if (receivedChar != PICO_ERROR_TIMEOUT) { // Se um caractere foi recebido
        // Exibição do caractere no display
        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, "Caractere: ", 10, 10);
        char buffer[2] = {(char)receivedChar, '\0'};
        ssd1306_draw_string(&ssd, buffer , 90, 10);
        ssd1306_send_data(&ssd);
        printf("Caractere: %c", receivedChar);

        // Exibição de número na matriz WS2812 (se for um número)
        if (receivedChar >= '0' && receivedChar <= '9') {
            int number = receivedChar - '0';
            display_number(number); // Exibe o número na matriz de LEDs
        }
    }
}
while (true) {
        if (stdio_usb_connected()) {
            
            handle_char(); // Verifica se há caracteres recebidos via UART
        }
        sleep_ms(100); // Pequeno delay para evitar sobrecarga
    }
   
  ```
O loop while (true) garante execução contínua. O código lê um caractere e o exibe no display OLED e no terminal. Se for um número (0-9), ele também é exibido na matriz de LEDs WS2812. O loop principal verifica a conexão USB e chama handle_char() a cada 100ms. 

## Funcionamento da interrupção.
```
// Função para tratar interrupções dos botões
void gpio_irq_handler(uint gpio, uint32_t events) {
    static absolute_time_t ultimo_tempo_a = 0;
    static absolute_time_t ultimo_tempo_b = 0;

    if (gpio == BUTTON_A) {
        static absolute_time_t lastInterruptTime = 0;
        absolute_time_t now = get_absolute_time();

        // Debounce
        if (absolute_time_diff_us(lastInterruptTime, now) > 200000) {
            greenLedState = !greenLedState;
            gpio_put(LED_RGB_GREEN, greenLedState);

            // Atualização do display
            ssd1306_fill(&ssd, false);
            ssd1306_draw_string(&ssd, greenLedState ? "Led Verde Ligado" : "Led Verde Desligado", 10, 10);
            ssd1306_send_data(&ssd);

            // Envio de mensagem ao Serial Monitor
            printf(greenLedState ? "Led Verde Ligado\n" : "Led Verde Desligado\n");
        }
        lastInterruptTime = now;
    } else if (gpio == BUTTON_B) {
        static absolute_time_t lastInterruptTime = 0;
        absolute_time_t now = get_absolute_time();

        // Debounce
        if (absolute_time_diff_us(lastInterruptTime, now) > 200000) {
            blueLedState = !blueLedState;
            gpio_put(LED_RGB_BLUE, blueLedState);

            // Atualização do display
            ssd1306_fill(&ssd, false);
            ssd1306_draw_string(&ssd, blueLedState ? "Led Azul Ligado" : "Led Azul Desligado", 10, 10);
            ssd1306_send_data(&ssd);

            // Envio de mensagem ao Serial Monitor
            printf(blueLedState ? "Led Azul Ligado\n" : "Led Azul Desligado\n");
        }
        lastInterruptTime = now;
    }
}
  ```
A função void gpio_irq_handler tem como objetivo fazer com que ocorra as ações no pushbotton . ultimo_tempo_a, ultimo_tempo_b e lastInterruptTime tem como função fazer o debounce para controlar a acção do pushbotton e evita alguns problemas. if(gpio) para dividir a função de A para alternar o estado do led verde e imprimir a informação do estado do led no display e no serial monitor e B  para alternar o estado do azul e imprimir a informação do estado do led no display e no serial monitor. 

## Diagrama de Conexões 💡:



  
 ## 🔗 Link do Vídeo de Funcionamento:


