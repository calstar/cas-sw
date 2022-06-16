#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/sensor.h>

#define DEVICE_NODE bno055_i2c_node

void main(void) {

	const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(DEVICE_NODE));
	struct sensor_value val;

	printk("----- BNO055 sensor application -----\n");

	if (i2c_dev == NULL) {
		printk("Failed to get device binding.\n");
		return;
	} else {
		printk("Got device binding.\n");
	}

	if (!device_is_ready(i2c_dev)) {
		printk("Device is not ready.\n");
		return;
	} else {
		printk("Device is ready.\n");
	}

	while (1) {

		k_msleep(1000);

		if (sensor_sample_fetch(i2c_dev) != 0) {
			printk("Sensor sample fetch fail.\n");
			continue;
		} else {
			printk("Sensor sample fetch succeeded.\n");
		}

		if (sensor_channel_get(i2c_dev, SENSOR_CHAN_ACCEL_X, &val) != 0) {
			printk("Sensor channel get fail.\n");
			continue;
		} else {
			printk("Sensor channel get succeeded.\n");
		}

		printk("sensor accelaration reading: %d\n m/s^2.", val.val1);

	}

}
