#ifndef WSRC_H_
#define WSRC_H_

#define AON_GPIO_START                 26
#define AON_GPIO_END                   28

#define GPIO_MODE_MASK                 0x18
#define SRC_STAT(src)                  (wsrc_table[src].status)

#define wsrc_enabled(stat)             (stat & 1)
#define wsrc_working(stat)             (stat & 2)
#define wsrc_wakeup(stat)              (stat & 4)
#define wsrc_set_enabled(stat)         (stat |= 1)
#define wsrc_clear_enabled(stat)       (stat &= ~1)
#define wsrc_set_working(stat)         (stat |= 2)
#define wsrc_clear_working(stat)       (stat &= ~2)
#define wsrc_set_wakeup(stat)          (stat |= 4)
#define wsrc_clear_wakeup(stat)        (stat &= ~4)
#define wsrc_gpio_mode(stat)           ((stat & GPIO_MODE_MASK) >> 3)
#define wsrc_set_gpio_mode(stat, mode) (stat |= (mode << 3) & ~(GPIO_MODE_MASK))

enum {
    /* TODO: not all of these can actually be used as wakeup sources.
     * find out which ones they are. */
    BLE_WAKEUP = 0,
    I2S_WAKEUP = 1,
    IMU_WAKEUP = 2,
    SPI_WAKEUP = 3,
    WIRE_WAKEUP = 4,
    SERIAL_WAKEUP = 5,
    SERIAL1_WAKEUP = 6,
    ADC_WAKEUP = 7,
    NUM_WAKEUP = 8
};

#define NUM_WAKEUP_SOURCES            (NUM_WAKEUP + (AON_GPIO_END - \
                                       AON_GPIO_START) + 1)

#ifdef __cplusplus
 extern "C" {
#endif

struct wsrc_entry {
    /* User callback function, void (*)(void) */
    uint32_t cb;

    /* Status flags;
     *
     * Bit 0:  enabled:       libs must set this in "begin()", and clear it
     *                        in "end()"
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

typedef struct wsrc_entry wsrc_entry_t;

extern wsrc_entry_t *wsrc_table;

void wsrc_table_init (void);
void wsrc_register_gpio (uint32_t pin, void (*callback)(void), uint32_t mode);
void wsrc_unregister_gpio (uint32_t pin);

void wsrc_register_id (int id, void (*callback)(void));
void wsrc_unregister_id (int id);

wsrc_entry_t *wsrc_get_next_active (int *id);

#ifdef __cplusplus
}
#endif

#endif
