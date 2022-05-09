/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

 // Documentation: https://docs.zephyrproject.org/3.0.0/reference/devicetree/bindings/sensor/bosch%2Cbmp388-i2c.html#dtbinding-bosch-bmp388-i2c

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <drivers/sensor.h>
#include <drivers/i2c.h>

// This needs to match the 'label' given in the overlay file
#define BMP388_NODELABEL "BMP388_I2C"

void main(void) {

	const struct device *i2c_dev = device_get_binding(BMP388_NODELABEL);
	struct sensor_value val;
	struct sensor_value attr;

	printk("BMP388 sensor application.\n");

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

		k_msleep(500);

		/*
		if (sensor_attr_get(i2c_dev, SENSOR_CHAN_PRESS, SENSOR_ATTR_SAMPLING_FREQUENCY, &attr) != 0) {
			printk("Could not get frequency attribute.\n");
			continue;
		} else {
			printk("Frequency attribute: %d\n.", attr.val1);
		}
		*/

		if (sensor_sample_fetch(i2c_dev) != 0) {
			printk("Sensor sample fetch fail.\n");
			continue;
		} else {
			printk("Sensor sample fetch succeeded.\n");
		}

		if (sensor_channel_get(i2c_dev, SENSOR_CHAN_PRESS, &val) != 0) {
			printk("Sensor channel get fail.\n");
			continue;
		} else {
			printk("Sensor channel get succeeded.\n");
		}

		printk("sensor: pressure reading: %d\n kilopascal.", val.val1);

	}

}
