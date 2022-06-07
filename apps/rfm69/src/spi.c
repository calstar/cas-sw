#include <device.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>

#define SPI_NODE cas_spi0

// The CS pin on the microcontroller for this SPI bus is GPIO pin B9
#define CS_PIN_BANK gpiob
#define CS_PIN_NUMBER 9

const struct spi_cs_control translator_spi_cs_control = {
    .gpio_dev = DEVICE_DT_GET(DT_NODELABEL(CS_PIN_BANK)),
    .gpio_pin = CS_PIN_NUMBER,
    .gpio_dt_flags = GPIO_ACTIVE_LOW,
    .delay = 10,
};

const struct spi_config translator_spi_config = {
    .operation = SPI_OP_MODE_MASTER,
    .cs = &translator_spi_cs_control,
};

uint8_t write_buf_contents [MAXIMUM_DATA_LENGTH];
uint8_t read_buf_contents [MAXIMUM_DATA_LENGTH];
uint8_t write_buf_length;
uint8_t read_buf_length;

struct spi_buf *write_buf;
struct spi_buf *read_buf;

struct spi_buf_set *write_buf_set;
struct spi_buf_set *read_buf_set;

struct device* get_spi_dev() {
	struct device *spi_dev = DEVICE_DT_GET(DT_NODELABEL(SPI_NODE));
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