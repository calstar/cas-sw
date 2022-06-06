/* u-blox SAM-M8Q GNSS module
 * 
 * Datasheet: https://content.u-blox.com/sites/default/files/SAM-M8Q_DataSheet_%28UBX-16012619%29.pdf
 * Protocol Specification: https://content.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf
 */

#include <device.h>

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

int sam_m8q_enable(struct device *dev);

int send_ubx_message(struct device *dev, struct ubx_message* msg);

void compute_checksum(struct ubx_message* msg);