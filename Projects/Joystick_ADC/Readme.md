# 🕹️ Visualizador de Joystick com Display OLED (Raspberry Pi Pico)

Este projeto desenvolvido na **BitdogLab** utiliza o microcontrolador **Raspberry Pi Pico**, um **display OLED SSD1306** e um **joystick analógico** para capturar e exibir em tempo real os valores analógicos dos eixos **X** e **Y** do joystick.

## 📷 Demonstração

> O display mostra continuamente os valores X e Y capturados pelo joystick.

## ⚙️ Componentes Utilizados

- Raspberry Pi Pico
- Display OLED SSD1306 (I2C)
- Joystick analógico

## 🛠️ Ligações

| Componente     | Pino do Pico |
|----------------|--------------|
| Joystick VRX   | GP26 (ADC0)  |
| Joystick VRY   | GP27 (ADC1)  |
| Display SDA    | GP14         |
| Display SCL    | GP15         |

## 📦 Dependências

- [Pico SDK](https://github.com/raspberrypi/pico-sdk)
- Biblioteca `ssd1306` incluída localmente (`inc/ssd1306.h`)

## 🧠 Lógica do Código

1. Inicializa o ADC para leitura dos valores analógicos dos eixos X e Y do joystick.
2. Configura a comunicação I2C com o display OLED.
3. Converte os valores lidos em strings.
4. Renderiza os valores no display utilizando um buffer.

## 💻 Como Compilar

1. Clone este repositório e configure seu ambiente com o Pico SDK.
2. Adicione o caminho da biblioteca `ssd1306` ao seu `CMakeLists.txt`.
3. Compile com `cmake` e `make`.
4. Envie o arquivo `.uf2` gerado para o Raspberry Pi Pico.

## 📋 Comportamento Observado

Valores típicos lidos do joystick em diferentes direções:

| Direção   | Valor X | Valor Y |
|-----------|---------|---------|
| Centro    | 2040    | 1996    |
| Cima      | 2040    | 4090    |
| Baixo     | 2040    |   25    |
| Direita   | 4090    | 1996    |
| Esquerda  |   25    | 1996    |

## 🚀 Melhorias Futuras

- Adicionar um cursor gráfico para representar visualmente a posição do joystick.
- Implementar calibração automática dos valores de centro.

---

Desenvolvido com ❤️ na BitdogLab.

