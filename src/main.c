#include "hal.h"
#include "icm45686.h"
#include "quaternion.h"
#include "ins.h"

static void icm45686_spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len) {
    uint16_t cs = PIN('A', 1);

    gpio_write(cs, false);
    spi_transfer_buf(SPI1, tx_buf, rx_buf, len);
    gpio_write(cs, true);
}

void test_uart() {
    uint16_t tx = PIN('B', 6);
    gpio_set_mode(tx, GPIO_MODE_AF);
    gpio_set_af(tx, 0);

    uint16_t rx = PIN('B', 7);
    gpio_set_mode(rx, GPIO_MODE_AF);
    gpio_set_af(rx, 0);

    uart_init(UART1, 115200);

    uint16_t led = PIN('B', 9);
    gpio_set_mode(led, GPIO_MODE_OUTPUT);

    for (;;) {
        if (timer_expired(200)) {
            static bool on;
            gpio_write(led, on);
            on = !on;
            uart_write_buf(UART1, "hi\r\n", 4);
        }
    }
}

void test_spi() {
    // UART
    uint16_t tx = PIN('B', 6);
    gpio_set_mode(tx, GPIO_MODE_AF);
    gpio_set_af(tx, 0);

    uint16_t rx = PIN('B', 7);
    gpio_set_mode(rx, GPIO_MODE_AF);
    gpio_set_af(rx, 0);

    uart_init(UART1, 115200);

    // LED
    uint16_t led = PIN('B', 9);
    gpio_set_mode(led, GPIO_MODE_OUTPUT);

    // SPI
    uint16_t cs = PIN('A', 1);
    gpio_set_mode(cs, GPIO_MODE_OUTPUT);
    gpio_write(cs, true);

    uint16_t miso = PIN('A', 6);
    gpio_set_mode(miso, GPIO_MODE_AF);
    gpio_set_af(miso, 0);

    uint16_t mosi = PIN('A', 12);
    gpio_set_mode(mosi, GPIO_MODE_AF);
    gpio_set_af(mosi, 0);

    uint16_t sck = PIN('A', 5);
    gpio_set_mode(sck, GPIO_MODE_AF);
    gpio_set_af(sck, 0);

    spi_init(SPI1);

    for (;;) {
        if (timer_expired(1000)) {
            static bool on;
            gpio_write(led, on);
            on = !on;

            uint8_t tx_buf[2] = {0x72 | 0x80, 0x00};
            uint8_t rx_buf[2];

            gpio_write(cs, false);
            spi_transfer_buf(SPI1, tx_buf, rx_buf, 2);
            gpio_write(cs, true);

            char uart_buf[32];
            snprintf(uart_buf, sizeof(uart_buf), "WHO_AM_I: %d\r\n", rx_buf[1]);
            uart_write_buf(UART1, uart_buf, strlen(uart_buf));
        }
    }
}

void test_imu() {
    // UART
    uint16_t tx = PIN('B', 6);
    gpio_set_mode(tx, GPIO_MODE_AF);
    gpio_set_af(tx, 0);

    uint16_t rx = PIN('B', 7);
    gpio_set_mode(rx, GPIO_MODE_AF);
    gpio_set_af(rx, 0);

    uart_init(UART1, 115200);

    // LED
    uint16_t led = PIN('B', 9);
    gpio_set_mode(led, GPIO_MODE_OUTPUT);

    // SPI
    uint16_t cs = PIN('A', 1);
    gpio_set_mode(cs, GPIO_MODE_OUTPUT);
    gpio_write(cs, true);

    uint16_t miso = PIN('A', 6);
    gpio_set_mode(miso, GPIO_MODE_AF);
    gpio_set_af(miso, 0);

    uint16_t mosi = PIN('A', 12);
    gpio_set_mode(mosi, GPIO_MODE_AF);
    gpio_set_af(mosi, 0);

    uint16_t sck = PIN('A', 5);
    gpio_set_mode(sck, GPIO_MODE_AF);
    gpio_set_af(sck, 0);

    spi_init(SPI1);

    // IMU
    icm45686_t imu = {
        .spi_transfer = icm45686_spi_transfer
    };

    uint8_t id = icm45686_read_id(&imu);

    icm45686_init(&imu);

    float accel[3];
    float gyro[3];
    for (;;) {
        if (timer_expired(10)) {
            // Toggle LED
            static bool on;
            gpio_write(led, on);
            on = !on;

            // Read IMU
            icm45686_read_accel(&imu, accel);
            icm45686_read_gyro(&imu, gyro);

            // UART
            char uart_buf[100];
            snprintf(uart_buf, sizeof(uart_buf),
                "%d %ld %ld %ld %ld %ld %ld\r\n", id, (int32_t)(accel[0] * 100),
                (int32_t)(accel[1] * 100), (int32_t)(accel[2] * 100),
                (int32_t)gyro[0], (int32_t)gyro[1], (int32_t)gyro[2]);
            uart_write_buf(UART1, uart_buf, strlen(uart_buf));
        }
    }
}

