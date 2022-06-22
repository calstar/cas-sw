SD Logger
######

The purpose of this application is to take in arbitrary data collected during flight and write it to an SD card in the SD card slot on the core board, which is connected to the SPI3 bus.

Here is a relevant sample application to base our code on: https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/subsys/fs/fat_fs

Here are relevant pages from the zephyr documentation to read:
https://docs.zephyrproject.org/latest/services/storage/disk/access.html
https://docs.zephyrproject.org/latest/services/file_system/index.html
