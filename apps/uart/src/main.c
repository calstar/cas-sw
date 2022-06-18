/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/uart.h>
#include <string.h>

/* You can edit these configuration parameters */
#define BAUDRATE 115200
#define MESSAGE "Hello Avionics \n"

#define UART_NODE DT_NODELABEL(usart1)
#if DT_NODE_HAS_STATUS(UART_NODE, disabled)
#error "Error in defining devicetree node for uart"
#endif

void main(void) {

	const struct device *uart_dev = DEVICE_DT_GET(UART_NODE);

	if (!device_is_ready(uart_dev)) {
		printk("Error: device 'uart' is not ready.");
        return;
    }

    struct uart_config config;
    config.baudrate = BAUDRATE;
    config.parity = UART_CFG_PARITY_NONE;
    config.stop_bits = UART_CFG_STOP_BITS_1;
    config.data_bits = UART_CFG_DATA_BITS_8;
    config.flow_ctrl = UART_CFG_FLOW_CTRL_NONE;

    if (uart_configure(uart_dev, &config) != 0) {
    	printk("Error in configuring device 'uart'.");
        return;
    }

    char* message = MESSAGE;

    while (1) {

        for (int i = 0; i < strlen(message); i++) {
         	uart_poll_out(uart_dev, message[i]);
        }

    }

    // Optional: test UART input
    // 
    // char* next_char;
    // int status;
    //
    // while (1) {
    //   status = uart_poll_in(uart_dev, next_char);
    //	 if (status == 0) { printk(next_char); }
    // }

}
