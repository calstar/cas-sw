/* Application to communicate with SAM-M8Q GNSS module and get GPS readings.
 * The CPU sends SPI messages to the SC18IS600 translator module, and this
 * translator module sends I2C messages to the SAM-M8Q module. 
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/i2c.h>
#include "sam_m8q.h"

#define DEVICE_NODE cas_i2c0

void main(void) {

	printk("--- SAM-M8Q GPS Module ---\n");

	const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(DEVICE_NODE));
	if (dev == NULL) {
		printk("Error in main.c: Failed to get device binding.\n");
		return NULL;
	}
	if (!device_is_ready(dev)) {
		printk("Error in main.c: Device is not ready.\n");
		return NULL;
	}

	uint8_t position_buf[28] = { 0, 0, 0, 0, 0, 0, 0, 
								 0, 0, 0, 0, 0, 0, 0, 
								 0, 0, 0, 0, 0, 0, 0, 
								 0, 0, 0, 0, 0, 0, 0, };
	while (1) {
		k_msleep(1000);
		sam_m8q_get_position(dev, &position_buf);
		printk("Altitude: %X, %X, %X, %X\n", position_buf[16], position_buf[17], position_buf[18], position_buf[19]);
	}

	return;

}