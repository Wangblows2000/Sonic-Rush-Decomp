#include <nitro/types.h>

typedef struct UnkVAlarm {
    void (*handler)(void *); /* 0x00 */
    void *arg;               /* 0x04 */
    u8 pad8[4];
    u32 frame;               /* 0x0c */
    s16 fire;                /* 0x10 */
    s16 delay;               /* 0x12 */
    struct UnkVAlarm *prev;  /* 0x14 */
    struct UnkVAlarm *next;  /* 0x18 */
    u32 period;              /* 0x1c */
    u8 pad20[4];
    u32 canceled;            /* 0x24 */
} UnkVAlarm;

extern UnkVAlarm *data_0207d394;

void func_02007374(UnkVAlarm *alarm);
void func_0200749c(UnkVAlarm *alarm);

void func_020074e8(UnkVAlarm *alarm)
{
    UnkVAlarm *prev;
    UnkVAlarm *next;

    for (next = data_0207d394; next; next = next->next) {
        if (next->frame < alarm->frame) {
            continue;
        }
        if (next->frame == alarm->frame && next->fire <= alarm->fire) {
            continue;
        }
        prev = next->prev;
        alarm->prev = prev;
        alarm->next = next;
        next->prev = alarm;
        if (prev) {
            prev->next = alarm;
        } else {
            data_0207d394 = alarm;
            func_02007374(alarm);
        }
        return;
    }
    func_0200749c(alarm);
}
