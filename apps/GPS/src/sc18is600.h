/* SC18IS600 SPI-to-I2C translator module
 * 
 * Datasheet: https://www.nxp.com/docs/en/data-sheet/SC18IS600.pdf
 */

#include <device.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>

#define SPI_NODE cas_onboard_spi

const struct spi_cs_control translator_spi_cs_control;

const struct spi_config translator_spi_config;

struct device* get_spi_dev();

int i2c_write_translated(struct device *dev, uint8_t *buf, uint32_t num_bytes, uint16_t addr);

int i2c_read_translated(struct device *dev, uint8_t *buf, uint32_t num_bytes, uint16_t addr);