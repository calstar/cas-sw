#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/sensor.h>

#define DEVICE_NODE bmp388_i2c_node

void main(void) {

	printk("BMP388 sensor application.\n");

	const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(DEVICE_NODE));
	struct sensor_value pressure_value;
	struct sensor_value temperature_value;

	if (dev == NULL) {
		printk("Failed to get device binding.\n");
		return;
	}

	if (!device_is_ready(dev)) {
		printk("Device is not ready.\n");
		return;
	}

	while (1) {

		k_msleep(100);

		if (sensor_sample_fetch(dev) != 0) {
			printk("Sensor sample fetch fail.\n");
			continue;
		}

		if (sensor_channel_get(dev, SENSOR_CHAN_PRESS, &pressure_value) != 0) {
			printk("Sensor channel get fail.\n");
			continue;
		}

		if (sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temperature_value) != 0) {
			printk("Sensor channel get fail.\n");
			continue;
		}

		printk("Pressure:    %d kilopascal.\n", pressure_value.val1);
		printk("Temperature: %d degrees Celsius.\n", temperature_value.val1);

	}

}