/* u-blox SAM-M8Q GNSS module
 * 
 * Datasheet: https://content.u-blox.com/sites/default/files/SAM-M8Q_DataSheet_%28UBX-16012619%29.pdf
 * Protocol Specification: https://content.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf
 */

#include <device.h>
#include <drivers/i2c.h>

#ifndef SAM_M8Q_HEADER
#define SAM_M8Q_HEADER

#define SAM_M8Q_I2C_ADDR 0x42

#define SYNC_CHAR_1 0xB5
#define SYNC_CHAR_2 0x62

#define NAV_CLASS 0x01
#define RXM_CLASS 0x02
#define INF_CLASS 0x04
#define ACK_CLASS 0x05
#define CFG_CLASS 0x06
#define UPD_CLASS 0x09
#define MON_CLASS 0x0A
#define AID_CLASS 0x0B
#define TIM_CLASS 0x0D
#define ESF_CLASS 0x10
#define MGA_CLASS 0x13
#define LOG_CLASS 0x21
#define SEC_CLASS 0x27
#define HNR_CLASS 0x28

#define NAV_POSLLH 0x02
#define NAV_PVT 0x07

#define ACK_ACK 0x01
#define ACK_NAK 0x00

#define CFG_PRT 0x00
#define CFG_MSG 0x01
#define CFG_RATE 0x08

#define NO_FIX 0
#define DEAD_RECKONING_ONLY 1
#define TWO_D_FIX 2
#define THREE_D_FIX 3
#define GNSS_DEAD_RECKONING_COMBINED 4
#define TIME_ONLY_FIX 5

/* This value was arbitrarily chosen since it is a convenient
 * round number. Also, the sc18is600 can only send I2C messages
 * with a maximum of 96 bytes, so the maximum ubx message length
 * needs to be well under 96. */
#define MAX_UBX_PAYLOAD_LENGTH 64

typedef struct {
    uint8_t msgClass;
    uint8_t msgId;
    uint16_t length;
    uint8_t payload[MAX_UBX_PAYLOAD_LENGTH];
    uint8_t checksumA;
    uint8_t checksumB;
} UbxMessage;

typedef struct {
	int32_t iTOW;
	int32_t lon;
	int32_t lat;
	int32_t height;
	int32_t hMSL;
	int32_t hAcc;
	int32_t vAcc;
} Position;

void computeChecksum(UbxMessage *msg);

int send_ubx_msg(struct device *dev, UbxMessage *msg);

int receive_ubx_msg(struct device *dev, UbxMessage *msg);

UbxMessage* create_ubx_msg(uint8_t class, uint8_t id, uint16_t length, uint8_t *payload);

int sam_m8q_get_position(struct device *dev, uint8_t *position_buf);

#endif