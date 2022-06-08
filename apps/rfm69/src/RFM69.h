#include <device.h>
#include "RFM69registers.h"

#ifndef RFM69_HEADER
#define RFM69_HEADER

/* The packet must be smaller than 66 bytes because the FIFO register
 * can hold at most 66 bytes. The structure of the packet is as follows:
 * 8 bytes: preamble (10101010)
 * 1 byte: network ID
 * 1 byte: destination address
 * 50 bytes: payload
 * See pages 51-52 of the datasheet for more information about packet structure. */
#define PAYLOAD_LENGTH 50
#define PACKET_LENGTH 60

// Usually, this should be either 433 MHz or 915 MHz
#define FREQUENCY RF69_915MHZ

// In binary: 10101010
#define PREAMBLE_BYTE 0xAA

#define NETWORKID 0
#define MYNODEID 0
#define TONODEID 255
#define ENCRYPT false
#define USEACK false

void write_internal_reg(struct device *dev, struct spi_config *cfg, uint8_t reg_address, uint8_t value);

uint8_t read_internal_reg(struct device *dev, struct spi_config *cfg, uint8_t reg_address);

void set_mode(struct device *dev, struct spi_config *cfg, uint8_t mode);

void set_payload_length(struct device *dev, struct spi_config *cfg, uint8_t payload_length);

void transmit_packet(struct device *dev, struct spi_config *cfg, uint8_t *payload, uint8_t address);

#endif