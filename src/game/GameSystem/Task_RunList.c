#include <nitro/types.h>
#include "game/GameSystem/task.h"

extern u32 gSystemFlags;
extern UnkTask *gTaskCurrent;

void Task_RunList(UnkTask *t, UnkTask *end) {
    u32 level = gSystemFlags & 7;
    for (gTaskCurrent = t = t->next; t != end; gTaskCurrent = t = t->next) {
        if (!(t->flags & 1) && t->func != NULL) {
            if (gSystemFlags & 0x80000000) {
                if ((t->prio & 0x10) || level < (t->prio & 7)) {
                    t->func();
                }
            } else {
                t->func();
            }
        }
    }
}
