/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <drivers/sensor.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000
#define I2C_DEVICE_ADDRESS     b1110111
#define DATA_READ_LENGTH 				10
#define I2C3_NODE DT_NODELABEL(i2c3)

void main(void) {
	// Warning: if bno055 and bmp388 both use I2C3, which device will i2c_dev this bind to?
	const struct device *i2c_dev = device_get_binding(DT_LABEL(I2C3_NODE));
	struct sensor_value gyro[3];


	printk("BNO055 sensor application\n");

	if (i2c_dev == NULL) {
		printk("Failed to get device binding");
	}

	while (1) {
		if (sensor_sample_fetch(i2c_dev) < 0) {
			printk("sensor: sample fetch fail.\n");
			return;
		}

		if (sensor_channel_get(i2c_dev, SENSOR_CHAN_GYRO_XYZ, gyro) < 0) {
			printk("sensor: channel get fail.\n");
			return;
		}

		printf("bno055: gyro (dps): x: %.3f, y: %.3f, z: %.3f\n",
			sensor_value_to_double(&gyro[0]),
			sensor_value_to_double(&gyro[1]),
			sensor_value_to_double(&gyro[2]));

		k_msleep(SLEEP_TIME_MS);
	}

}