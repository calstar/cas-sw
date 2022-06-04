/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/i2c.h>

#define DEVICE_NODE sam_i2c_node
#define DEVICE_I2C_ADDR 0x77

void main(void) {

	const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(DEVICE_NODE));

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

	/*

	uint8_t buf [64];
	int rc;

	while (1) {

		rc = i2c_read(i2c_dev, buf, 1, DEVICE_I2C_ADDR);

	}

	*/

}