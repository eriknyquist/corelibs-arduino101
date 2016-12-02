#ifndef WSRC_H_
#define WSRC_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "board.h"
#include "pins_arduino.h"

#define GPIO_START                     0
#define GPIO_END                       (NUM_DIGITAL_PINS - 1)

#define GPIO_MODE_MASK                 0x18
#define SRC_STAT(src)                  (wsrc_table[src].status)

#define MAX_ATTACHABLE                 10
#define wsrc_cansleep(stat)            (stat & 1)
#define wsrc_working(stat)             (stat & 2)
#define wsrc_wakeup(stat)              (stat & 4)
#define wsrc_set_cansleep(stat)        (stat |= 1)
#define wsrc_clear_cansleep(stat)      (stat &= ~1)
#define wsrc_set_working(stat)         (stat |= 2)
#define wsrc_clear_working(stat)       (stat &= ~2)
#define wsrc_set_wakeup(stat)          (stat |= 4)
#define wsrc_clear_wakeup(stat)        (stat &= ~4)
#define wsrc_gpio_mode(stat)           ((stat & GPIO_MODE_MASK) >> 3)
#define wsrc_set_gpio_mode(stat, mode) (stat |= (mode << 3) & ~(GPIO_MODE_MASK))

enum {
    /* TODO: not all of these can actually be used as wakeup sources.
     * find out which ones they are. */
    BLE_WAKEUP = NUM_DIGITAL_PINS,
    I2S_WAKEUP = NUM_DIGITAL_PINS + 1,
    IMU_WAKEUP = NUM_DIGITAL_PINS + 2,
    WIRE_RX_WAKEUP = NUM_DIGITAL_PINS + 3,
    SERIAL_WAKEUP = NUM_DIGITAL_PINS + 4,
    SERIAL1_WAKEUP = NUM_DIGITAL_PINS + 5,
    ADC_WAKEUP = NUM_DIGITAL_PINS + 6,
    AON_TIMER_WAKEUP = NUM_DIGITAL_PINS + 7,
    TIMER1_WAKEUP = NUM_DIGITAL_PINS + 8,
    PWM_TIMER_WAKEUP = NUM_DIGITAL_PINS + 9,
    SPI_RX_WAKEUP = NUM_DIGITAL_PINS + 10,
    SPI1_RX_WAKEUP = NUM_DIGITAL_PINS + 11,
    RTC_WAKEUP = NUM_DIGITAL_PINS + 12,
    WATCHDOG_WAKEUP = NUM_DIGITAL_PINS + 13,
    MAILBOX_WAKEUP = NUM_DIGITAL_PINS + 14,
    COMPARATORS_WAKEUP = NUM_DIGITAL_PINS + 15,
    NUM_WAKEUP = NUM_DIGITAL_PINS + 16
};

struct wsrc_entry {
    /* IRQ associated with this wakeup source */
    uint8_t irq;
    /* Status flags;
     *
     * Bit 0:  sleep:         indicates if this wakeup source can be used in
     *                        deeper sleep modes (if unset, can only be used in
     *                        doze mode)
     * Bit 1:  working:       indicates this entry is waiting for something
     *                        to complete in interrupt context (e.g.
     *                        interrupt-based UART transfer in progress)
     * Bit 2:  wakeup:        indicates this entry is to be used as a wakeup
     *                        source
     * Bits 3-5: GPIO mode:   0 - LOW
     *                        1 - HIGH
     *                        2 - CHANGE
     *                        3 - FALLING
     *                        4 - RISING
     *
     * Bits 6-7: Reserved.    */
    volatile uint8_t status;
} __attribute__((packed));

struct wsrc_active {
    uint32_t cb;
    uint8_t index;
} __attribute__((packed));

struct wsrc {
    uint32_t callback;
    uint8_t status;
    uint8_t irq;
    uint8_t id;
};

typedef struct wsrc wsrc_t;
typedef struct wsrc_entry wsrc_entry_t;
typedef struct wsrc_active wsrc_active_t;

extern wsrc_entry_t wsrc_table[NUM_WAKEUP];

void wsrc_table_init (void);
void wsrc_register_gpio (uint32_t pin, void (*callback)(void), uint32_t mode);
void wsrc_register_id (int id, void (*callback)(void));
void wsrc_unregister (int id);

int wsrc_get_next_active (wsrc_t *wsrc);

#ifdef __cplusplus
}
#endif

#endif
