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

/* The I2C bytes need to be sent all in one I2C transmission.
 * It doesn't work if you transmit one byte at a time. */
void send_ubx_msg(struct device *dev, UbxMessage *msg) {
	uint8_t send_buf[MAX_UBX_MESSAGE_LENGTH];
	send_buf[0] = SYNC_CHAR_1;
	send_buf[1] = SYNC_CHAR_2;
	send_buf[2] = msg->msgClass;
	send_buf[3] = msg->msgId;
	send_buf[4] = (msg->length & 0x00FF);
	send_buf[5] = ((msg->length & 0xFF00) >> 8);
	for (int i=0; i<(msg->length); i++) {
		send_buf[i+6] = msg->payload[i];
	}
	send_buf[msg->length + 6] = msg->checksumA;
	send_buf[msg->length + 7] = msg->checksumB;
	i2c_write(dev, send_buf, (msg->length + 8), SAM_M8Q_I2C_ADDR);
	return;
}

/* The sam-m8q is constantly outputting garbage we don't need
 * (including NMEA messages and lots of 0xFF bytes), so it's
 * important to check the two sync chars to verify this is
 * the correct message we want to read in. */
UbxMessage* receive_ubx_msg(struct device *dev) {

	int timeout = 1000000;

	char sync1 = 0;
	char sync2 = 0;
	UbxMessage *msg = create_ubx_msg(0, 0, 0, NULL);
	
	while (1) {

		timeout--;
		if (timeout < 0) { 
			printk("Error in sam-m8q.c: Waited too long to recieve UBX reply.\n");
			break; 
		}

		i2c_read(dev, &sync1, 1, SAM_M8Q_I2C_ADDR);
		if (sync1 == SYNC_CHAR_1) {
			i2c_read(dev, &sync2, 1, SAM_M8Q_I2C_ADDR);
			if (sync2 == SYNC_CHAR_2) {

				i2c_read(dev, &(msg->msgClass), 1, SAM_M8Q_I2C_ADDR);
				i2c_read(dev, &(msg->msgId), 1, SAM_M8Q_I2C_ADDR);
				i2c_read(dev, &(msg->length), 2, SAM_M8Q_I2C_ADDR);
				i2c_read(dev, msg->payload, msg->length, SAM_M8Q_I2C_ADDR);
				i2c_read(dev, &(msg->checksumA), 1, SAM_M8Q_I2C_ADDR);
				i2c_read(dev, &(msg->checksumB), 1, SAM_M8Q_I2C_ADDR);
				break;

			}
		}
	}

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

PVTData* sam_m8q_get_pvt_data(struct device *dev) {
	UbxMessage *msg_request = create_ubx_msg(NAV_CLASS, NAV_PVT, 0, NULL);
	send_ubx_msg(dev, msg_request);
	UbxMessage *msg_response = receive_ubx_msg(dev);
	return (PVTData*) msg_response->payload;
}



