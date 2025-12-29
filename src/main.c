#include "board.h"
#include "icm45686.h"
#include "quaternion.h"
#include "ins.h"

static void icm45686_spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf,
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

    uint32_t led_timer = 0;
    uint32_t ins_timer = 0;

    float accel[3];
    float gyro[3];
    float roll;
    float pitch;
    float yaw;

    for (;;) {
        if (timer_expired(&led_timer, 500)) {
            static bool on;
            gpio_write(board_pins.led, on);
            on = !on;
        }

        if (timer_expired(&ins_timer, 20)) {
            icm45686_read_accel(&imu, accel);
            icm45686_read_gyro(&imu, gyro);

            gyro[0] *= 3.14159f / 180.0f;
            gyro[1] *= 3.14159f / 180.0f;
            gyro[2] *= 3.14159f / 180.0f;

            ins_update(&ins, gyro[0], gyro[1], gyro[2], accel[0], accel[1],
                accel[2], 0.02f);

            quat_to_euler(ins.q, &roll, &pitch, &yaw);
            roll *= 180.0f / 3.14159f;
            pitch *= 180.0f / 3.14159f;
            yaw *= 180.0f / 3.14159f;

            char uart_buf[32];
            snprintf(uart_buf, sizeof(uart_buf), "%ld %ld %ld\r\n",
                (int32_t)roll, (int32_t)pitch, (int32_t)yaw);
            uart_write_buf(UART1, uart_buf, strlen(uart_buf));
        }
    }

    return 0;
}
