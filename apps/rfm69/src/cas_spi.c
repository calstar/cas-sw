#include <device.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>
#include "cas_spi.h"

struct spi_config* get_spi_config(char gpio_bank, int gpio_pin_num) {

	struct spi_cs_control *ctrl;
	struct spi_config *cfg;

	switch (gpio_bank) {
		case 'A' : ctrl->gpio_dev = DEVICE_DT_GET(DT_NODELABEL(GPIO_BANK_A));
		case 'B' : ctrl->gpio_dev = DEVICE_DT_GET(DT_NODELABEL(GPIO_BANK_B));
		case 'C' : ctrl->gpio_dev = DEVICE_DT_GET(DT_NODELABEL(GPIO_BANK_C));
		case 'D' : ctrl->gpio_dev = DEVICE_DT_GET(DT_NODELABEL(GPIO_BANK_D));
	}
	ctrl->gpio_pin = gpio_pin_num;
	ctrl->gpio_dt_flags = GPIO_ACTIVE_LOW;

	cfg->operation = SPI_OP_MODE_MASTER;
	cfg->cs = ctrl;

	return cfg;

}

struct device* get_spi_dev(int spi_bus) {

	struct device *spi_dev;

	switch (spi_bus) {
		case 1 : spi_dev = DEVICE_DT_GET(DT_NODELABEL(SPI1_NODE));
		case 2 : spi_dev = DEVICE_DT_GET(DT_NODELABEL(SPI2_NODE));
		case 3 : spi_dev = DEVICE_DT_GET(DT_NODELABEL(SPI3_NODE));
	}

	if (spi_dev == NULL) {
		printk("Error in spi.c: Failed to get device binding.\n");
		return NULL;
	}

	if (!device_is_ready(spi_dev)) {
		printk("Error in spi.c: Device is not ready.\n");
		return NULL;
	}

	return spi_dev;

}

void clear_write_buf_set() {
	for (int i=0; i<MAXIMUM_DATA_LENGTH; i++) { write_buf_contents[i] = 0x00; }
	write_buf_length = 0;
	write_buf->buf = write_buf_contents;
	write_buf->len = write_buf_length;
	write_buf_set->buffers = write_buf;
	write_buf_set->count = 1;
}

void clear_read_buf_set() {
	for (int i=0; i<MAXIMUM_DATA_LENGTH; i++) { read_buf_contents[i] = 0x00; }
	read_buf_length = 0;
	read_buf->buf = read_buf_contents;
	read_buf->len = read_buf_length;
	read_buf_set->buffers = read_buf;
	read_buf_set->count = 1;
}



