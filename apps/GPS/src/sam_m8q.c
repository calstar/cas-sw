/* u-blox SAM-M8Q GNSS module
 * 
 * Datasheet: https://content.u-blox.com/sites/default/files/SAM-M8Q_DataSheet_%28UBX-16012619%29.pdf
 * Protocol Specification: https://content.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf
 *
 * Note: for more information about how this code works, please
 * see pages 168-179 of the protocol specification.
 */

#include <device.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>
#include "sam_m8q.h"
#include "sc18is600.h"
#include "cas_spi.h"

/* Copied from this implementation:
 * https://github.com/melopero/Melopero_UBX/blob/master/src/Melopero_UBX.cpp */
void computeChecksum(UbxMessage *msg){
  msg->checksumA = msg->msgClass;
  msg->checksumB = msg->checksumA;
  msg->checksumA += msg->msgId;
  msg->checksumB += msg->checksumA;
  msg->checksumA += msg->length % (1<<8);
  msg->checksumB += msg->checksumA;
  msg->checksumA += msg->length >> 8;
  msg->checksumB += msg->checksumA;
  for (int i = 0; i < msg->length; i++){
    msg->checksumA += msg->payload[i];
    msg->checksumB += msg->checksumA;
  }
}

int send_ubx_msg(struct device *dev, struct spi_config *cfg, UbxMessage *msg) {
	int status = 0;
	char sync_char_1 = SYNC_CHAR_1;
	char sync_char_2 = SYNC_CHAR_2;
	status = i2c_write_translated(dev, cfg, &sync_char_1,      1,           SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, cfg, &sync_char_2,      1,           SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, cfg, &(msg->msgClass),  1,           SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, cfg, &(msg->msgId),     1,           SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, cfg, &(msg->length),    2,           SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, cfg, msg->payload,      msg->length, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, cfg, &(msg->checksumA), 1,           SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, cfg, &(msg->checksumB), 1,           SAM_M8Q_I2C_ADDR);
	if (status != 0) { printk("Error in sam-m8q.c: Failed to send a UBX message over the I2C channel.\n"); }
	return status;
}

int receive_ubx_msg(struct device *dev, struct spi_config *cfg, UbxMessage *msg) {
	int status = 0;
	char sync_char_1 = SYNC_CHAR_1;
	char sync_char_2 = SYNC_CHAR_2;
	status = i2c_read_translated(dev, cfg, &sync_char_1,      1,           SAM_M8Q_I2C_ADDR);
	status = i2c_read_translated(dev, cfg, &sync_char_2,      1,           SAM_M8Q_I2C_ADDR);
	status = i2c_read_translated(dev, cfg, &(msg->msgClass),  1,           SAM_M8Q_I2C_ADDR);
	status = i2c_read_translated(dev, cfg, &(msg->msgId),     1,           SAM_M8Q_I2C_ADDR);
	status = i2c_read_translated(dev, cfg, &(msg->length),    2,           SAM_M8Q_I2C_ADDR);
	status = i2c_read_translated(dev, cfg, msg->payload,      msg->length, SAM_M8Q_I2C_ADDR);
	status = i2c_read_translated(dev, cfg, &(msg->checksumA), 1,           SAM_M8Q_I2C_ADDR);
	status = i2c_read_translated(dev, cfg, &(msg->checksumB), 1,           SAM_M8Q_I2C_ADDR);
	if (status != 0) { printk("Error in sam-m8q.c: Failed to receive a UBX message over the I2C channel.\n"); }
	return status;
}

UbxMessage* create_ubx_msg(uint8_t class, uint8_t id, uint16_t length, uint8_t *payload_buf) {
	if (length > MAX_UBX_PAYLOAD_LENGTH) {
		printk("Error in sam-m8q.c: Message is too long.\n");
		return NULL;
	}
	UbxMessage msg;
	msg.msgClass = class;
	msg.msgId = id;
	msg.length = length;
	for (int i=0; i<MAX_UBX_PAYLOAD_LENGTH; i++) { msg.payload[i] = 0; }
	for (int i=0; i<length; i++) { msg.payload[i] = payload_buf[i]; }
	computeChecksum(&msg);
	return &msg;
}

Position* sam_m8q_get_position(struct device *dev, struct spi_config *cfg) {
	int status = 0;
	uint8_t payload_buf[28] = { 0, 0, 0, 0, 0, 0, 0, 
								0, 0, 0, 0, 0, 0, 0, 
								0, 0, 0, 0, 0, 0, 0, 
								0, 0, 0, 0, 0, 0, 0, };
	UbxMessage *msg = create_ubx_msg(NAV_CLASS, NAV_POSLLH, 28, payload_buf);
	status = send_ubx_msg(dev, cfg, msg);
	if (status != 0) { printk("Error in sam-m8q.c: Failed to send ubx position request.\n"); }
	status = receive_ubx_msg(dev, cfg, msg);
	if (status != 0) { printk("Error in sam-m8q.c: Failed to receive ubx position reply.\n"); }
	for (int i=0; i<28; i++) { payload_buf[i] = msg->payload[i]; }
	return (Position*) &payload_buf;
}