void test_quat() {
    // UART
    uint16_t tx = PIN('B', 6);
    gpio_set_mode(tx, GPIO_MODE_AF);
    gpio_set_af(tx, 0);

    uint16_t rx = PIN('B', 7);
    gpio_set_mode(rx, GPIO_MODE_AF);
    gpio_set_af(rx, 0);

    uart_init(UART1, 115200);

    // SPI
    uint16_t cs = PIN('A', 1);
    gpio_set_mode(cs, GPIO_MODE_OUTPUT);
    gpio_write(cs, true);

    uint16_t miso = PIN('A', 6);
    gpio_set_mode(miso, GPIO_MODE_AF);
    gpio_set_af(miso, 0);

    uint16_t mosi = PIN('A', 12);
    gpio_set_mode(mosi, GPIO_MODE_AF);
    gpio_set_af(mosi, 0);

    uint16_t sck = PIN('A', 5);
    gpio_set_mode(sck, GPIO_MODE_AF);
    gpio_set_af(sck, 0);

    spi_init(SPI1);

    // IMU
    icm45686_t imu = {
        .spi_transfer = icm45686_spi_transfer
    };
    icm45686_init(&imu);

    float accel[3];
    float gyro[3];
    quat_t q = quat_identity();
    for (;;) {
        if (timer_expired(20)) {
            // Read IMU
            icm45686_read_accel(&imu, accel);
            icm45686_read_gyro(&imu, gyro);

            gyro[0] *= 3.14159f / 180.0f;
            gyro[1] *= 3.14159f / 180.0f;
            gyro[2] *= 3.14159f / 180.0f;

            quat_update_gyro(&q, gyro[0], gyro[1], gyro[2], 0.02f);

            float roll, pitch, yaw;
            quat_to_euler(q, &roll, &pitch, &yaw);
            roll *= 180.0f / 3.14159f;
            pitch *= 180.0f / 3.14159f;
            yaw *= 180.0f / 3.14159f;

            // UART
            char uart_buf[32];
            snprintf(uart_buf, sizeof(uart_buf), "%ld %ld %ld\r\n",
                (int32_t)roll, (int32_t)pitch, (int32_t)yaw);
            uart_write_buf(UART1, uart_buf, strlen(uart_buf));
        }
    }
}

void test_ins() {
    // UART
    uint16_t tx = PIN('B', 6);
    gpio_set_mode(tx, GPIO_MODE_AF);
    gpio_set_af(tx, 0);

    uint16_t rx = PIN('B', 7);
    gpio_set_mode(rx, GPIO_MODE_AF);
    gpio_set_af(rx, 0);

    uart_init(UART1, 115200);

    // SPI
    uint16_t cs = PIN('A', 1);
    gpio_set_mode(cs, GPIO_MODE_OUTPUT);
    gpio_write(cs, true);

    uint16_t miso = PIN('A', 6);
    gpio_set_mode(miso, GPIO_MODE_AF);
    gpio_set_af(miso, 0);

    uint16_t mosi = PIN('A', 12);
    gpio_set_mode(mosi, GPIO_MODE_AF);
    gpio_set_af(mosi, 0);

    uint16_t sck = PIN('A', 5);
    gpio_set_mode(sck, GPIO_MODE_AF);
    gpio_set_af(sck, 0);

    spi_init(SPI1);

    // INS
    ins_t ins;
    ins_init(&ins);

    // IMU
    icm45686_t imu = {
        .spi_transfer = icm45686_spi_transfer
    };
    icm45686_init(&imu);

    float accel[3];
    float gyro[3];
    for (;;) {
        if (timer_expired(20)) {
            // Read IMU
            icm45686_read_accel(&imu, accel);
            icm45686_read_gyro(&imu, gyro);

            gyro[0] *= 3.14159f / 180.0f;
            gyro[1] *= 3.14159f / 180.0f;
            gyro[2] *= 3.14159f / 180.0f;

            ins_update(&ins, gyro[0], gyro[1], gyro[2], accel[0], accel[1],
                accel[2], 0.02f);

            float roll, pitch, yaw;
            quat_to_euler(ins.q, &roll, &pitch, &yaw);
            roll *= 180.0f / 3.14159f;
            pitch *= 180.0f / 3.14159f;
            yaw *= 180.0f / 3.14159f;

            // UART
            char uart_buf[32];
            snprintf(uart_buf, sizeof(uart_buf), "%ld %ld %ld\r\n",
                (int32_t)roll, (int32_t)pitch, (int32_t)yaw);
            uart_write_buf(UART1, uart_buf, strlen(uart_buf));
        }
    }
}

int main(void) {
    systick_init();

    // test_uart();
    // test_spi();
    // test_imu();
    // test_quat();
    test_ins();
    
    return 0;
}
