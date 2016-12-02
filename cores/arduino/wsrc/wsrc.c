#include <stdint.h>
#include "dccm/dccm_alloc.h"
#include "wsrc.h"
#include "string.h"

#ifdef __cplusplus
 extern "C" {
#endif

static uint8_t active_index;
static uint8_t num_active;
static wsrc_active_t wsrc_active[MAX_ATTACHABLE];


wsrc_entry_t wsrc_table[NUM_WAKEUP] = {
    { .irq = IRQ_GPIO1_INTR, .status = 0 },     /* I00 */
    { .irq = IRQ_GPIO1_INTR, .status = 0 },     /* I01 */
    { .irq = IRQ_GPIO_INTR, .status = 0 },      /* I02 */
    { .irq = IRQ_GPIO1_INTR, .status = 0 },     /* I03 */
    { .irq = IRQ_GPIO_INTR, .status = 0 },      /* I04 */
    { .irq = IRQ_GPIO1_INTR, .status = 0 },     /* I05 */
    { .irq = IRQ_GPIO1_INTR, .status = 0 },     /* I06 */
    { .irq = IRQ_GPIO_INTR, .status = 0 },      /* I07 */
    { .irq = IRQ_GPIO_INTR, .status = 0 },      /* I08 */
    { .irq = IRQ_GPIO1_INTR, .status = 0 },     /* I09 */
    { .irq = IRQ_GPIO_INTR, .status = 0 },      /* I010 */
    { .irq = IRQ_GPIO_INTR, .status = 0 },      /* I011 */
    { .irq = IRQ_GPIO_INTR, .status = 0 },      /* I012 */
    { .irq = IRQ_GPIO_INTR, .status = 0 },      /* I013 */
    { .irq = IRQ_GPIO0_INTR, .status = 0 },     /* I014 */
    { .irq = IRQ_GPIO0_INTR, .status = 0 },     /* I015 */
    { .irq = IRQ_GPIO0_INTR, .status = 0 },     /* I016 */
    { .irq = IRQ_GPIO0_INTR, .status = 0 },     /* I017 */
    { .irq = IRQ_GPIO0_INTR, .status = 0 },     /* I018 */
    { .irq = IRQ_GPIO0_INTR, .status = 0 },     /* I019 */
    { .irq = IRQ_GPIO0_INTR, .status = 0 },     /* I020 */
    { .irq = IRQ_GPIO_INTR, .status = 0 },      /* I021 */
    { .irq = IRQ_GPIO_INTR, .status = 0 },      /* I022 */
    { .irq = IRQ_GPIO_INTR, .status = 0 },      /* I023 */
    { .irq = IRQ_GPIO_INTR, .status = 0 },      /* I024 */
    { .irq = IRQ_GPIO_INTR, .status = 0 },      /* I025 */
    { .irq = IRQ_ALWAYS_ON_GPIO, .status = 1 }, /* IO26 */
    { .irq = IRQ_ALWAYS_ON_GPIO, .status = 1 }, /* IO27 */
    { .irq = IRQ_ALWAYS_ON_GPIO, .status = 1 }, /* IO28 */
    { .irq = IRQ_ALWAYS_ON_GPIO, .status = 1 }, /* BLE_WAKEUP */
    { .irq = IRQ_I2S_INTR, .status = 0 },       /* I2S_WAKEUP */
    { .irq = IRQ_ALWAYS_ON_GPIO, .status = 1 }, /* IMU_WAKEUP */
    { .irq = IRQ_I2C0_RX_AVAIL, .status = 0 },  /* WIRE_RX_WAKEUP */
    { .irq = IRQ_USB_INTR, .status = 0 },       /* SERIAL_WAKEUP */
    { .irq = IRQ_UART1_INTR, .status = 0 },     /* SERIAL1_WAKEUP */
    { .irq = IRQ_ADC_IRQ, .status = 0 },        /* ADC_WAKEUP */
    { .irq = IRQ_ALWAYS_ON_TMR, .status = 0 },  /* AON_TIMER_WAKEUP */
    { .irq = IRQ_TIMER1, .status = 0 },         /* TIMER1_WAKEUP */
    { .irq = IRQ_PWM_TIMER_INTR, .status = 0 }, /* PWM_TIMER_WAKEUP */
    { .irq = IRQ_SPI1_RX_AVAIL, .status = 0 },  /* SPI_RX_WAKEUP */
    { .irq = IRQ_SPI0_RX_AVAIL, .status = 0 },  /* SPI1_RX_WAKEUP */
    { .irq = IRQ_RTC_INTR, .status = 1 },       /* RTC_WAKEUP */
    { .irq = IRQ_WDOG_INTR, .status = 0 },      /* WATCHDOG_WAKEUP */
    { .irq = IRQ_MAILBOXES_INTR, .status = 0 }, /* MAILBOX_WAKEUP */
    { .irq = IRQ_COMPARATORS_INTR, .status = 1},/* COMPARATORS_WAKEUP*/
};

static void wsrc_set_active (uint8_t index, void (*cb)(void))
{
    if (num_active < MAX_ATTACHABLE) {
        wsrc_active[num_active].cb = (uint32_t)cb;
        wsrc_active[num_active++].index = index;
    }
}

static void wsrc_clear_active (uint8_t index)
{
    unsigned int i, j;

    for (i = 0; i < num_active; ++i) {

        /* Look for this value in wsrc_active ... */
        if (wsrc_active[i].index == index) {

            /* ... and shift over all remaining values that follow it */
            for (j = i + 1; j < num_active; ++j) {
                wsrc_active[j - 1] = wsrc_active[j];
            }

            --num_active;
            break;
        }
    }
}

int wsrc_get_next_active (wsrc_t *wsrc)
{
    uint8_t i;

    if (active_index == num_active || !wsrc) {
        active_index = 0;
        return 0;
    }

    i = wsrc_active[active_index].index;

    wsrc->irq = wsrc_table[i].irq;
    wsrc->callback = wsrc_active[active_index].cb;
    wsrc->status = wsrc_table[i].status;
    wsrc->id = i;

    ++active_index;
    return 1;
}

void wsrc_register_gpio (uint32_t pin, void (*callback)(void), uint32_t mode)
{
    /* Set interrupt mode */
    wsrc_set_gpio_mode(SRC_STAT(pin), mode);

    if (!wsrc_wakeup(SRC_STAT(pin))) {
        wsrc_set_active(pin, callback);
        wsrc_set_wakeup(SRC_STAT(pin));
    }
}

void wsrc_register_id (int id, void (*callback)(void))
{
    if (!wsrc_wakeup(SRC_STAT(id))) {
        wsrc_set_active(id, callback);
        wsrc_set_wakeup(SRC_STAT(id));
    }
}

void wsrc_unregister (int id)
{
    wsrc_clear_active(id);
    wsrc_clear_wakeup(SRC_STAT(id));
}

#ifdef __cplusplus
}
#endif
