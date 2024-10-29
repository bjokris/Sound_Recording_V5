
#include "seekTable.h"
#include "seek0.h"
#include "seek1.h"
#include "seek2.h"

/* create Seek table */
seekTable_t g_seekTable[ENUM_SEEK_MAX_INDEX] = {
    /* seek 0*/
    [ENUM_SEEK_0_INDEX] = {
        .data = {false, true, false, true, 0, {0}, 0},
        .func = {
            seek0_do_calc,
            seek0_checkWarningAlarm,
            seek0_sendToLora
        },
    },
    /* seek 1 */
    [ENUM_SEEK_1_INDEX] = {
        .data = {false, false, true, true, 0, {0}, 0},
        .func = {
            seek1_do_calc,
            seek1_checkWarningAlarm,
            seek1_sendToLora
        },
    },
    /* seek 2 */
    [ENUM_SEEK_2_INDEX] = {
        .data = {false, false, true, true, 0, {0}, 0},
        .func = {
            seek2_do_calc,
            seek2_checkWarningAlarm,
            seek2_sendToLora
        },
    }

};
void seekTable_init(void)
{

}