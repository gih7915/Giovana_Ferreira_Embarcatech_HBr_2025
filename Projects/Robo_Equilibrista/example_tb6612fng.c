#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "motor.h"
#include "mpu6050_i2c.h"

// ======================= CONFIGURAÇÕES =======================
#define LOOP_DT         0.005f      // 5ms = 200Hz
#define COMPLEMENTARY_A 0.98f       // peso do giroscópio no filtro complementar
#define DEADZONE        5           // PWM mínimo para vencer atrito (0-255)
#define ACCEL_SENS_2G  16384.0f
#define GYRO_SENS_250   131.0f

// PID - ajustar conforme necessidade
float Kp = 100.0f;
float Ki = 1.0f;
float Kd = 1.0f;

float setpoint = 0.0f; // ângulo alvo (0 = em pé)

// ======================= VARIÁVEIS GLOBAIS ==================
float angle = 0.0f;
float integral = 0.0f;
float last_error = 0.0f;

// ======================= FUNÇÕES ============================

// Calcula o ângulo de pitch a partir do acelerômetro (em graus)
float get_pitch_from_accel(float ax, float az) {
    return atan2f(ax, az) * 180.0f / M_PI;
}

// Loop principal de controle PID e acionamento do motor
void control_loop() {
    int16_t accel_raw[3], gyro_raw[3], temp_raw;
    float accel[3], gyro[3];

    // Ler dados brutos do MPU6050
    mpu6050_read_raw(accel_raw, gyro_raw, &temp_raw);

    // Converter dados para valores físicos (g e deg/s)
    accel[0] = accel_raw[0] / ACCEL_SENS_2G;
    accel[1] = accel_raw[1] / ACCEL_SENS_2G;
    accel[2] = accel_raw[2] / ACCEL_SENS_2G;

    gyro[0] = gyro_raw[0] / GYRO_SENS_250;
    gyro[1] = gyro_raw[1] / GYRO_SENS_250;
    gyro[2] = gyro_raw[2] / GYRO_SENS_250;

    // Inicializa o ângulo com leitura do acelerômetro na primeira execução
    static bool first_run = true;
    if (first_run) {
        angle = get_pitch_from_accel(accel[0], accel[2]);
        first_run = false;
    }

    // Calcula ângulo usando filtro complementar (acel + giroscópio)
    float accel_angle = get_pitch_from_accel(accel[0], accel[2]);
    angle = COMPLEMENTARY_A * (angle + gyro[1] * LOOP_DT) +
            (1.0f - COMPLEMENTARY_A) * accel_angle;

    // Calcula erro do PID
    float error = setpoint - angle;

    integral += error * LOOP_DT;
    // Limita integral para evitar windup
    if (integral > 100.0f) integral = 100.0f;
    if (integral < -100.0f) integral = -100.0f;

    float derivative = (error - last_error) / LOOP_DT;
    last_error = error;

    // Calcula saída do PID
    float output = (Kp * error) + (Ki * integral) + (Kd * derivative);

    // Ajusta PWM mínimo para vencer atrito e controla sentido do motor
    bool forward = output > 0;
    float abs_output = fabsf(output);

    const uint16_t PWM_MAX = 255;
    const uint16_t PWM_MIN = DEADZONE; // aqui DEADZONE = 5

    uint16_t pwm_val;
    if (abs_output > 0 && abs_output < PWM_MIN) {
        pwm_val = PWM_MIN;
    } else {
        pwm_val = (uint16_t)fminf(abs_output, PWM_MAX);
    }

    pwm_val <<= 8; // escala para 16 bits do driver

    motor_set_both_level(pwm_val, forward);
}

// ======================= MAIN ===============================
int main() {
    stdio_init_all();

    // Inicializa I2C e sensor MPU6050
    mpu6050_setup_i2c();
    mpu6050_reset();

    // Inicializa motores
    motor_setup();
    motor_enable();

    sleep_ms(1000); // tempo para estabilizar sensores e motores

    while (true) {
        control_loop();
        sleep_us((int)(LOOP_DT * 1e6)); // espera precisa de 5 ms
    }

    return 0;
}
