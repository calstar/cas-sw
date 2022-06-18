/* Application to communicate with the SAM-M8Q GNSS module over 
 * I2C and get latitude, longitude, and altitude readings.
 * 
 * The cas-stack needs to have the I2C1 and I2C2 buses shorted together
 * for this application to work, because the sam_m8q is actually
 * attached to the I2C2 bus.
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/i2c.h>
#include "sam_m8q.h"

#define DEVICE_NODE cas_i2c0

void main(void) {

	printk("--- SAM-M8Q GPS Module ---\n");

	const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(DEVICE_NODE));
	if (i2c_dev == NULL) {
		printk("Error in main.c: Failed to get device binding.\n");
		return;
	}
	if (!device_is_ready(i2c_dev)) {
		printk("Error in main.c: Device is not ready.\n");
		return;
	}

	PVTData *pvt;

	while(1) {

		k_msleep(1000);
	
		pvt = sam_m8q_get_pvt_data(i2c_dev);

		printk("Year: %d, Month: %d, Day: %d\n", pvt->year, pvt->month, pvt->day);
		printk("Hour: %d, Minute: %d, Second: %d\n", pvt->hour, pvt->min, pvt->sec);
		printk("Latitude: %d degrees, Longitude: %d degrees\n", (pvt->latitude)/10000000, (pvt->longitude)/10000000);
		printk("Height: %d meters, Height above sea level: %d meters\n", (pvt->height)/1000, (pvt->hMSL)/1000);
		printk("Time/Date valid?: %d", (pvt->validTimeFlag & 0x03 != 0));
		printk("\n\n");

	}

	return;

}