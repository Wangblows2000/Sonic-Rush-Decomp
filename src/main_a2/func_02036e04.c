#include <nitro/types.h>
#include "main_a2/task.h"

extern u32 data_02087cb8;
extern UnkTask *data_022b4574;

void func_02036e04(UnkTask *t, UnkTask *end) {
    u32 level = data_02087cb8 & 7;
    for (data_022b4574 = t = t->next; t != end; data_022b4574 = t = t->next) {
        if (!(t->flags & 1) && t->func != NULL) {
            if (data_02087cb8 & 0x80000000) {
                if ((t->prio & 0x10) || level < (t->prio & 7)) {
                    t->func();
                }
            } else {
                t->func();
            }
        }
    }
}
