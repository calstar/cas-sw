#include <device.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>

#ifndef CAS_SPI_HEADER
#define CAS_SPI_HEADER

/* Arbitrarily chosen for convenience purposes. This value can be increased if
 * desired, but be aware that the specific device you are communicating with
 * might have stricter limits on the amount of data it can recieve. */
#define MAXIMUM_DATA_LENGTH 64

#define GPIO_BANK_A gpioa
#define GPIO_BANK_B gpiob
#define GPIO_BANK_C gpioc
#define GPIO_BANK_D gpiod

#define SPI1_NODE cas_spihs
#define SPI2_NODE cas_spi0
#define SPI3_NODE cas_onboard_spi

struct spi_config* get_spi_config(char gpio_bank, int gpio_pin_num);

struct device* get_spi_dev(int spi_bus);

void clear_write_buf_set();

void clear_read_buf_set();

int cas_spi_transceive(struct device *dev, struct spi_config *cfg, 
					   uint8_t *send_buf, uint8_t send_length,
					   uint8_t *receive_buf, uint8_t receive_length);

#endif