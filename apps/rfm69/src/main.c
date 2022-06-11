#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include "rfm69.h"
#include "cas_spi.h"

// This application uses spi bus number 2, aka cas_spi0
#define SPI_NODE cas_spi0
// The CS pin on the microcontroller for this SPI bus is GPIO pin B9
#define CS_BANK gpiob
#define CS_PIN 9

// Usually, this should be either 433 MHz or 915 MHz
#define FREQUENCY RF69_915MHZ

#define NETWORKID 0
#define MYNODEID 0
#define TONODEID 255

uint8_t send_buffer[PAYLOAD_LENGTH];

void clear_send_buffer() {
	for (int i=0; i<PAYLOAD_LENGTH; i++) {
		send_buffer[i] = 0;
	}
}

void main(void) {

	printk(" --- RFM69 Radio Transciever ---\n");

	struct device *dev = DEVICE_DT_GET(DT_NODELABEL(SPI_NODE));

	if (dev == NULL) {
		printk("Error in main.c: Failed to get device binding.\n");
		return NULL;
	}
	if (!device_is_ready(dev)) {
		printk("Error in main.c: Device is not ready.\n");
		return NULL;
	}

	struct device *cs_gpio_device = DEVICE_DT_GET(DT_NODELABEL(CS_BANK));
	struct spi_config *cfg = get_spi_config(cs_gpio_device, CS_PIN);

	clear_send_buffer();
	send_buffer[0] = 0xDE;
	send_buffer[1] = 0xAD;
	send_buffer[2] = 0xBE;
	send_buffer[3] = 0xEF;

	// Remove this line
	while (1) { cas_spi_transceive(dev, cfg, &send_buffer, 4, NULL, 0); }

	// Uncomment this line
	// transmit_packet(dev, cfg, &send_buffer, NETWORKID, TONODEID);
	
}