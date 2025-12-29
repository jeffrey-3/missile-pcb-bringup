#include "board.h"
#include "math.h"
#include "icm45686.h"
#include "quaternion.h"
#include "ins.h"

void icm45686_spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len) {
    gpio_write(board_pins.cs, false);
    spi_transfer_buf(SPI1, tx_buf, rx_buf, len);
    gpio_write(board_pins.cs, true);
}

int main(void) {
    systick_init();

    board_init();

    ins_t ins;
    ins_init(&ins);

    icm45686_t imu = {
        .spi_transfer = icm45686_spi_transfer
    };
    icm45686_init(&imu);

    float accel[3];
    float gyro[3];
    float roll;
    float pitch;
    float yaw;
    uint32_t led_timer = 0;
    uint32_t ins_timer = 0;
    float i = 0;
    float gyro_sum[3] = {0.0f, 0.0f, 0.0f};

    for (;;) {
        if (timer_expired(&led_timer, 500)) {
            static bool on;
            gpio_write(board_pins.led, on);
            on = !on;
        }

        if (timer_expired(&ins_timer, 10)) {
            icm45686_read_accel(&imu, accel);
            icm45686_read_gyro(&imu, gyro);

            gyro[0] = gyro[0] * DEG2RAD - 0.00795f;
            gyro[1] = gyro[1] * DEG2RAD + 0.00295f;
            gyro[2] = gyro[2] * DEG2RAD - 0.00020f;

            i += 1;
            gyro_sum[0] += gyro[0];
            gyro_sum[1] += gyro[1];
            gyro_sum[2] += gyro[2];

            ins_update(&ins, gyro[0], gyro[1], gyro[2], accel[0], accel[1],
                accel[2], 0.01f);

            quat_to_euler(ins.q, &roll, &pitch, &yaw);

            roll *= RAD2DEG;
            pitch *= RAD2DEG;
            yaw *= RAD2DEG;

            char uart_buf[64];
            snprintf(uart_buf, sizeof(uart_buf),
                "%.0f %.0f %.0f %.1f %.1f %.1f\r\n",
                (double)roll, (double)pitch, (double)yaw,
                (double)ins.vel.x, (double)ins.vel.y, (double)ins.vel.z);
            uart_write_buf(UART1, uart_buf, strlen(uart_buf));
        }
    }

    return 0;
}
