#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/uart.h>
#include <sys/printk.h>
//
// static void myuart_send(struct device *uart, char * szStr) {
//     size_t i;
//     unsigned char temp;
//     for (i = 0; i < strlen(szStr); i++) {
//         temp = szStr[i];
//         uart_poll_out(uart, temp);
//     }
// }
//
// #define myuart_UART "UART_1"
//
// void main(void) {
// 	    struct device * uart;
//
//
// 	    // uart setup
// 	    uart = device_get_binding(myuart_UART);
// 	    if (!uart) {
// 	        printk("Cannot find myuart uart(%s)!\n", myuart_UART);
// 	        return;
// 	    }
//
// 	    printk("Sending AT to UART");
// 	    myuart_send(uart,"AT\r");
// 	    printk("Sleeping 500ms");
// 	    k_usleep(500000);
// 	    printk("Infinite Loop to read the response from uart1");
// 	    while (1) {
// 				unsigned char recvChar;
// 				while (uart_poll_in(uart, &recvChar) == 0) {
// 					printk("%c", recvChar);
// 	    }
// 	}
// }

/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */


static const char *poll_data = "This is a POLL test.\r\n";

static int test_poll_in(void)
{
	unsigned char recv_char;
	const struct device *uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

	if (!device_is_ready(uart_dev)) {
		TC_PRINT("UART device not ready\n");
		return TC_FAIL;
	}

	TC_PRINT("Please send characters to serial console\n");

	/* Verify uart_poll_in() */
	while (1) {
		while (uart_poll_in(uart_dev, &recv_char) < 0) {
			/* Allow other thread/workqueue to work. */
			k_yield();
		}

		TC_PRINT("%c", recv_char);

		if ((recv_char == '\n') || (recv_char == '\r')) {
			break;
		}
	}

	return TC_PASS;
}

static int test_poll_out(void)
{
	int i;
	const struct device *uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

	if (!device_is_ready(uart_dev)) {
		TC_PRINT("UART device not ready\n");
		return TC_FAIL;
	}

	/* Verify uart_poll_out() */
	for (i = 0; i < strlen(poll_data); i++) {
		uart_poll_out(uart_dev, poll_data[i]);
	}

	return TC_PASS;
}

void test_uart_poll_out(void)
{
	zassert_true(test_poll_out() == TC_PASS, NULL);
}

void test_uart_poll_in(void)
{
	zassert_true(test_poll_in() == TC_PASS, NULL);
}
