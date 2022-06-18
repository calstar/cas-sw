#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0 DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN	DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS DT_GPIO_FLAGS(LED0_NODE, gpios)

void main(void) {

	const struct device *dev;

	dev = device_get_binding(LED0);
	if (dev == NULL) {
		return;
	}

	if (gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS) != 0) {
		return;
	}

	bool led_is_on = true;
	while (1) {
		gpio_pin_set(dev, PIN, (int)led_is_on);
		led_is_on = !led_is_on;
		k_msleep(1000);
	}
}
