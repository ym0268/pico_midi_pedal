#include "hardware/gpio.h"

#include "midi_gpio.h"

bool midi_gpio_init(void)
{
    /* pedal */
    gpio_init(PIN_PEDAL_0);
    gpio_set_dir(PIN_PEDAL_0, GPIO_IN);
    gpio_pull_up(PIN_PEDAL_0);

    /* pedal type */
    gpio_init(PIN_PEDAL_TYPE);
    gpio_set_dir(PIN_PEDAL_TYPE, GPIO_IN);
    gpio_pull_up(PIN_PEDAL_TYPE);

    /* Status LED */
    gpio_init(PIN_STATUS_LED);
    gpio_set_dir(PIN_STATUS_LED, GPIO_OUT);
}

bool get_pedal_is_normally_closed(void)
{
    return !gpio_get(PIN_PEDAL_TYPE);
}

bool get_pedal_is_open(void)
{
    return gpio_get(PIN_PEDAL_0);
}

void status_led_write(bool led_state)
{
    gpio_put(PIN_STATUS_LED, led_state);
}