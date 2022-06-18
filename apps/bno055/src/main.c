/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/sensor.h>

#define BNO055_I2C_BUS_WRITE_ARRAY_INDEX ((u8)1)
#define I2C_BUFFER_LEN 8
#define I2C0           5

s8 BNO055_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {

    s32 BNO055_iERROR = BNO055_INIT_VALUE;
    u8 array[I2C_BUFFER_LEN] = { BNO055_INIT_VALUE };
    u8 stringpos = BNO055_INIT_VALUE;

    array[BNO055_INIT_VALUE] = reg_addr;

    for (stringpos = BNO055_INIT_VALUE; stringpos < cnt; stringpos++) {
        *(reg_data + stringpos) = array[stringpos];
    }

    return (s8)BNO055_iERROR;

}

s8 BNO055_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
	
	s32 BNO055_iERROR = BNO055_INIT_VALUE;
    u8 array[I2C_BUFFER_LEN];
    u8 stringpos = BNO055_INIT_VALUE;

    array[BNO055_INIT_VALUE] = reg_addr;
    for (stringpos = BNO055_INIT_VALUE; stringpos < cnt; stringpos++) {
        array[stringpos + BNO055_I2C_BUS_WRITE_ARRAY_INDEX] = *(reg_data + stringpos);
    }

}

s8 I2C_routine(void) {

    bno055.bus_write = BNO055_I2C_bus_write;
    bno055.bus_read = BNO055_I2C_bus_read;
    bno055.delay_msec = BNO055_delay_msek;
    bno055.dev_addr = BNO055_I2C_ADDR1;
    return BNO055_INIT_VALUE;
}

void main(void) {

	// Define starting values

    struct bno055_t bno055;

    s32 comres = BNO055_ERROR;
    u8 power_mode = BNO055_INIT_VALUE;

    s16 gyro_datax = BNO055_INIT_VALUE;
    s16 gyro_datay = BNO055_INIT_VALUE;
    s16 gyro_dataz = BNO055_INIT_VALUE;
    struct bno055_gyro_t gyro_xyz;

    double d_gyro_datax = BNO055_INIT_VALUE;
    double d_gyro_datay = BNO055_INIT_VALUE;
    double d_gyro_dataz = BNO055_INIT_VALUE;
    struct bno055_gyro_double_t d_gyro_xyz;

    // Initialize the BNO055 device

    I2C_routine();

    comres = bno055_init(&bno055);
    power_mode = BNO055_POWER_MODE_NORMAL;
    comres += bno055_set_power_mode(power_mode);

    // Read the raw sensor data

    comres += bno055_set_operation_mode(BNO055_OPERATION_MODE_GYRONLY);

    comres += bno055_read_gyro_x(&gyro_datax);
    comres += bno055_read_gyro_y(&gyro_datay);
    comres += bno055_read_gyro_z(&gyro_dataz);
    comres += bno055_read_gyro_xyz(&gyro_xyz);

    // Read the converted sensor data

    comres += bno055_convert_double_gyro_x_dps(&d_gyro_datax);
    comres += bno055_convert_double_gyro_y_dps(&d_gyro_datay);
    comres += bno055_convert_double_gyro_z_dps(&d_gyro_dataz);
    comres += bno055_convert_double_gyro_x_rps(&d_gyro_datax);
    comres += bno055_convert_double_gyro_y_rps(&d_gyro_datay);
    comres += bno055_convert_double_gyro_z_rps(&d_gyro_dataz);
    comres += bno055_convert_double_gyro_xyz_dps(&d_gyro_xyz);
    comres += bno055_convert_double_gyro_xyz_rps(&d_gyro_xyz);

    // De-Initialize the DNO055 device

    power_mode = BNO055_POWER_MODE_SUSPEND;
    comres += bno055_set_power_mode(power_mode);

    // 



}
