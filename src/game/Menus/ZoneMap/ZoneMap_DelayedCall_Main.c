#include <nitro/types.h>

typedef struct { u8 pad[8]; u32 count; u32 limit; s32 arg; } UnkStruct_022e3150;
typedef struct { u8 pad[0x10]; UnkStruct_022e3150 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void ZoneMap_SetNaviMessage(void *p, s32 a);

#pragma thumb on
void ZoneMap_DelayedCall_Main(void) {
    UnkStruct_022e3150 *t = gTaskCurrent->p;
    if (t->limit != 0) {
        t->count++;
        if (t->count >= t->limit) {
            ZoneMap_SetNaviMessage(t, t->arg);
        }
    }
}
#pragma thumb reset
