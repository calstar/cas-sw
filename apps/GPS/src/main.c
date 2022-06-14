/* Application to communicate with the SAM-M8Q GNSS module over 
 * I2C and get latitude, longitude, and altitude readings.
 * 
 * The cas-stack needs to have the I2C1 and I2C2 buses shorted together.
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
		return;
	}
	if (!device_is_ready(dev)) {
		printk("Error in main.c: Device is not ready.\n");
		return;
	}
	while (1) {
		k_msleep(1000);
		sam_m8q_get_position(dev);
	}

	return;

}