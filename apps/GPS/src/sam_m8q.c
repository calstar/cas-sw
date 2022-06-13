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

/* Copied from this implementation:
 * https://github.com/melopero/Melopero_UBX/blob/master/src/Melopero_UBX.cpp */
void resetPayload(UbxMessage *msg){
  for (int i = 0; i < msg->length; i++)
    msg->payload[i] = 0;
}

int send_ubx_msg(struct device *dev, struct spi_config *cfg, UbxMessage *msg) {
	int status = 0;
	status = i2c_write_translated(dev, cfg, SYNC_CHAR_1, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, cfg, SYNC_CHAR_2, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, cfg, msg->msgClass, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, cfg, msg->msgId, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, cfg, msg->length, 2, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, cfg, msg->payload, msg->length, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, cfg, msg->checksumA, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, cfg, msg->checksumB, 1, SAM_M8Q_I2C_ADDR);
	if (status != 0) { printk("Error in sam-m8q.c: Failed to send a UBX message over the I2C channel.\n"); }
	return status;
}

/* This function sends a configuration message to the sam-m8q which sets
 * the communication channel to be ubx-only. For more information, see
 * page 252 of the protocol specification. */
int sam_m8q_enable(struct device *dev, struct spi_config *cfg) {
	int status = 0;
	UbxMessage *msg;
	msg->msgClass = CFG_CLASS;
	msg->msgId = CFG_PRT;
	msg->length = 20;
	resetPayload(msg);
	msg->payload[4] = 0x84;
	msg->payload[12] = 0x01;
	msg->payload[14] = 0x01;
	computeChecksum(msg);
	status = send_ubx_msg(dev, cfg, msg);
	if (status != 0) { printk("Error in sam-m8q.c: Failed to send configuration message.\n"); }
	return status;
}

int receive_ubx_msg(struct device *dev, struct spi_config *cfg, uint8_t length, uint8_t *buf) {
	return 0;
}

Position* sam_m8q_get_position(struct device *dev, struct spi_config *cfg) {
	return NULL;
}



