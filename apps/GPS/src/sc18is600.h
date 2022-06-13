/* SC18IS600 SPI-to-I2C translator module
 *
 * Datasheet: https://www.nxp.com/docs/en/data-sheet/SC18IS600.pdf
 */

#include <device.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>

#define WRITE_COMMAND 0x00
#define READ_COMMAND 0x01
#define READ_AFTER_WRITE_COMMAND 0x02
#define WRITE_AFTER_WRITE_COMMAND 0x03
#define READ_BUFFER_COMMAND 0x06
#define SPI_CONFIG_COMMAND 0x18
#define REG_WRITE_COMMAND 0x20
#define REG_READ_COMMAND 0x21
#define POWER_DOWN_COMMAND 0x30

#define IO_CONFIG_REG 0x00
#define IO_STATE_REG 0x01
#define I2C_CLOCK_REG 0x02
#define I2C_TIMEOUT_REG 0x03
#define I2C_STATUS_REG 0x04
#define I2C_ADDRESS_REG 0x05

#define I2C_NO_ERR 0xF0
#define I2C_ERR_NO_RESPONSE_1 0xF1
#define I2C_ERR_NO_RESPONSE_2 0xF2
#define I2C_ERR_BUSY 0xF3
#define I2C_ERR_TIMEOUT 0xF8
#define I2C_ERR_INVALID_LENGTH 0xF9

uint8_t read_internal_reg(struct device *dev, struct spi_config *cfg, uint8_t reg_address);

int check_i2c_status(struct device *dev, struct spi_config *cfg);

int i2c_write_translated(struct device *dev, struct spi_config *cfg, uint8_t *i2c_data_buf, uint8_t i2c_num_bytes, uint8_t i2c_addr);

int i2c_read_translated(struct device *dev, struct spi_config *cfg, uint8_t *i2c_data_buf, uint8_t i2c_num_bytes, uint8_t i2c_addr);