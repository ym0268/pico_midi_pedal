/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "bsp/board.h"
#include "tusb.h"

#include "midi_app.h"
#include "midi_gpio.h"

#if CFG_TUH_MIDI

static bool pedal_is_normally_closed = false;

void pedal_task(void);
void led_blinking_task(void);
uint32_t get_blinking_time_ms(void);

/*------------- MAIN -------------*/
int main(void)
{
    board_init();

    printf("TinyUSB Host MIDI Example\r\n");

    tusb_init();
    midi_gpio_init();

    /* set pedal type */
    pedal_is_normally_closed = get_pedal_is_normally_closed();

    while (1)
    {
        // tinyusb host task
        tuh_task();
        led_blinking_task();
    
        pedal_task();
        midi_host_app_task();
    }

    return 0;
}

#endif

//--------------------------------------------------------------------+
// Pedal Task
//--------------------------------------------------------------------+
void pedal_task(void)
{
    uint8_t             buf[3];
    static  uint32_t    pedal_task_start_ms = 0;
    const   uint32_t    interval_ms = 10;
    static  bool        stored_pedal_on = false;
    bool                pedal_on;

    if(!get_midi_tx_is_active()){
        return;
    }
    if(board_millis() - pedal_task_start_ms < interval_ms){
        return;
    }
    pedal_task_start_ms += interval_ms;

    /* get pedal state */
    pedal_on = !get_pedal_is_open();     /* normally open: open=off, normally close: open=on */
    if(pedal_is_normally_closed){
        pedal_on = !pedal_on;
    }

    if(stored_pedal_on == pedal_on){
        return;
    }
    stored_pedal_on = pedal_on;

    /* create sustain message */
    buf[0] = 0xB0;                      /* Control Change (0ch) */
    buf[1] = 0x40;                      /* Sustain pedal on/off */
    buf[2] = pedal_on ? 0x7F : 0x00;    /* ON/OFF               */

    midi_stream_write(buf, sizeof(buf));
}

//--------------------------------------------------------------------+
// Blinking Task
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
    uint32_t interval_ms = get_blinking_time_ms();
    static uint32_t start_ms = 0;

    static bool led_state = false;

    // Blink every interval ms
    if ( board_millis() - start_ms < interval_ms) return; // not enough time
    start_ms += interval_ms;

    status_led_write(led_state);
    led_state = !led_state; // toggle
}

uint32_t get_blinking_time_ms(void)
{
    MIDI_STATUS status;
    uint32_t t = 10;

    status = get_midi_status();
    switch(status){
    case MIDI_STATUS_MOUNTED:
        t = 1000;
        break;
    case MIDI_STATUS_NOT_MOUNTED:
    default:
        t = 200;
        break;
    }
    return t;
}