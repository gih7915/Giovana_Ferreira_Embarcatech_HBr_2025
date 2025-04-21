# 🌡️ Monitor de Temperatura com Display OLED (Raspberry Pi Pico)

Este projeto desenvolvido na **BitdogLab** utiliza o **sensor de temperatura interno** do **Raspberry Pi Pico** e um **display OLED SSD1306** para exibir em tempo real a temperatura do chip em graus Celsius.

## 📷 Demonstração

> O display mostra continuamente a temperatura lida do sensor interno do Raspberry Pi Pico.

## ⚙️ Componentes Utilizados

- Raspberry Pi Pico
- Display OLED SSD1306 (I2C)

## 🛠️ Ligações

| Componente     | Pino do Pico |
|----------------|--------------|
| Display SDA    | GP14         |
| Display SCL    | GP15         |

> O sensor de temperatura é interno ao chip RP2040, portanto não exige conexão física.

## 📦 Dependências

- [Pico SDK](https://github.com/raspberrypi/pico-sdk)
- Biblioteca `ssd1306` incluída localmente (`inc/ssd1306.h`)

## 🧠 Lógica do Código

1. Inicializa a comunicação I2C e o display OLED.
2. Habilita o sensor de temperatura interno.
3. Realiza leituras ADC do canal do sensor.
4. Converte o valor lido (RAW) em temperatura Celsius com a seguinte fórmula:
5. Exibe o valor no display usando a biblioteca SSD1306.

## 💻 Como Compilar

1. Clone este repositório e configure seu ambiente com o Pico SDK.
2. Adicione o caminho da biblioteca `ssd1306` ao seu `CMakeLists.txt`.
3. Compile com `cmake` e `make`.
4. Envie o arquivo `.uf2` gerado para o Raspberry Pi Pico.
float temperature_celsius = 27.0f - (voltage - 0.706f) / 0.001721f;

## 📋 Observações

- CONVERSION_FACTOR é utilizado para transformar a leitura bruta (RAW) do ADC em tensão.
- A temperatura pode variar ligeiramente dependendo da tensão de alimentação e temperatura ambiente.
- O projeto é útil para visualizar a temperatura do chip durante o funcionamento do código.

## 🚀 Melhorias Futuras

- Exibir um gráfico com a variação de temperatura ao longo do tempo.
- Implementar alarmes para temperaturas muito altas.
- Registrar dados em cartão SD ou enviar via comunicação serial.

---

Desenvolvido com ❤️ na BitdogLab.
