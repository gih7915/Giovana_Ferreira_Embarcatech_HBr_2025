# 🌟 Luminus Memory

O **Luminus Memory** é um jogo interativo desenvolvido na Fase 1 do curso. Foi inspirado no clássico **Genius (Simon Says)**, onde o jogador precisa memorizar e reproduzir sequências de luzes. 🧠💡

## 🎯 Objetivo

Criar um jogo de memória desafiador e envolvente utilizando:

- 🎧 **Joystick** para navegação  
- 🛡️ **Matriz de LEDs WS2812B** para exibir as sequências  
- 📟 **Display OLED** para feedback e interface
- 🛡️ **Botões** para interação do usuário

## 🛠️ Lista de materiais  

| Componente            | Conexão na BitDogLab      |
|-----------------------|---------------------------|
| BitDogLab (RP2040)    | -                         |
| Matriz WS2812B 5x5    | GPIO7                     |
| Display OLED I2C      | SDA: GPIO14 / SCL: GPIO15 |
| Joystick             | GPIO26 (X) / GPIO27 (Y)  |
| Botão A           | GPIO5  |
| Botão B           | GPIO6  |

## 🚀 Execução  

1. Abra o projeto no **VS Code** com o ambiente configurado para Raspberry Pi Pico (CMake + compilador ARM).  
2. Compile o projeto (Ctrl+Shift+B no VS Code ou via terminal com `cmake` e `make`).  
3. Conecte a **BitDogLab** via **USB** e coloque a **Pico** no modo de boot (`BOOTSEL` + conectar o cabo).  
4. Copie o arquivo **.uf2** gerado para a unidade de armazenamento que aparece (**RPI-RP2**).  
5. A **Pico** reiniciará automaticamente e iniciará o jogo.  

## 🎮 Jogabilidade  

1. **O jogo inicia com uma sequência de LEDs piscando em uma ordem específica.**  
2. O jogador deve memorizar a sequência e, em seguida, **reproduzi-la corretamente** utilizando o **joystick** para selecionar os LEDs e confirmar a escolha com o botão.  
3. Se o jogador acerta a sequência, **um novo LED é adicionado** e o desafio continua. 🔥  
4. Caso erre, o jogo reinicia e o jogador pode tentar novamente. ⏪  

## 📚 Lógica  

- **O jogo começa com uma sequência de um LED piscando aleatoriamente.**  
- **A cada acerto, a sequência cresce** e o jogador precisa repetir desde o início.  
- **O joystick é usado para mover o cursor** sobre a matriz de LEDs e selecionar os LEDs na ordem correta.  
- **O display OLED exibe informações como nível atual e feedbacks de acerto/erro.**  

## 📂 Arquivos  

- [jogo_da_memoria.c](./jogo_da_memoria.c) Código principal do projeto  

## 🖼️​ Imagens do Projeto  

### 🔲 Matriz de LEDs durante a execução  
<img src="https://github.com/user-attachments/assets/f7291866-f3b2-498e-a788-c8c2e605d6ad" width="300">

### 📟 Interface OLED mostrando feedback  
<img src="https://github.com/user-attachments/assets/0e4945ed-b35b-415c-b693-616fe1f8efdc" width="300">

---

Desenvolvido com ❤️ na BitdogLab.
 
