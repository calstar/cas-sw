#include <device.h>
#include "cas_spi.h"
#include "rfm69.h"

void write_internal_reg(struct device *dev, struct spi_config *cfg, uint8_t reg_address, uint8_t value) {

	clear_write_buf_set();
	clear_read_buf_set();

	// Set MSB to 1, indicates a write
	reg_address |= 0x80;

	write_buf_contents[0] = reg_address;
	write_buf_contents[1] = value;
	write_buf_length = 2;

	int status = spi_transceive(dev, cfg, write_buf_set, read_buf_set);

	if (status != 0) printk("Error in rfm69.c: Failed to write internal register.\n");

	return;

}

uint8_t read_internal_reg(struct device *dev, struct spi_config *cfg, uint8_t reg_address) {

	clear_write_buf_set();
	clear_read_buf_set();

	// Set MSB to 0, indicates a read
	reg_address &= 0x7F;

	write_buf_contents[0] = reg_address;
	write_buf_length = 1;
	read_buf_length = 1;

	int status = spi_transceive(dev, cfg, write_buf_set, read_buf_set);

	if (status != 0) printk("Error in rfm69.c: Failed to read internal register.\n");

	return read_buf_contents[0];

}

void set_mode(struct device *dev, struct spi_config *cfg, uint8_t mode) {
	write_internal_reg(dev, cfg, REG_OPMODE, (RF_OPMODE_SEQUENCER_OFF | RF_OPMODE_LISTEN_OFF | mode));
}

void set_payload_length(struct device *dev, struct spi_config *cfg, uint8_t payload_length) {
	write_internal_reg(dev, cfg, REG_PAYLOADLENGTH, payload_length);
}

void transmit_packet(struct device *dev, struct spi_config *cfg, uint8_t *payload, uint8_t address) {

	set_payload_length(dev, cfg, PAYLOAD_LENGTH);

	for (int i=0; i<8; i++) { write_internal_reg(dev, cfg, REG_FIFO, PREAMBLE_BYTE); }
	write_internal_reg(dev, cfg, REG_FIFO, NETWORKID);
	write_internal_reg(dev, cfg, REG_FIFO, TONODEID);
	for (int i=0; i<PAYLOAD_LENGTH; i++) { write_internal_reg(dev, cfg, REG_FIFO, payload[i]); }

	set_mode(dev, cfg, RF_OPMODE_TRANSMITTER);

	// Wait for confirmation that the packet was sent
	while ((read_internal_reg(dev, cfg, REG_IRQFLAGS2) & RF_IRQFLAGS2_PACKETSENT) == 0x00);

	set_mode(dev, cfg, RF_OPMODE_STANDBY);

}
