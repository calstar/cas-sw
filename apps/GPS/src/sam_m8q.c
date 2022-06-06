/* u-blox SAM-M8Q GNSS module
 * 
 * Datasheet: https://content.u-blox.com/sites/default/files/SAM-M8Q_DataSheet_%28UBX-16012619%29.pdf
 * Protocol Specification: https://content.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf
 */

#include <device.h>
#include "sam_m8q.h"

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

/* Resets the payload to all zeroes*/
void resetPayload(UbxMessage *msg){
  for (int i = 0; i < msg->length; i++)
    msg->payload[i] = 0;
}

int send_ubx_msg(struct device *dev, UbxMessage *msg) {
	int status = 0;
	status = i2c_write_translated(dev, SYNC_CHAR_1, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, SYNC_CHAR_2, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, msg->msgClass, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, msg->msgId, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, msg->length, 2, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, msg->payload, msg->length, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, msg->checksumA, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write_translated(dev, msg->checksumB, 1, SAM_M8Q_I2C_ADDR);
	if (status != 0) { printk("Error in sam-m8q.c: Failed to send a UBX message over the I2C channel.\n"); }
	return status;
}

int sam_m8q_enable(struct device *dev) {
	// send a configuration message which sets the communication
	// channel to be UBX-only
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
	status = send_ubx_msg(dev, msg);
	if (status != 0) { printk("Error in sam-m8q.c: Failed to send configuration message.\n"); }
	return status;
}

// TODO: Implement these 2 functions
int sam_m8q_sample_fetch(struct device *dev) {}
int sam_m8q_channel_get(struct device *dev, PVTData channel, void *val) {}

/*
int sam_m8q_sample_fetch(struct device *dev) {}
int sam_m8q_channel_get(struct device *dev, enum gnss_channel chan, void *val) {}
int sam_m8q_attr_get(struct device *dev, enum gnss_channel chan, enum gnss_attribute attr, void *val) {}
int sam_m8q_attr_set(struct device *dev, enum gnss_channel chan, enum gnss_attribute attr, void *val) {}
int sam_m8q_trigger_set(struct device *dev, struct gnss_trigger *trig, gnss_trigger_handler_t handler) {}
*/


