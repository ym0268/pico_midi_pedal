
//--------------------------------------------------------------------+
// GPIO PIN DEFINITION (GPIO NUMBER)
//--------------------------------------------------------------------+

/* Pedal */
#define PIN_PEDAL_0     15

/* Pedal Type */
#define PIN_PEDAL_TYPE  10

/* Status LED */
#define PIN_STATUS_LED  25          /* Onboard LED */

//--------------------------------------------------------------------+
// FUNCTIONS
//--------------------------------------------------------------------+
bool midi_gpio_init(void);
bool get_pedal_is_normally_closed(void);
bool get_pedal_is_open(void);
void status_led_write(bool led_state);