#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/sensor.h>
#include <stdio.h>

#define DEVICE_NODE bmp388_i2c_node

void main(void) {

	printk("BMP388 sensor application.\n");

	const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(DEVICE_NODE));
	struct sensor_value press_val;
	struct sensor_value temp_val;

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

		if (sensor_channel_get(dev, SENSOR_CHAN_PRESS, &press_val) != 0) {
			printk("Sensor channel get fail.\n");
			continue;
		}

		if (sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp_val) != 0) {
			printk("Sensor channel get fail.\n");
			continue;
		}

		/* Look at the functions bmp388_press_channel_get() and 
		 * bmp388_temp_channel_get() in the driver file located at:
		 * zephyr/drivers/sensor/bmp388/bmp388.c.
		 * Those two functions describe the procedure for converting
		 * between the raw pressure/temperature values and the final values. */
		float pressure = (float) press_val.val1 + ((float) press_val.val2 / 1000000.0);
		float temperature = (float) temp_val.val1 + ((float) temp_val.val2 / 1000000.0);
		printk("Pressure:    %f kilopascal.\n", pressure);
		printk("Temperature: %f degrees Celsius.\n", temperature);
	
	}

}