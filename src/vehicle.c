#include "vehicle.h"

static vehicle_t vehicle;

void vehicle_init() {
    vehicle.led_timer = 0;
    vehicle.ins_timer = 0;

    board_init();

    vehicle_imu_init();
    vehicle_flash_init(); 
    vehicle_ins_init();
    vehicle_logger_init();

    vehicle.boot_mode = vehicle_run_cli();
}

void vehicle_update() {
    switch (vehicle.boot_mode) {
        case BOOT_MODE_FLIGHT:
            vehicle_update_flight();
            break;
        case BOOT_MODE_CALIBRATE:
            vehicle_update_calibrate();
            break;
        case BOOT_MODE_RETREIVE:
            vehicle_update_retreive();
            break;
        case BOOT_MODE_ERASE:
            vehicle_update_erase();
            break;
    }
}

void vehicle_update_flight() {
    if (timer_expired(&vehicle.led_timer, 500)) {
        gpio_write(board_pins.led, vehicle.led_on);
        vehicle.led_on = !vehicle.led_on;
    }

    if (timer_expired(&vehicle.ins_timer, 10)) {
        float accel[3];
        float gyro[3];
        icm45686_read_accel(&vehicle.imu, accel);
        icm45686_read_gyro(&vehicle.imu, gyro);

        gyro[0] = gyro[0] * DEG2RAD - 0.00795f;
        gyro[1] = gyro[1] * DEG2RAD + 0.00295f;
        gyro[2] = gyro[2] * DEG2RAD - 0.00020f;

        ins_update(&vehicle.ins, gyro[0], gyro[1], gyro[2],
            accel[0], accel[1], accel[2], 0.01f);

        float roll;
        float pitch;
        float yaw;
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

void vehicle_update_calibrate() {
    if (timer_expired(&vehicle.led_timer, 500)) {
        gpio_write(board_pins.led, vehicle.led_on);
        vehicle.led_on = !vehicle.led_on;
    }
}

void vehicle_update_retreive() {
    if (timer_expired(&vehicle.led_timer, 500)) {
        gpio_write(board_pins.led, vehicle.led_on);
        vehicle.led_on = !vehicle.led_on;
    }
}

void vehicle_update_erase() {
    if (timer_expired(&vehicle.led_timer, 500)) {
        gpio_write(board_pins.led, vehicle.led_on);
        vehicle.led_on = !vehicle.led_on;
    }
}

boot_mode_t vehicle_run_cli() {
    char uart_buf[100] = "Missile Command Line Interface\r\n(1) Flight\r\n"
       "(2) Calibrate\r\n(3) Retreive\r\n(9) Erase\r\n";
    uart_write_buf(UART1, uart_buf, strlen(uart_buf));

    for (;;) {
        static char cmd_buf[CMD_BUF_LEN];
        static uint8_t idx = 0;

        while (!uart_read_ready(UART1)) {
            spin(1);
        }

        char c = uart_read_byte(UART1);

        if (c == '\r' || c == '\n') {
            cmd_buf[idx] = '\0';
            idx = 0;

            if (strcmp(cmd_buf, "1") == 0) {
                uart_write_buf(UART1, "Flight Selected\r\n", 17);
                return BOOT_MODE_FLIGHT;
            } else if (strcmp(cmd_buf, "2") == 0) {
                uart_write_buf(UART1, "Calibrate Selected\r\n", 20);
                return BOOT_MODE_CALIBRATE;
            } else if (strcmp(cmd_buf, "3") == 0) {
                uart_write_buf(UART1, "Retrieve Selected\r\n", 19);
                return BOOT_MODE_RETREIVE;
            } else if (strcmp(cmd_buf, "9") == 0) {
                uart_write_buf(UART1, "Erase Selected\r\n", 16);
                return BOOT_MODE_ERASE;
            } else {
                uart_write_buf(UART1, "Invalid\r\n", 9);
            }

            continue;
        }

        if (idx < (CMD_BUF_LEN - 1)) {
            cmd_buf[idx++] = c;
        } else {
            idx = 0;
        }
    }

}

void vehicle_imu_init() {
    vehicle.imu.spi_transfer = board_icm45686_spi_transfer;
    icm45686_init(&vehicle.imu);
}

void vehicle_flash_init() {
    vehicle.flash.spi_transfer = board_w25q128jv_spi_transfer;
}

void vehicle_ins_init() {
    ins_init(&vehicle.ins);
}

void vehicle_logger_init() {
    uint8_t messages_per_page = 3;
    message_t logger_buffer[messages_per_page];

    vehicle.logger.write_page = vehicle_logger_write_page;
    vehicle.logger.write_enable = vehicle_logger_write_enable;
    vehicle.logger.write_disable = vehicle_logger_write_disable;
    vehicle.logger.delay_ms = delay;
    vehicle.logger.buffer = logger_buffer;
    vehicle.logger.messages_per_page = messages_per_page;
    vehicle.logger.sector_erase_time = 450;
    vehicle.logger.write_enable_time = 5;

    // logger_init(&vehicle.logger);
}

void vehicle_logger_write_page(uint32_t page, uint8_t *data) {
    w25q128jv_write_page(&vehicle.flash, page, 0, 256, data);
}

void vehicle_logger_erase_sector(uint16_t sector) {
    w25q128jv_erase_sector(&vehicle.flash, sector);
}

void vehicle_logger_write_enable() {
    w25q128jv_write_enable(&vehicle.flash);
}

void vehicle_logger_write_disable() {
    w25q128jv_write_disable(&vehicle.flash);
}
