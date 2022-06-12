#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/spi.h>

void dw_transceive(const struct device *spi, struct spi_config* spi_cfg, uint8_t command, uint8_t rx_buf[], uint8_t rx_len) {
        uint8_t tx_buf[1] = {command};
        const struct spi_buf spi_buf_tx = {
                .buf = tx_buf,
                .len = sizeof(tx_buf)
        };
        struct spi_buf_set tx = {
                .buffers = &spi_buf_tx,
                .count = 1
        };

        struct spi_buf spi_buf_rx[] = {
                {
                        .buf = NULL,
                        .len = sizeof(tx_buf)
                },
                {
                        .buf = rx_buf,
                        .len = rx_len
                }
        };
        struct spi_buf_set rx = {
                .buffers = spi_buf_rx,
                .count = 2
        };
        int status = spi_transceive(spi, spi_cfg, &tx, &rx);
        printk("Status of spi_transceive: %d\n", status);
}

void main(void) {

	printk(" --- RFM69 Radio Transciever ---\n");

	// This application uses pin B9 as the CS pin
	const struct spi_cs_control ctrl = {
    	.gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpiob)),
    	.gpio_pin = 9,
    	.gpio_dt_flags = GPIO_ACTIVE_LOW,
    };

	const struct spi_config cfg = {
		.frequency = 1000000,
		.operation = SPI_OP_MODE_MASTER | SPI_TRANSFER_MSB | SPI_WORD_SET(8),
		.cs = &ctrl,
	};

	// This application uses SPI bus 2, aka cas_spi0
	const struct device *spi_dev = device_get_binding("SPI2");

	if (spi_dev == NULL) {
		printk("Error in main.c: Failed to get device binding.\n");
		return NULL;
	}
	if (!device_is_ready(spi_dev)) {
		printk("Error in main.c: Device is not ready.\n");
		return NULL;
	}

	while (1) {	

		uint8_t recieve_buffer[4] = { 0x00, 0x00, 0x00, 0x00 };

		dw_transceive(spi_dev, &cfg, 0x55, &recieve_buffer, 4);

	}
	
}
