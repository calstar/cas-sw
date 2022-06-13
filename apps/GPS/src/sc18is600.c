/* SC18IS600 SPI-to-I2C translator module
 *
 * Datasheet: https://www.nxp.com/docs/en/data-sheet/SC18IS600.pdf
 *
 * Note: For more information about how this code works, please 
 * see pages 11-14 of the datasheet.
 */

#include <string.h>
#include <device.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>
#include "sc18is600.h"
#include "cas_spi.h"

uint8_t read_internal_reg(struct device *dev, struct spi_config *cfg, uint8_t reg_address) {

	int status = 0;

	uint8_t send_buf[3] = { REG_READ_COMMAND, reg_address, 0x00 };
	int send_length = 3;
	uint8_t recieve_buf[3] = { 0x00, 0x00, 0x00 };
	int recieve_length = 3;

	status = cas_spi_transceive(dev, cfg, send_buf, send_length, recieve_buf, recieve_length);

	if (status != 0) printk("Error in sc18is600.c: Failed to read internal register.\n");

	return recieve_buf[2];

}

int check_i2c_status(struct device *dev, struct spi_config *cfg) {
	int status = 0;
	uint8_t error_code = read_internal_reg(dev, cfg, I2C_STATUS_REG);
	switch (error_code) {
		case I2C_NO_ERR : status = 0; break;
		case I2C_ERR_NO_RESPONSE_1 : status = 1; printk("Error in sc18is600.c: I2C no response (type 1).\n");
		case I2C_ERR_NO_RESPONSE_2 : status = 2; printk("Error in sc18is600.c: I2C no response (type 2).\n");
		case I2C_ERR_BUSY : status = 3; printk("Error in sc18is600.c: I2C device is busy.\n");
		case I2C_ERR_TIMEOUT : status = 4; printk("Error in sc18is600.c: I2C timeout.\n");
		case I2C_ERR_INVALID_LENGTH : status = 5; printk("Error in sc18is600.c: invalid I2C message length.\n");
	}
	return status;
}

int i2c_write_translated(struct device *dev, struct spi_config *cfg, uint8_t *i2c_data_buf, uint8_t i2c_num_bytes, uint8_t i2c_addr) {

	int status = 0;

	uint8_t send_buf[MAX_SPI_DATA_LENGTH];
	send_buf[0] = WRITE_COMMAND;
	send_buf[1] = i2c_num_bytes;
	send_buf[2] = i2c_addr;
	for (int i=0; i<i2c_num_bytes; i++) { send_buf[i+3] = i2c_data_buf[i]; }
	int send_length = i2c_num_bytes + 3;

	uint8_t recieve_buf = NULL;
	int recieve_length = 0;

	status = cas_spi_transceive(dev, cfg, send_buf, send_length, recieve_buf, recieve_length);

	if (status != 0) printk("Error in sc18is600.c: SPI-write to I2C-write translation failed.\n");

	status = check_i2c_status(dev, cfg);

	if (status != 0) printk("Error in sc18is600.c: I2C error code appeared in I2C status register.\n");

	return status;
	
}

/* The first command tells the device to read from the I2C bus and store the data
 * in the read buffer. The second command tells the device to return the data in the
 * read buffer to the CPU over the SPI bus. */
int i2c_read_translated(struct device *dev, struct spi_config *cfg, uint8_t *i2c_data_buf, uint8_t i2c_num_bytes, uint8_t i2c_addr) {

	int status = 0;

	uint8_t send_1_buf[3] = { READ_COMMAND, i2c_num_bytes, i2c_addr };
	int send_1_length = 3;
	uint8_t recieve_1_buf = NULL;
	int recieve_1_length = 0;

	status = cas_spi_transceive(dev, cfg, send_1_buf, send_1_length, recieve_1_buf, recieve_1_length);

	if (status != 0) printk("Error in sc18is600.c: SPI-read to I2C-read translation failed (part 1).\n");

	uint8_t send_2_buf[1] = { READ_BUFFER_COMMAND };
	int send_2_length = 1;
	uint8_t recieve_2_buf[MAX_SPI_DATA_LENGTH];
	int recieve_2_length = i2c_num_bytes;

	status = cas_spi_transceive(dev, cfg, send_2_buf, send_2_length, recieve_2_buf, recieve_2_length);

	if (status != 0) printk("Error in sc18is600.c: SPI-read to I2C-read translation failed (part 2).\n");

	status = check_i2c_status(dev, cfg);

	if (status != 0) printk("Error in sc18is600.c: I2C error code appeared in I2C status register.\n");

	for (int i=0; i<i2c_num_bytes; i++) {
		i2c_data_buf[i] = recieve_2_buf[i];
	}

	return status;
	
}