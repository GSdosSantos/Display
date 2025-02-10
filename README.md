# Projeto de Comunicação Serial e Controle de LEDs com RP2040

Este projeto foi desenvolvido como parte de uma atividade prática para consolidar os conceitos de comunicação serial, controle de LEDs e uso de interrupções com o microcontrolador RP2040 na placa BitDogLab. O projeto combina hardware e software para criar uma aplicação interativa que utiliza LEDs WS2812, um display SSD1306, LEDs RGB e botões.

## Descrição do Projeto

O projeto tem como objetivo demonstrar o uso de interfaces de comunicação serial (UART e I2C) e o controle de diferentes tipos de LEDs (comuns e endereçáveis WS2812). Além disso, o projeto utiliza interrupções para tratar eventos de botões e implementa técnicas de debouncing para garantir a confiabilidade das entradas.

### Componentes Utilizados

- **Matriz 5x5 de LEDs WS2812**: Conectada à GPIO 7.
- **LED RGB**: Conectado às GPIOs 11 (Verde), 12 (Azul) e 13 (Vermelho).
- **Botão A**: Conectado à GPIO 5.
- **Botão B**: Conectado à GPIO 6.
- **Display SSD1306**: Conectado via I2C (GPIO 14 e GPIO 15).

### Funcionalidades

1. **Modificação da Biblioteca `font.h`**:
   - Foram adicionados caracteres minúsculos à biblioteca `font.h` para exibição no display SSD1306.

2. **Entrada de Caracteres via PC**:
   - Caracteres digitados no Serial Monitor do VS Code são exibidos no display SSD1306.
   - Quando um número entre 0 e 9 é digitado, um símbolo correspondente é exibido na matriz 5x5 de LEDs WS2812.

3. **Interação com o Botão A**:
   - Pressionar o botão A alterna o estado do LED RGB Verde.
   - O estado do LED é exibido no display SSD1306 e enviado ao Serial Monitor.

4. **Interação com o Botão B**:
   - Pressionar o botão B alterna o estado do LED RGB Azul.
   - O estado do LED é exibido no display SSD1306 e enviado ao Serial Monitor.

### Requisitos do Projeto

- **Uso de Interrupções**: Todas as funcionalidades relacionadas aos botões são implementadas utilizando rotinas de interrupção (IRQ).
- **Debouncing**: O tratamento do bouncing dos botões é feito via software.
- **Controle de LEDs**: O projeto inclui o controle de LEDs comuns e LEDs WS2812.
- **Utilização do Display SSD1306**: O display é utilizado para exibir mensagens informativas e caracteres digitados pelo usuário.
- **Envio de Informação pela UART**: A comunicação serial via UART é utilizada para enviar informações ao Serial Monitor.
- **Organização do Código**: O código está bem estruturado e comentado para facilitar o entendimento.

## Estrutura do Código

O código está organizado em várias funções principais:

- **Inicialização**:
  - `inicializar_i2c()`: Configura a comunicação I2C para o display SSD1306.
  - `inicializar_display()`: Inicializa o display SSD1306.
  - `inicializar_ws2812()`: Configura os LEDs WS2812.
  - `inicializar_botoes()`: Configura os botões com interrupções.
  - `inicializar_leds()`: Configura os LEDs RGB.

- **Manipulação de LEDs**:
  - `set_number(int num)`: Exibe um número na matriz de LEDs WS2812.
  - `put_pixel(uint32_t pixel_grb)`: Envia um pixel para a matriz de LEDs WS2812.
  - `urgb_u32(uint8_t r, uint8_t g, uint8_t b)`: Converte valores RGB para um formato de 32 bits.

- **Tratamento de Interrupções**:
  - `gpio_irq_handler(uint gpio, uint32_t eventos)`: Manipula as interrupções dos botões A e B.

- **Entrada de Dados**:
  - `tratar_usb()`: Lê caracteres da entrada USB e os exibe no display e na matriz de LEDs.
  - `mostrar_aleatorio()`: Exibe caracteres aleatórios no display e na matriz de LEDs.

## Como Executar o Projeto

1. **Compilação**:
   - Utilize o ambiente de desenvolvimento do Pico SDK para compilar o código.
   - Certifique-se de que todas as bibliotecas necessárias estão instaladas e configuradas corretamente.

2. **Upload**:
   - Conecte a placa BitDogLab ao computador via USB.
   - Carregue o código compilado na placa.

3. **Execução**:
   - Abra o Serial Monitor no VS Code para interagir com o projeto.
   - Digite caracteres para exibi-los no display e na matriz de LEDs.
   - Pressione os botões A e B para alternar os estados dos LEDs RGB Verde e Azul.

## Conclusão

Este projeto demonstra a integração de diferentes componentes e técnicas de programação para criar uma aplicação interativa com o RP2040. Através da manipulação de LEDs, uso de interrupções e comunicação serial, o projeto oferece uma visão prática do desenvolvimento de sistemas embarcados.

## Autor

- **Nome**: Gabriel Santos dos Santos
- **Data**: 09/02/2025
