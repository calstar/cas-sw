/* u-blox SAM-M8Q GNSS module
 * 
 * Datasheet: https://content.u-blox.com/sites/default/files/SAM-M8Q_DataSheet_%28UBX-16012619%29.pdf
 * Protocol Specification: https://content.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf
 */

#include <device.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>

#define SAM_M8Q_I2C_ADDR 0x42

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

#define SYNC_CHAR_1 0xB5
#define SYNC_CHAR_2 0x62

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

/* Note: The sc18is600 has a maximum I2C message length of 96 bytes,
 * so the payload of the ubx messages should be a lot smaller than
 * 96 bytes, since the ubx message contains more than just the payload. 
 * 64 was chosen because it is a convenient power of 2. */
#define MAX_MESSAGE_LENGTH 64

typedef struct {
   uint8_t msgClass;
   uint8_t msgId;
   uint16_t length;
   uint8_t payload[MAX_MESSAGE_LENGTH];
   uint8_t checksumA;
   uint8_t checksumB;
} UbxMessage;

typedef struct {
	int32_t longitude;
	int32_t latitude;
} Position;

void computeChecksum(UbxMessage *msg);

void resetPayload(UbxMessage *msg);

int send_ubx_msg(struct device *dev, struct spi_config *cfg, UbxMessage *msg);

int sam_m8q_enable(struct device *dev, struct spi_config *cfg);

int receive_ubx_msg(struct device *dev, struct spi_config *cfg, uint8_t length, uint8_t *buf);

Position* sam_m8q_get_position(struct device *dev, struct spi_config *cfg);



