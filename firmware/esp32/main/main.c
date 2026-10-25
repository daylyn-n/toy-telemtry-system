#include "driver/uart.h"
#include <stdio.h>

// UART1 wired to the STM32's USART1
#define STM32_UART   UART_NUM_1
#define STM32_TX_PIN 17 // -> STM32 PA10
#define STM32_RX_PIN 18 // <- STM32 PA9
#define LINE_SIZE    128

static void stm32_uart_init(void) {
    const uart_config_t cfg = {
        .baud_rate  = 115200, // must match STM32 BRR
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // rx buffer must be bigger than the 128-byte hardware FIFO
    ESP_ERROR_CHECK(uart_driver_install(STM32_UART, 256, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(STM32_UART, &cfg));
    ESP_ERROR_CHECK(uart_set_pin(STM32_UART, STM32_TX_PIN, STM32_RX_PIN,
                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

void app_main(void) {
    stm32_uart_init();

    uint8_t line[LINE_SIZE];
    size_t  len = 0;

    while (1) {
        uint8_t c;
        // block until one byte arrives from the STM32
        if (uart_read_bytes(STM32_UART, &c, 1, portMAX_DELAY) != 1) {
            continue;
        }

        if (c != '\r' && c != '\n') {
            line[len++] = c;
        }

        // print the stored bytes at end of line (or when the buffer fills)
        if (c == '\n' || len == LINE_SIZE - 1) {
            line[len] = '\0';
            printf("stored %u bytes: \"%s\"\n", (unsigned)len, (char *)line);
            len = 0;
        }
    }
}
