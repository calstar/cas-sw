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

struct spi_buf_set* create_spi_buf_set() {

	struct spi_buf_set *my_buf_set;
	struct spi_buf *my_buf;
	uint8_t my_buf_contents [MAXIMUM_DATA_LENGTH];

	my_buf_set->buffers = my_buf;
	my_buf_set->count = 1;

	my_buf->buf = my_buf_contents;
	my_buf->len = 0;

	clear_spi_buf_set(my_buf_set);

	return my_buf_set;

}

void clear_spi_buf_set(struct spi_buf_set *my_buf_set) {

	struct spi_buf *my_buf = my_buf_set->buffers;
	uint8_t *my_buf_contents = my_buf_set->buffers->buf;

	for (int i=0; i<MAXIMUM_DATA_LENGTH; i++) { my_buf_contents[i] = 0x00; }

	my_buf->len = 0;

}

uint8_t* get_buf_contents(struct spi_buf_set *my_buf_set) {
	return my_buf_set->buffers->buf;
}

int get_buf_length(struct spi_buf_set *my_buf_set) {
	return my_buf_set->buffers->len;
}

void set_buf_contents(struct spi_buf_set *my_buf_set, uint8_t *new_buf_contents) {
	for (int i=0; i<sizeof(new_buf_contents); i++) {
		((my_buf_set->buffers->buf)[i]) = new_buf_contents[i];
	}
}

void set_buf_length(struct spi_buf_set *my_buf_set, int new_buf_length) {
	my_buf_set->buffers->len = new_buf_length;
}



