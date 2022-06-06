# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../cas-zephyr-drivers/zephyr)
list(APPEND SOC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../cas-zephyr-drivers/zephyr)
list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../cas-zephyr-drivers/zephyr)

# To add out-of-tree device drivers to the project, uncomment the following line.
# list(APPEND ZEPHYR_EXTRA_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/../../cas-zephyr-drivers/zephyr/drivers/my_driver)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Update the following line to contain the name of the current application.
project(GPS)

# If you have multiple source files, list all of them here.
target_sources(app PRIVATE src/main.c src/sam_m8q.c src/sc18is600.c src/sam_m8q.h)