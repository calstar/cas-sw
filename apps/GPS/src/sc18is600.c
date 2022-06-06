/* SC18IS600 SPI-to-I2C translator module
 * 
 * Datasheet: https://www.nxp.com/docs/en/data-sheet/SC18IS600.pdf
 */

#include <device.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>
#include "sc18is600.h"

// GPIO pin C8 is used for CS pin
const struct spi_cs_control translator_spi_cs_control = {
    .gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpioc)),
    .gpio_pin = 8,
    .gpio_dt_flags = GPIO_ACTIVE_LOW,
    .delay = 10,
};

const struct spi_config translator_spi_config = {
    .operation = SPI_OP_MODE_MASTER,
    .cs = &translator_spi_cs_control,
};

struct device* get_spi_dev() {
	struct device *spi_dev = DEVICE_DT_GET(DT_NODELABEL(SPI_NODE));
	if (spi_dev == NULL) {
		printk("Failed to get device binding.\n");
		return;
	}
	if (!device_is_ready(spi_dev)) {
		printk("Device is not ready.\n");
		return;
	}
	return spi_dev;
}

int i2c_write_translated(struct device *dev, uint8_t *buf, uint32_t num_bytes, uint16_t addr) {

	int status;

	struct spi_buf *write_buf;
	write_buf->buf = buf;
	write_buf->len = num_bytes;

	status = spi_write(dev, &translator_spi_config, write_buf);

	return 0;
	
}

int i2c_read_translated(struct device *dev, uint8_t *buf, uint32_t num_bytes, uint16_t addr) {

	return 0;
	
}