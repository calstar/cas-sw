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

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000
#define I2C_DEVICE_ADDRESS     b1110111
#define DATA_READ_LENGTH 				10

/* The devicetree node identifier for the &i2c alias. */
#define I2C1_NODE DT_NODELABEL(i2c1)

// #if DT_NODE_HAS_STATUS(I2C0_NODE, okay)
// #define I2C0	DT_GPIO_LABEL(I2C0_NODE, gpios)
// #define PIN	DT_GPIO_PIN(I2C0_NODE, gpios)
// #define FLAGS	DT_GPIO_FLAGS(I2C0_NODE, gpios)
// #else
//
// /* A build error here means your board isn't set up to communitcate in I2C */
// #error "Unsupported board: cas-i2c0 devicetree alias is not defined"
// #define I2C0	""
// #define PIN	0
// #define FLAGS	0
// #endif

void main(void) {
	const struct device *i2c_dev = device_get_binding(DT_LABEL(I2C1_NODE));
	struct sensor_value val;
	uint32_t pressure = 0U;


	printk("BMP388 sensor application\n");

	if (i2c_dev == NULL) {
		printk("Failed to get device binding");
	}

	while (1) {
		if (sensor_sample_fetch(i2c_dev)) {
			printk("sensor: sample fetch fail.\n");
			return;
		}

		if (sensor_channel_get(i2c_dev, SENSOR_CHAN_PRESS, &val) != 0) {
			printk("sensor: channel get fail.\n");
			return;
		}

		pressure = val.val1;
		printk("sensor: pressure reading: %d\n", pressure);

		// bmp388_io_ops.read(i2c_dev, pressuer_data, I2C_DEVICE_ADDRESS, READ_DATA_LENGTH);
		// printk(pressuer_data);
		k_msleep(SLEEP_TIME_MS);
	}

}
