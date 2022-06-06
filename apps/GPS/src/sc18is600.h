/* SC18IS600 SPI-to-I2C translator module
 * 
 * Datasheet: https://www.nxp.com/docs/en/data-sheet/SC18IS600.pdf
 */

#include <device.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>

#define SPI_NODE cas_onboard_spi

#define MAXIMUM_DATA_LENGTH 96

#define WRITE_COMMAND 0x00
#define READ_COMMAND 0x01
#define READ_AFTER_WRITE_COMMAND 0x02
#define WRITE_AFTER_WRITE_COMMAND 0x03
#define READ_BUFFER_COMMAND 0x06
#define SPI_CONFIG_COMMAND 0x18
#define REG_WRITE_COMMAND 0x20
#define REG_READ_COMMAND 0x21
#define POWER_DOWN_COMMAND 0x30

#define I2C_STATUS_REG 0x04

#define I2C_NO_ERR 0xF0
#define I2C_ERR_NO_RESPONSE_1 0xF1
#define I2C_ERR_NO_RESPONSE_2 0xF2
#define I2C_ERR_BUSY 0xF3
#define I2C_ERR_TIMEOUT 0xF8
#define I2C_ERR_INVALID_LENGTH 0xF9

const struct spi_cs_control translator_spi_cs_control = {
	// GPIO pin C8 is used for CS pin
    .gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpioc)),
    .gpio_pin = 8,
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




