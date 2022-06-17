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

/* Arbitrarily chosen value since it is a
 * convenient round number. */
#define MAX_UBX_PAYLOAD_LENGTH 100
/* The length of the entire message is equal to
 * the payload length plus 1 byte for the class, 1
 * byte for the id, 2 bytes for the length, 2 bytes
 * for the CRC, and 2 bytes for the sync chars. */
#define MAX_UBX_MESSAGE_LENGTH (MAX_UBX_PAYLOAD_LENGTH + 8)

/* Copied from this implementation:
 * https://github.com/melopero/Melopero_UBX/blob/master/src/Melopero_UBX.h */
typedef struct {
    uint8_t msgClass;
    uint8_t msgId;
    uint16_t length;
    uint8_t payload[MAX_UBX_PAYLOAD_LENGTH];
    uint8_t checksumA;
    uint8_t checksumB;
} UbxMessage;

/* Copied from this implementation:
 * https://github.com/melopero/Melopero_UBX/blob/master/src/Melopero_UBX.h */
typedef struct {
 uint32_t itow;
 uint16_t year;
 uint8_t month;
 uint8_t day;
 uint8_t hour;
 uint8_t min;
 uint8_t sec;
 uint8_t validTimeFlag; // 7 | 6 | 5 | 4 | 3:validMag | 2:fullyResolved | 1:validTime | 0:validDate
 uint32_t timeAccuracy; //nanoseconds
 int32_t nano; //nanoseconds
 uint8_t fixType;
 uint8_t fixStatusFlags;
 uint8_t additionalFlags;
 uint8_t numberOfSatellites;
 int32_t longitude; //degrees
 int32_t latitude; //degrees
 int32_t height; //mm
 int32_t hMSL; //mm
 uint32_t horizontalAccuracy;
 uint32_t verticalAccuracy;
 int32_t velocityNorth; //mm/s
 int32_t velocityEast; //mm/s
 int32_t velocityDown; //mm/s
 int32_t groundSpeed; //mm/s
 int32_t headingOfMotion; //degrees
 uint32_t speedAccuracy; //mm/s
 int32_t headingAccuracy; //mm/s
 uint16_t positionDOP;
 uint8_t reserved;
 int32_t headingOfVehicle;
 int16_t magneticDeclination;
 uint16_t declinationAccuracy;
} PVTData;

void computeChecksum(UbxMessage *msg);

void send_ubx_msg(struct device *dev, UbxMessage *msg);

UbxMessage* receive_ubx_msg(struct device *dev);

UbxMessage* create_ubx_msg(uint8_t class, uint8_t id, uint16_t length, uint8_t *payload_buf);

PVTData* sam_m8q_get_pvt_data(struct device *dev);

#endif