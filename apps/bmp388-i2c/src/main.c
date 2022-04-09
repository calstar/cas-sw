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
#include <drivers/bmp388.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the &i2c alias. */
#define I2C0_NODE DT_ALIAS(cas-i2c0)

#if DT_NODE_HAS_STATUS(I2C0_NODE, okay)
#define I2C0	DT_GPIO_LABEL(I2C0_NODE, gpios)
#define PIN	DT_GPIO_PIN(I2C0_NODE, gpios)
#define FLAGS	DT_GPIO_FLAGS(I2C0_NODE, gpios)
#else

/* A build error here means your board isn't set up to communitcate in I2C */
#error "Unsupported board: cas-i2c0 devicetree alias is not defined"
#define I2C0	""
#define PIN	0
#define FLAGS	0
#endif

void main(void)
{
	const struct device *i2c_dev = DEVICE_DT_GET(I2C0);

	if (i2c_dev == NULL) {
		printk("Failed to get device binding");
	}

	void* pressuer_data;
	// FIXME: Find device address on i2c bus for the BMP388 Node
	// Documentation: https://docs.zephyrproject.org/3.0.0/reference/devicetree/bindings/sensor/bosch%2Cbmp388-i2c.html#dtbinding-bosch-bmp388-i2c
	while (1) {
		bmp388_io_ops.read(i2c_dev, pressuer_data, ,10);
		printk(pressuer_data);
	}
}
