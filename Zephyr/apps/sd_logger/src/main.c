#include <zephyr.h>
#include <device.h>
#include <storage/disk_access.h>
#include <fs/fs.h>
#include <ff.h>

void main(void) {
	
	static const char *disk_pdrv = "SD";

	if (disk_access_init(disk_pdrv) != 0) {
		printk("Storage init error.\n");
	}

	uint32_t block_count;
	if (disk_access_ioctl(disk_pdrv, DISK_IOCTL_GET_SECTOR_COUNT, &block_count)) {
		printk("Unable to get sector count");
	}

	return;

}