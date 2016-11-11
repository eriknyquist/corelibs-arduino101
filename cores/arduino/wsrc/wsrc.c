#include <stdint.h>
#include "dccm/dccm_alloc.h"
#include "wsrc.h"
#include "string.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define idtoi(id) ((id < NUM_WAKEUP && id >= 0) ? id : 0)
#define gpiotoi(pin) ((pin >= AON_GPIO_START && pin <= AON_GPIO_END) ? \
                     (pin - AON_GPIO_START) + NUM_WAKEUP : NUM_WAKEUP)

static uint8_t active_index;
static uint8_t num_active;
static uint8_t *wsrc_active;
wsrc_entry_t *wsrc_table;

static void wsrc_set_active (uint8_t index)
{
    wsrc_active[num_active++] = index;
}

static void wsrc_clear_active (uint8_t index)
{
    unsigned int i, j;

    for (i = 0; i < num_active; ++i) {

        /* Look for this value in wsrc_active ... */
        if (wsrc_active[i] == index) {

            /* ... and shift over all remaining values that follow it */
            for (j = i + 1; j < num_active; ++j) {
                wsrc_active[j - 1] = wsrc_active[j];
            }

            --num_active;
            break;
        }
    }
}

wsrc_entry_t *wsrc_get_next_active (int *id)
{
    uint8_t curr;
    int wsrc_id;

    if (active_index == num_active) {
        active_index = 0;
        return NULL;
    }

    curr = wsrc_active[active_index++];
    wsrc_id = curr;

    if (curr >= NUM_WAKEUP) {
        wsrc_id = (curr - NUM_WAKEUP) + AON_GPIO_START;
    }

    if (id) *id = wsrc_id;
    return &wsrc_table[curr];
}

void wsrc_table_init (void)
{
    void *pool;
    unsigned int pool_size;
    unsigned int table_size;

    table_size = sizeof(wsrc_entry_t) * NUM_WAKEUP_SOURCES;
    pool_size = table_size + NUM_WAKEUP_SOURCES;
    pool = dccm_malloc(pool_size);
    memset(pool, 0, pool_size);

    wsrc_table = (wsrc_entry_t *)pool;
    wsrc_active = ((uint8_t *)pool) + table_size;

    num_active = 0;
    active_index = 0;

    /* GPIOs enabled by default (i.e. no GPIO.begin() method) */
    for (int i = NUM_WAKEUP; i < NUM_WAKEUP_SOURCES; ++i) {
        wsrc_set_enabled(SRC_STAT(i));
    }

    /* ADC enabled by default */
    wsrc_set_enabled(SRC_STAT(ADC_WAKEUP));
}

void wsrc_register_gpio (uint32_t pin, void (*callback)(void), uint32_t mode)
{
    unsigned int index;
    index = gpiotoi(pin);

    /* Set interrupt mode */
    wsrc_set_gpio_mode(SRC_STAT(index), mode);

    /* Save callback function */
    wsrc_table[index].cb = (uint32_t)callback;

    if (!wsrc_wakeup(SRC_STAT(index))) {
        wsrc_set_active(index);
        wsrc_set_wakeup(SRC_STAT(index));
    }
}

void wsrc_unregister_gpio (uint32_t pin)
{
    unsigned int index;

    index = gpiotoi(pin);
    wsrc_clear_active(index);
    wsrc_clear_wakeup(SRC_STAT(index));
}

void wsrc_register_id (int id, void (*callback)(void))
{
    unsigned int index;
    index = idtoi(id);

    /* Save callback function */
    wsrc_table[index].cb = (uint32_t)callback;

    if (!wsrc_wakeup(SRC_STAT(index))) {
        wsrc_set_active(index);
        wsrc_set_wakeup(SRC_STAT(index));
    }
}

void wsrc_unregister_id (int id)
{
    unsigned int index;

    index = idtoi(id);
    wsrc_clear_active(index);
    wsrc_clear_wakeup(SRC_STAT(index));
}

#ifdef __cplusplus
}
#endif
