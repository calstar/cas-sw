#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include "rfm69.h"
#include "cas_spi.h"

// This application uses spi bus number 2, aka cas_spi0
#define SPI_BUS 2
// The CS pin on the microcontroller for this SPI bus is GPIO pin B9
#define CS_BANK 'B'
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

	struct device *dev = get_spi_dev(SPI_BUS);
	struct spi_config *cfg = get_spi_config(CS_BANK, CS_PIN);

	if (dev == NULL) {
		printk("Error in main.c: Unable to get spi device reference.\n");
		return;
	}

	clear_send_buffer();
	send_buffer[0] = 0xDE;
	send_buffer[1] = 0xAD;
	send_buffer[2] = 0xBE;
	send_buffer[3] = 0xEF;

	transmit_packet(dev, cfg, &send_buffer, NETWORKID, TONODEID);
	
}