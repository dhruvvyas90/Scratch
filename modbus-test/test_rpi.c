/*
 * Copyright © 2008-2014 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus.h>

#define SERVER_ID 1
#define SERIAL_PORT "/dev/ttyAMA0"
#define BAUD_RATE 19200
#define PARITY 'N'
#define BYTE_SIZE 8
#define STOP_BITS 1

#define RPI_PIN 18

int main(int argc, char *argv[])
{
    uint16_t *tab_rp_registers = NULL;
    modbus_t *ctx = NULL;
    uint32_t sec_to = 1;
    uint32_t usec_to = 0;
    int i;
    int rc;
    int nb_points = 1;
    ctx = modbus_new_rtu(SERIAL_PORT, BAUD_RATE, PARITY, BYTE_SIZE, STOP_BITS);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }
    modbus_set_debug(ctx, TRUE);
    modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL);
    modbus_set_slave(ctx, SERVER_ID);
    modbus_get_response_timeout(ctx, &sec_to, &usec_to);
    modbus_enable_rpi(ctx,TRUE);
    modbus_configure_rpi_bcm_pin(ctx,RPI_PIN);
    modbus_rpi_pin_export_direction(ctx);


    //modbus_get_response_timeout(ctx, &old_response_to_sec, &old_response_to_usec);
    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
    // modbus_get_response_timeout(ctx, &new_response_to_sec, &new_response_to_usec);
    /* Allocate and initialize the memory to store the registers */
    tab_rp_registers = (uint16_t *) malloc(nb_points * sizeof(uint16_t));
    memset(tab_rp_registers, 0, nb_points * sizeof(uint16_t));
    rc = modbus_read_registers(ctx, 97,1, tab_rp_registers);
    printf("Date received is : %d\n",tab_rp_registers[0]);
    free(tab_rp_registers);

    /* Close the connection */
    modbus_rpi_pin_unexport_direction(ctx);
    modbus_close(ctx);
    modbus_free(ctx);
}
