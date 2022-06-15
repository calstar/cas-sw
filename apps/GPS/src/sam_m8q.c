/* u-blox SAM-M8Q GNSS module
 * 
 * Datasheet: https://content.u-blox.com/sites/default/files/SAM-M8Q_DataSheet_%28UBX-16012619%29.pdf
 * Protocol Specification: https://content.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf
 *
 * Note: for more information about how this code works, please
 * see pages 168-179 and page 374 of the protocol specification.
 */

#include <device.h>
#include <drivers/i2c.h>
#include "sam_m8q.h"

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

void send_ubx_msg(struct device *dev, UbxMessage *msg) {
	char sync_char_1 = SYNC_CHAR_1;
	char sync_char_2 = SYNC_CHAR_2;
	i2c_write(dev, &sync_char_1,      1,           SAM_M8Q_I2C_ADDR);
	i2c_write(dev, &sync_char_2,      1,           SAM_M8Q_I2C_ADDR);
	i2c_write(dev, &(msg->msgClass),  1,           SAM_M8Q_I2C_ADDR);
	i2c_write(dev, &(msg->msgId),     1,           SAM_M8Q_I2C_ADDR);
	i2c_write(dev, &(msg->length),    2,           SAM_M8Q_I2C_ADDR);
	i2c_write(dev, msg->payload,      msg->length, SAM_M8Q_I2C_ADDR);
	i2c_write(dev, &(msg->checksumA), 1,           SAM_M8Q_I2C_ADDR);
	i2c_write(dev, &(msg->checksumB), 1,           SAM_M8Q_I2C_ADDR);
	return;
}

UbxMessage* receive_ubx_msg(struct device *dev) {
	char sync_char_1 = 0;
	char sync_char_2 = 0;
	UbxMessage *msg = create_ubx_msg(0, 0, 0, NULL);
	i2c_read(dev, &sync_char_1,      1,           SAM_M8Q_I2C_ADDR);
	i2c_read(dev, &sync_char_2,      1,           SAM_M8Q_I2C_ADDR);
	i2c_read(dev, &(msg->msgClass),  1,           SAM_M8Q_I2C_ADDR);
	i2c_read(dev, &(msg->msgId),     1,           SAM_M8Q_I2C_ADDR);
	i2c_read(dev, &(msg->length),    2,           SAM_M8Q_I2C_ADDR);
	i2c_read(dev, msg->payload,      msg->length, SAM_M8Q_I2C_ADDR);
	i2c_read(dev, &(msg->checksumA), 1,           SAM_M8Q_I2C_ADDR);
	i2c_read(dev, &(msg->checksumB), 1,           SAM_M8Q_I2C_ADDR);
	return msg;
}

UbxMessage* create_ubx_msg(uint8_t class, uint8_t id, uint16_t length, uint8_t *payload_buf) {
	if (length > MAX_UBX_PAYLOAD_LENGTH) {
		printk("Error in sam-m8q.c: Message is too long.\n");
		return NULL;
	}
	static UbxMessage msg;
	msg.msgClass = class;
	msg.msgId = id;
	msg.length = length;
	for (int i=0; i<MAX_UBX_PAYLOAD_LENGTH; i++) { msg.payload[i] = 0; }
	for (int i=0; i<length; i++) { msg.payload[i] = payload_buf[i]; }
	computeChecksum(&msg);
	return &msg;
}

void sam_m8q_get_position(struct device *dev) {
	UbxMessage *msg_request = create_ubx_msg(NAV_CLASS, NAV_POSLLH, 0, NULL);
	send_ubx_msg(dev, msg_request);
	UbxMessage *msg_response = receive_ubx_msg(dev);
	uint32_t longitude = (uint32_t) (msg_response->payload)[4];
	uint32_t latitude = (uint32_t) (msg_response->payload)[8];
	uint32_t altitude = (uint32_t) (msg_response->payload)[12];
	printk("Longitude: %d, Latitude: %d, Altitude: %d\n", longitude, latitude, altitude);
	return;

}