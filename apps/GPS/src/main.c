#include <zephyr.h>
#include <device.h>
#include <devicetree.h>

void main(void) {

	struct device *dev = get_spi_dev();
	if (dev == NULL) {
		printk("Failed to get device binding.\n");
		return;
	}

	if (sam_m8q_enable(dev) != 0) {
		printk("Error in initializing sam-m8q.\n");
		return;
	}

	printk("Sucesfully initialized sam-m8q.\n");
	return;

}