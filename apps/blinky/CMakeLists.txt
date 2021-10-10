# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../cas-zephyr-drivers/zephyr)
list(APPEND SOC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../cas-zephyr-drivers/zephyr)
list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../cas-zephyr-drivers/zephyr)
list(APPEND ZEPHYR_EXTRA_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/../../cas-zephyr-drivers/zephyr)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(blinky)

target_sources(app PRIVATE src/main.c)
