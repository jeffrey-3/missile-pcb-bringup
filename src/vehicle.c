#include "vehicle.h"

vehicle_t vehicle;

void vehicle_init() {
    vehicle.imu.spi_transfer = board_icm45686_spi_transfer;
    vehicle.flash.spi_transfer = board_w25q128jv_spi_transfer;

    board_init();
    ins_init(&vehicle.ins);
    icm45686_init(&vehicle.imu);
    
    uint8_t read_data[10] = {0};
    uint8_t write_data[3] = {1, 2, 3};

    w25q128jv_read(&vehicle.flash, 1, 85, 10, read_data);

    w25q128jv_write_enable(&vehicle.flash);
    w25q128jv_erase_sector(&vehicle.flash, 0);
    w25q128jv_write_disable(&vehicle.flash);

    w25q128jv_read(&vehicle.flash, 1, 85, 10, read_data);

    w25q128jv_write_enable(&vehicle.flash);
    w25q128jv_write_page(&vehicle.flash, 1, 85, 3, write_data);
    w25q128jv_write_disable(&vehicle.flash);

    w25q128jv_read(&vehicle.flash, 1, 84, 10, read_data);

    float accel[3];
    float gyro[3];
    float gyro_sum[3] = {0.0f, 0.0f, 0.0f};
    float i = 0;
    float roll;
    float pitch;
    float yaw;
    bool on = false;
    uint32_t led_timer = 0;
    uint32_t ins_timer = 0;

    for (;;) {
        if (timer_expired(&led_timer, 500)) {
            gpio_write(board_pins.led, on);
            on = !on;
        }

        if (timer_expired(&ins_timer, 10)) {
            icm45686_read_accel(&vehicle.imu, accel);
            icm45686_read_gyro(&vehicle.imu, gyro);

            gyro[0] = gyro[0] * DEG2RAD - 0.00795f;
            gyro[1] = gyro[1] * DEG2RAD + 0.00295f;
            gyro[2] = gyro[2] * DEG2RAD - 0.00020f;

            i += 1;
            gyro_sum[0] += gyro[0];
            gyro_sum[1] += gyro[1];
            gyro_sum[2] += gyro[2];

            ins_update(&vehicle.ins, gyro[0], gyro[1], gyro[2],
                accel[0], accel[1], accel[2], 0.01f);

            quat_to_euler(vehicle.ins.q, &roll, &pitch, &yaw);

            roll *= RAD2DEG;
            pitch *= RAD2DEG;
            yaw *= RAD2DEG;

            char uart_buf[64];
            snprintf(uart_buf, sizeof(uart_buf),
                "%.0f,%.0f,%.0f,%.1f,%.1f,%.1f\r\n",
                (double)roll, (double)pitch, (double)yaw,
                (double)vehicle.ins.vel.x, (double)vehicle.ins.vel.y,
                (double)vehicle.ins.vel.z);
            uart_write_buf(UART1, uart_buf, strlen(uart_buf));
        }
    }
}
