#include <nitro/types.h>
#include "game/GameSystem/task.h"

extern UnkTask *gTaskListMainHead;
extern UnkTask *gTaskListMainTail;
extern UnkTask *gTaskListVBlankHead;
extern UnkTask *gTaskListVBlankTail;

void Task_Destroy(UnkTask *t);

void Task_DestroyRange(u16 lo, u16 hi) {
    UnkTask *t;
    if (lo <= 0xefff) {
        t = gTaskListMainHead->next;
        while (t != gTaskListMainTail && t->id < lo) {
            t = t->next;
        }
        while (t != gTaskListMainTail && t->id <= hi) {
            Task_Destroy(t);
            t = t->next;
        }
    }
    if (hi >= 0xf000) {
        t = gTaskListVBlankHead->next;
        while (t != gTaskListVBlankTail && t->id < lo) {
            t = t->next;
        }
        while (t != gTaskListVBlankTail && t->id <= hi) {
            Task_Destroy(t);
            t = t->next;
        }
    }
}
