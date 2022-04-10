# Example

# add_subdirectory_ifdef(CONFIG_SERIAL_HAS_DRIVER serial)
# add_subdirectory_ifdef(CONFIG_SYS_CLOCK_EXISTS timer)

zephyr_library()
zephyr_library_sources_ifdef(CONFIG_BNO055_driver bno055.c)