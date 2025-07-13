#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/mpu6050_i2c.h" // ou um driver customizado do MPU-6500


#define I2C_PORT i2c0
#define I2C_SDA 0
#define I2C_SCL 1

/*
#define I2C_OLED i2c1
#define SDA_OLED 14
#define SCL_OLED 15
*/

int main() {
    stdio_init_all();           // Inicializa USB serial
    mpu6050_setup_i2c();       // Configura barramento I2C
    mpu6050_reset();           // Reinicia o sensor

    int16_t accel[3], gyro[3], temp;

    while (1) {
        mpu6050_read_raw(accel, gyro, &temp); // Armazena valores lidos nas variaveis

        printf("Accel X: %d, Y: %d, Z: %d | Temp: %.2f °C\n",accel[0], accel[1], accel[2],(temp / 340.0) + 36.53);

        sleep_ms(1000); // espera 1 segundo
    }
}
