#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include "rfm69.h"
#include "cas_spi.h"

#define NETWORKID 0
#define MYNODEID 0
#define TONODEID 255

#define DEVICE_NODE cas_spi0

uint8_t payload_buffer[PAYLOAD_LENGTH];

void clear_payload_buffer() {
	for (int i=0; i<sizeof(payload_buffer); i++) {
		payload_buffer[i] = 0;
	}
}

void main(void) {

	printk("--- RFM69 Radio Transciever ---\n");

	// This application uses SPI bus 2, aka cas_spi0
	const struct device *spi_dev = DEVICE_DT_GET(DT_NODELABEL(DEVICE_NODE));

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

	if (spi_dev == NULL) {
		printk("Error in main.c: Failed to get device binding.\n");
		return NULL;
	}
	if (!device_is_ready(spi_dev)) {
		printk("Error in main.c: Device is not ready.\n");
		return NULL;
	}

	configure_radio(spi_dev, &cfg);

	while (1) {
		// Put payload data here
		clear_payload_buffer();
		payload_buffer[0] = 0xAA;
		payload_buffer[1] = 0xBB;
		payload_buffer[2] = 0xCC;
		payload_buffer[3] = 0xDD;
		payload_buffer[4] = 0xEE;
		transmit_packet(spi_dev, &cfg, &payload_buffer, NETWORKID, TONODEID);
		printk("sent packet\n");
	}

}