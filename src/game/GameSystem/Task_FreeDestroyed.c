#include <nitro/types.h>
#include "game/GameSystem/task.h"

extern UnkTask *gTaskListMainHead;
extern UnkTask *gTaskListMainTail;
extern UnkTask *gTaskListVBlankHead;
extern UnkTask *gTaskListVBlankTail;

void Task_FreeNode(UnkTask *t);

void Task_FreeDestroyed(void) {
    UnkTask *t;
    UnkTask *next;
    t = gTaskListMainHead->next;
    while (t != gTaskListMainTail) {
        next = t->next;
        if (t->flags & 1) {
            Task_FreeNode(t);
        }
        t = next;
    }
    t = gTaskListVBlankHead->next;
    while (t != gTaskListVBlankTail) {
        next = t->next;
        if (t->flags & 1) {
            Task_FreeNode(t);
        }
        t = next;
    }
}
