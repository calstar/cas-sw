/* Application to communicate with SAM-M8Q GNSS module and get GPS readings.
 * The CPU sends SPI messages to the SC18IS600 translator module, and this
 * translator module sends I2C messages to the SAM-M8Q module.
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>

void main(void) {

	struct device *dev = get_spi_dev();
	if (dev == NULL) {
		printk("Error in main.c: Failed to get spi device.\n");
		return;
	}

	if (sam_m8q_enable(dev) != 0) {
		printk("Error in main.c: Failed to initialize sam-m8q.\n");
		return;
	}

	if (sam_m8q_sample_fetch(dev) != 0) {
		printk("Error in main.c: Failed to fetch data from sam-m8q.\n");
		return;
	}

	int status;
	int32_t latitude_val;

	// TODO: get latitude data

	return;

}