# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-cas-core INTERFACE)

target_sources(mbed-cas-core
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-cas-core
    INTERFACE
        .
)

target_link_libraries(mbed-cas-core INTERFACE mbed-stm32f401xe)
