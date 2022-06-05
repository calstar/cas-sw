/* u-blox SAM-M8Q GNSS module
 * 
 * Datasheet: https://content.u-blox.com/sites/default/files/SAM-M8Q_DataSheet_%28UBX-16012619%29.pdf
 * Protocol Specification: https://content.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf
 */

#include <device.h>
#include <drivers/i2c.h>

#define SAM_M8Q_I2C_ADDR 0x42
#define DATA_REGISTER 0xFF

#define UBX_SYNC_CHAR_1 0xB5
#define UBX_SYNC_CHAR_2 0x62

#define CLASS_NAV 0x01
#define CLASS_RXM 0x02
#define CLASS_INF 0x04
#define CLASS_ACK 0x05
#define CLASS_CFG 0x06
#define CLASS_UPD 0x09
#define CLASS_MON 0x0A
#define CLASS_AID 0x0B
#define CLASS_TIM 0x0D
#define CLASS_ESF 0x10
#define CLASS_MGA 0x13
#define CLASS_LOG 0x21
#define CLASS_SEC 0x27
#define CLASS_HNR 0x28

#define ID_ACK_ACK 0x01
#define ID_ACK_NAK 0x00

#define ID_CFG_PRT 0x00

struct ubx_message {
	uint8_t sync_char_1;
	uint8_t sync_char_2;
	uint8_t class;
	uint8_t id;
	uint16_t length;
	uint8_t *payload;
	uint8_t checksum_1;
	uint8_t checksum_2;
};

/*
int sam_m8q_sample_fetch(struct device *dev) {

}

int sam_m8q_channel_get(struct device *dev,
				        enum gnss_channel chan,
				        void *val) {

}

int sam_m8q_attr_get(struct device *dev,
			         enum gnss_channel chan,
			         enum gnss_attribute attr,
			         void *val) {

}

int sam_m8q_attr_set(struct device *dev,
			         enum gnss_channel chan,
			         enum gnss_attribute attr,
			         void *val) {

}

int sam_m8q_trigger_set(struct device *dev,
				        struct gnss_trigger *trig,
				        gnss_trigger_handler_t handler) {

}
*/

int sam_m8q_enable(struct device *dev) {
	// send a configuration message which sets the communication
	// channel to be UBX-only
	int status = 0;
	struct ubx_message msg;
	msg.sync_char_1 = UBX_SYNC_CHAR_1;
	msg.sync_char_2 = UBX_SYNC_CHAR_2;
	msg.class = CLASS_CFG;
	msg.id = ID_CFG_PRT;
	msg.length = (uint16_t) 20;
	uint8_t payload [20];
	payload[4] = 0x84;
	payload[12] = 0x01;
	payload[14] = 0x01;
	compute_checksum(&msg);
	status = send_ubx_message(dev, &msg);
	if (status != 0) { printk("Error in function sam_m8q_enable() in file sam-m8q.c\n"); }
	return status;
}

int send_ubx_message(struct device *dev, struct ubx_message* msg) {
	int status = 0;
	status = i2c_write(dev, msg->sync_char_1, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write(dev, msg->sync_char_2, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write(dev, msg->class, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write(dev, msg->id, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write(dev, msg->length, 2, SAM_M8Q_I2C_ADDR);
	status = i2c_write(dev, *(msg->payload), msg->length, SAM_M8Q_I2C_ADDR);
	status = i2c_write(dev, msg->checksum_1, 1, SAM_M8Q_I2C_ADDR);
	status = i2c_write(dev, msg->checksum_2, 1, SAM_M8Q_I2C_ADDR);
	if (status != 0) { printk("Error in function send_ubx_message() in file sam-m8q.c\n"); }
	return status;
}

void compute_checksum(struct ubx_message* msg) {
	// 8-bit fletcher algorithm
	uint8_t checksum_1 = 0;
	uint8_t checksum_2 = 0;
	for (uint8_t i=2; i<6; i++) {
		checksum_1 += (uint8_t) ((uint8_t*)msg)[i];
		checksum_2 += checksum_1;
	}
	for (uint8_t i=0; i<(msg->length); i++) {
		checksum_1 += (uint8_t) msg->payload[i];
		checksum_2 += checksum_1;
	}
	msg->checksum_1 = checksum_1;
	msg->checksum_2 = checksum_2;
}








