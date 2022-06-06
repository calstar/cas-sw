/* u-blox SAM-M8Q GNSS module
 * 
 * Datasheet: https://content.u-blox.com/sites/default/files/SAM-M8Q_DataSheet_%28UBX-16012619%29.pdf
 * Protocol Specification: https://content.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf
 */

#include <device.h>

#define SAM_M8Q_I2C_ADDR 0x42
#define DATA_REGISTER 0xFF

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

//********* NAV MESSAGE SECTION **********
#define NAV_PVT 0x07

//********* ACK MESSAGE SECTION **********
#define ACK_ACK 0x01
#define ACK_NAK 0x00

//********* CFG MESSAGE SECTION **********
#define CFG_PRT 0x00
#define CFG_MSG 0x01
#define CFG_RATE 0x08

//********* FixTypes SECTION **********
#define NO_FIX 0
#define DEAD_RECKONING_ONLY 1
#define TWO_D_FIX 2
#define THREE_D_FIX 3
#define GNSS_DEAD_RECKONING_COMBINED 4
#define TIME_ONLY_FIX 5

#define MAX_MESSAGE_LENGTH 64

typedef struct {
   uint8_t msgClass;
   uint8_t msgId;
   uint16_t length;
   uint8_t payload[MAX_MESSAGE_LENGTH]; //uint8_t* payload;
   uint8_t checksumA;
   uint8_t checksumB;
} UbxMessage;

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