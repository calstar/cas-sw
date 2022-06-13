#include <device.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>
#include "cas_spi.h"

uint8_t write_buf_contents[MAX_SPI_DATA_LENGTH];
uint8_t read_buf_contents[MAX_SPI_DATA_LENGTH];

struct spi_buf write_buf = {
	.buf = write_buf_contents,
	.len = 0,
};
struct spi_buf read_buf = {
	.buf = read_buf_contents,
	.len = 0,
};

struct spi_buf_set write_buf_set = {
	.buffers = &write_buf,
	.count = 1,
};
struct spi_buf_set read_buf_set = {
	.buffers = &read_buf,
	.count = 1,
};

void clear_write_buf_set() {
	for (int i=0; i<MAX_SPI_DATA_LENGTH; i++) { write_buf_contents[i] = 0x00; }
	write_buf.len = 0;
}
void clear_read_buf_set() {
	for (int i=0; i<MAX_SPI_DATA_LENGTH; i++) { read_buf_contents[i] = 0x00; }
	read_buf.len = 0;
}

int cas_spi_transceive(struct device *dev, struct spi_config *cfg, 
						uint8_t *send_buf, uint8_t send_length,
						uint8_t *receive_buf, uint8_t receive_length) {

	if (send_length > MAX_SPI_DATA_LENGTH || receive_length > MAX_SPI_DATA_LENGTH) {
		printk("Error in cas_spi.c: write or read length is too long.\n");
		return -1;
	}

	clear_write_buf_set();
	clear_read_buf_set();

	for (int i=0; i<send_length; i++) { write_buf_contents[i] = send_buf[i]; }
	write_buf.len = send_length;
	read_buf.len = receive_length;

	int status = spi_transceive(dev, cfg, &write_buf_set, &read_buf_set);
	if (status != 0) printk("Error in cas_spi.c: Failed to transceive over spi.\n");

	for (int i=0; i<receive_length; i++) { receive_buf[i] = read_buf_contents[i]; }

	return status;

}




