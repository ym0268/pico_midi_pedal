#include <stdbool.h>

/* ---------------------- */
/* enum                   */
/* ---------------------- */
typedef enum {
    MIDI_STATUS_NOT_MOUNTED = 0,
    MIDI_STATUS_MOUNTED,
} MIDI_STATUS;


/* ---------------------- */
/* functions              */
/* ---------------------- */
void midi_host_app_task(void);
MIDI_STATUS get_midi_status(void);
bool get_midi_tx_is_active(void);
bool get_midi_rx_is_active(void);
uint32_t midi_stream_write(uint8_t const *buf, uint32_t bufsize);