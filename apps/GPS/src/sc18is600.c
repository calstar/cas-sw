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

struct device* get_spi_dev() {
	struct device *spi_dev = DEVICE_DT_GET(DT_NODELABEL(SPI_NODE));
	if (spi_dev == NULL) {
		printk("Error in sc18is600.c: Failed to get device binding.\n");
		return NULL;
	}
	if (!device_is_ready(spi_dev)) {
		printk("Error in sc18is600.c: Device is not ready.\n");
		return NULL;
	}
	return spi_dev;
}

// Helper function that returns a pointer to the actual data in the read buffer.
uint8_t* get_read_buffer_ptr() {
	return read_buf_set->buffers->buf;
}

// Helper functions that empty the contents of the write and read buffers.
void clear_write_buf_set() {
	for (int i=0; i<MAXIMUM_DATA_LENGTH; i++) { write_buf_contents[i] = 0x00; }
	write_buf_length = MAXIMUM_DATA_LENGTH;
	write_buf->buf = write_buf_contents;
	write_buf->len = write_buf_length;
	write_buf_set->buffers = write_buf;
	write_buf_set->count = 1;
}
void clear_read_buf_set() {
	for (int i=0; i<MAXIMUM_DATA_LENGTH; i++) { read_buf_contents[i] = 0x00; }
	read_buf_length = MAXIMUM_DATA_LENGTH;
	read_buf->buf = read_buf_contents;
	read_buf->len = read_buf_length;
	read_buf_set->buffers = read_buf;
	read_buf_set->count = 1;
}

uint8_t read_internal_reg(struct device *dev, uint8_t reg_address) {

	int status = 0;

	clear_write_buf_set();
	clear_read_buf_set();

	uint8_t dummy_byte = 0x00;

	write_buf_contents[0] = REG_READ_COMMAND;
	write_buf_contents[1] = reg_address;
	write_buf_contents[2] = dummy_byte;
	write_buf_length = 3;
	read_buf_length = 1;

	status = spi_transceive(dev, &translator_spi_config, write_buf_set, read_buf_set);

	if (status != 0) printk("Error in sc18is600.c: Failed to read internal register.\n");

	return get_read_buffer_ptr()[0];

}

int check_i2c_status(struct device *dev) {
	int status = 0;
	uint8_t error_code = read_internal_reg(dev, I2C_STATUS_REG);
	switch (error_code) {
		case I2C_NO_ERR : status = 0; break;
		case I2C_ERR_NO_RESPONSE_1 : status = 1; printk("Error in sc18is600.c: I2C no response (error 1).\n");
		case I2C_ERR_NO_RESPONSE_2 : status = 2; printk("Error in sc18is600.c: I2C no response (error 2).\n");
		case I2C_ERR_BUSY : status = 3; printk("Error in sc18is600.c: I2C device is busy.\n");
		case I2C_ERR_TIMEOUT : status = 4; printk("Error in sc18is600.c: I2C timeout.\n");
		case I2C_ERR_INVALID_LENGTH : status = 5; printk("Error in sc18is600.c: invalid I2C message length.\n");
	}
	return status;
}

int i2c_write_translated(struct device *dev, uint8_t *i2c_data_buf, uint8_t i2c_num_bytes, uint8_t i2c_addr) {

	int status = 0;

	clear_write_buf_set();
	clear_read_buf_set();

	write_buf_contents[0] = WRITE_COMMAND;
	write_buf_contents[1] = i2c_num_bytes;
	write_buf_contents[2] = i2c_addr;
	for (uint8_t i=0; i<i2c_num_bytes; i++) {
		write_buf_contents[i+3] = i2c_data_buf[i];
	}
	write_buf_length = i2c_num_bytes + 3;

	status = spi_transceive(dev, &translator_spi_config, write_buf_set, read_buf_set);

	status = check_i2c_status(dev);

	if (status != 0) printk("Error in sc18is600.c: SPI-write to I2C-write translation failed.\n");

	return status;
	
}

/* The first command tells the device to read from the I2C bus and store the data
 * in the read buffer. The second command tells the device to return the data in the
 * read buffer to the CPU over the SPI bus. */
int i2c_read_translated(struct device *dev, uint8_t *i2c_data_buf, uint8_t i2c_num_bytes, uint8_t i2c_addr) {

	int status = 0;

	clear_write_buf_set();
	clear_read_buf_set();

	write_buf_contents[0] = READ_COMMAND;
	write_buf_contents[1] = i2c_num_bytes;
	write_buf_contents[2] = i2c_addr;
	write_buf_length = 3;

	status = spi_transceive(dev, &translator_spi_config, write_buf_set, read_buf_set);

	clear_write_buf_set();

	write_buf_contents[0] = READ_BUFFER_COMMAND;
	write_buf_length = 1;
	read_buf_length = i2c_num_bytes;

	status = spi_transceive(dev, &translator_spi_config, write_buf_set, read_buf_set);

	status = check_i2c_status(dev);

	if (status != 0) printk("Error in sc18is600.c: SPI-read to I2C-read translation failed.\n");

	memcpy(i2c_data_buf, read_buf_contents, i2c_num_bytes);

	return status;
	
}



