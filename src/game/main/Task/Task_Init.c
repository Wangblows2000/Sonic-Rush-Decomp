#include <nitro/types.h>
#include "main_a2/task.h"

extern u32 gTaskPoolUsed;
extern UnkTask gTaskPool[];
extern UnkTask *gTaskFreeList[];
extern UnkTask *gTaskListMainHead;
extern UnkTask *gTaskListMainTail;
extern UnkTask *gTaskListVBlankHead;
extern UnkTask *gTaskListVBlankTail;
extern UnkTask *gTaskCurrent;

UnkTask *Task_AllocNode(void);
void MIi_CpuClear32(u32 data, void *dst, u32 size);

void Task_Init(void) {
    s32 i;
    {
        vu32 zero = 0;
        gTaskPoolUsed = 0;
        MIi_CpuClear32(zero, gTaskPool, sizeof(UnkTask) * 256);
    }
    for (i = 0; i < 256; i++) {
        gTaskFreeList[i] = &gTaskPool[i];
    }
    gTaskListMainHead = Task_AllocNode();
    gTaskListMainTail = Task_AllocNode();
    gTaskListVBlankHead = Task_AllocNode();
    gTaskListVBlankTail = Task_AllocNode();
    gTaskListMainHead->prev = NULL;
    gTaskListMainHead->next = gTaskListMainTail;
    gTaskListMainHead->func = NULL;
    gTaskListMainHead->dtor = NULL;
    gTaskListMainHead->prio = 0x20;
    gTaskListMainHead->id = 0;
    gTaskListMainHead->unk1a = 0xffff;
    gTaskListMainTail->prev = gTaskListMainHead;
    gTaskListMainTail->next = gTaskListVBlankHead;
    gTaskListMainTail->func = NULL;
    gTaskListMainTail->dtor = NULL;
    gTaskListMainTail->prio = 0x20;
    gTaskListMainTail->id = 0xefff;
    gTaskListMainTail->unk1a = 0xffff;
    gTaskListVBlankHead->prev = gTaskListMainTail;
    gTaskListVBlankHead->next = gTaskListVBlankTail;
    gTaskListVBlankHead->func = NULL;
    gTaskListVBlankHead->dtor = NULL;
    gTaskListVBlankHead->prio = 0x20;
    gTaskListVBlankHead->id = 0xf000;
    gTaskListVBlankHead->unk1a = 0xffff;
    gTaskListVBlankTail->prev = gTaskListVBlankHead;
    gTaskListVBlankTail->next = NULL;
    gTaskListVBlankTail->func = NULL;
    gTaskListVBlankTail->dtor = NULL;
    gTaskListVBlankTail->prio = 0x20;
    gTaskListVBlankTail->id = 0xffff;
    gTaskListVBlankTail->unk1a = 0xffff;
    gTaskCurrent = gTaskListMainHead;
}
