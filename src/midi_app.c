/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021, Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdbool.h>
#include "bsp/board.h"
#include "tusb.h"
#include "class/midi/midi_host.h"

#include "midi_app.h"

static uint8_t midi_dev_addr = 0;
static MIDI_STATUS midi_status = MIDI_STATUS_NOT_MOUNTED;

static void midi_tx(void);
static void midi_rx(void);


void midi_host_app_task(void)
{
    midi_tx();
    midi_rx();
}

MIDI_STATUS get_midi_status(void)
{
    return midi_status;
}

bool get_midi_tx_is_active(void)
{
    if(!midi_dev_addr || !tuh_midi_configured(midi_dev_addr)){
        return false;
    }
    if(tuh_midih_get_num_tx_cables(midi_dev_addr) < 1){
        return false;
    }   
    return true;
}

bool get_midi_rx_is_active(void)
{
    if(!midi_dev_addr || !tuh_midi_configured(midi_dev_addr)){
        return false;
    }
    if(tuh_midih_get_num_rx_cables(midi_dev_addr) < 1){
        return false;
    }
    return true;
}

uint32_t midi_stream_write(uint8_t const *buf, uint32_t bufsize)
{
    uint32_t nwritten = 0;
    uint8_t  cable = 0;

    cable = tuh_midih_get_num_tx_cables(midi_dev_addr) - 1;
    nwritten = tuh_midi_stream_write(midi_dev_addr, cable, buf, bufsize);

    return nwritten;
}

static void midi_tx(void)
{
    if(!get_midi_tx_is_active()){
        return;
    }
    tuh_midi_stream_flush(midi_dev_addr);
}

static void midi_rx(void)
{
    if(!get_midi_rx_is_active()){
        return;
    }
    tuh_midi_read_poll(midi_dev_addr);
}

//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

// Invoked when device with hid interface is mounted
// Report descriptor is also available for use. tuh_hid_parse_report_descriptor()
// can be used to parse common/simple enough descriptor.
// Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE, it will be skipped
// therefore report_desc = NULL, desc_len = 0
void tuh_midi_mount_cb(uint8_t dev_addr, uint8_t in_ep, uint8_t out_ep, uint8_t num_cables_rx, uint16_t num_cables_tx)
{
    printf("MIDI device address = %u, IN endpoint %u has %u cables, OUT endpoint %u has %u cables\r\n",
        dev_addr, in_ep & 0xf, num_cables_rx, out_ep & 0xf, num_cables_tx);
    midi_dev_addr = dev_addr;
    midi_status = MIDI_STATUS_MOUNTED;
}

// Invoked when device with hid interface is un-mounted
void tuh_midi_umount_cb(uint8_t dev_addr, uint8_t instance)
{
    midi_dev_addr = 0;
    printf("MIDI device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
    midi_status = MIDI_STATUS_NOT_MOUNTED;
}

void tuh_midi_rx_cb(uint8_t dev_addr, uint32_t num_packets)
{
    if (midi_dev_addr == dev_addr)
    {
        if (num_packets != 0)
        {
        uint8_t cable_num;
        uint8_t buffer[48];
        uint32_t bytes_read = tuh_midi_stream_read(dev_addr, &cable_num, buffer, sizeof(buffer));
        TU_LOG1("Read bytes %u cable %u", bytes_read, cable_num);
        TU_LOG1_MEM(buffer, bytes_read, 2);

        /* loop back to midi device */
        tuh_midi_stream_write(dev_addr, cable_num, buffer, bytes_read);
        }
    }
}

void tuh_midi_tx_cb(uint8_t dev_addr)
{

}
