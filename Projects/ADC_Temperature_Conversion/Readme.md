# 🔥 Leitor de Temperatura com BitDogLab e Display OLED

Este projeto utiliza o microcontrolador **Raspberry Pi Pico** para medir a **temperatura interna** do chip e exibi-la em um **display OLED SSD1306** via I2C.

---

## 📷 Visão Geral

O sistema lê a temperatura usando o **sensor interno do RP2040**, converte a leitura para Celsius e atualiza o display OLED com o valor atual.

---

## 🧠 Como Funciona

- O **ADC** lê a tensão correspondente à temperatura interna.
- A tensão é convertida em **graus Celsius** com base na fórmula do datasheet.
- Um buffer gráfico é usado para escrever strings no display.
- O conteúdo do buffer é atualizado a cada 500 ms.

---

## 🔧 Hardware Utilizado

- [Raspberry Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/)
- Display OLED 128x64 (controlador **SSD1306**, comunicação **I2C**)
- BitDogLab

### 🖲 Conexões

| Display OLED | Raspberry Pi Pico |
|--------------|-------------------|
| VCC          | 3V3               |
| GND          | GND               |
| SDA          | GPIO 14           |
| SCL          | GPIO 15           |

---

## 📦 Dependências

- SDK do Raspberry Pi Pico
- Biblioteca `ssd1306_i2c.h` (incluir no projeto)
- Compilador CMake para a Raspberry Pi Pico

---

## 🚀 Compilação e Upload

1. Clone este repositório e configure o ambiente do Pico SDK.
2. Certifique-se de ter a biblioteca `ssd1306_i2c.h` e seus arquivos `.c` incluídos no projeto.
3. Compile com CMake:

```bash
mkdir build
cd build
cmake ..
make
