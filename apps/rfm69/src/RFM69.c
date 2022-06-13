#include <device.h>
#include "cas_spi.h"
#include "rfm69.h"

void write_internal_reg(struct device *dev, struct spi_config *cfg, uint8_t reg_address, uint8_t value) {

	// Set MSB to 1, indicates a write
	reg_address |= 0x80;

	uint8_t send_buf[2] = { reg_address, value };
	int send_length = 2;
	uint8_t recieve_buf[2] = { 0x00, 0x00 };
	int recieve_length = 0;

	int status = cas_spi_transceive(dev, cfg, send_buf, send_length, recieve_buf, recieve_length);
	if (status != 0) printk("Error in rfm69.c: Failed to write internal register.\n");

	return;

}

uint8_t read_internal_reg(struct device *dev, struct spi_config *cfg, uint8_t reg_address) {

	// Set MSB to 0, indicates a read
	reg_address &= 0x7F;

	uint8_t send_buf[2] = { reg_address, 0x00 };
	int send_length = 2;
	uint8_t recieve_buf[2] = { 0x00, 0x00 };
	int recieve_length = 2;

	int status = cas_spi_transceive(dev, cfg, send_buf, send_length, recieve_buf, recieve_length);
	if (status != 0) printk("Error in rfm69.c: Failed to read internal register.\n");

	return recieve_buf[1];

}

void set_mode(struct device *dev, struct spi_config *cfg, uint8_t mode) {
	write_internal_reg(dev, cfg, REG_OPMODE, (RF_OPMODE_SEQUENCER_OFF | RF_OPMODE_LISTEN_OFF | mode));
}

void set_payload_length(struct device *dev, struct spi_config *cfg, uint8_t payload_length) {
	write_internal_reg(dev, cfg, REG_PAYLOADLENGTH, payload_length);
}

void set_frequency(struct device *dev, struct spi_config *cfg, uint8_t freq_msb, uint8_t freq_mid, uint8_t freq_lsb) {
	write_internal_reg(dev, cfg, REG_FRFMSB, freq_msb);
	write_internal_reg(dev, cfg, REG_FRFMID, freq_mid);
	write_internal_reg(dev, cfg, REG_FRFLSB, freq_lsb);
}

void configure_radio(struct device *dev, struct spi_config *cfg) {
	// Usually, the frequency should be either 433 MHz or 915 MHz
	set_frequency(dev, cfg, RF_FRFMSB_915, RF_FRFMID_915, RF_FRFLSB_915);
	set_payload_length(dev, cfg, PAYLOAD_LENGTH);
}

void transmit_packet(struct device *dev, struct spi_config *cfg, 
					 uint8_t *payload, uint8_t receiver_network, uint8_t receiver_address) {

	set_mode(dev, cfg, RF_OPMODE_STANDBY);

	// Assemble packet into the FIFO register
	for (int i=0; i<8; i++) { write_internal_reg(dev, cfg, REG_FIFO, PREAMBLE_BYTE); }
	write_internal_reg(dev, cfg, REG_FIFO, receiver_network);
	write_internal_reg(dev, cfg, REG_FIFO, receiver_address);
	for (int i=0; i<PAYLOAD_LENGTH; i++) { write_internal_reg(dev, cfg, REG_FIFO, payload[i]); }

	set_mode(dev, cfg, RF_OPMODE_TRANSMITTER);

	// Wait for confirmation that the packet was sent
	while ((read_internal_reg(dev, cfg, REG_IRQFLAGS2) & RF_IRQFLAGS2_PACKETSENT) == 0x00);

	set_mode(dev, cfg, RF_OPMODE_STANDBY);

}
