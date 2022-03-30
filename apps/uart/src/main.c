#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/uart.h>

static void myuart_send(struct device *uart, char * szStr) {
    size_t i;
    unsigned char temp;
    for (i = 0; i < strlen(szStr); i++) {
        temp = szStr[i];
        uart_poll_out(uart, temp);
    }
}

#define myuart_UART "UART_1"

void main(void) {
	    struct device * uart;


	    // uart setup
	    uart = device_get_binding(myuart_UART);
	    if (!uart) {
	        printk("Cannot find myuart uart(%s)!\n", myuart_UART);
	        return;
	    }

	    printk("Sending AT to UART");
	    myuart_send(uart,"AT\r");
	    printk("Sleeping 500ms");
	    k_usleep(500000);
	    printk("Infinite Loop to read the response from uart1");
	    while (1) {
				unsigned char recvChar;
				while (uart_poll_in(uart, &recvChar) == 0) {
					printk("%c", recvChar);
	    }
	}
}
