#include <nitro/types.h>

typedef struct { u8 pad0[4]; u32 flags; u32 flags2; } UnkWork_02062340;
typedef struct { u8 pad0[0x10]; UnkWork_02062340 *work; } UnkTask_02062340;
typedef struct { u8 pad0[8]; u8 a; u8 b; u8 c; } UnkStruct_0208923c;
extern UnkTask_02062340 *data_022c4da8;
extern void *gTaskCurrent;
extern UnkStruct_0208923c data_0208923c;

void Task_Destroy(void *t);

#pragma thumb on
void func_02062340(void) {
    UnkWork_02062340 *w;

    if (data_022c4da8 == NULL) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    w = data_022c4da8->work;
    if (w->flags & 1) {
        UnkStruct_0208923c *g = &data_0208923c;
        if (w->flags & 4) {
            g->a |= 4;
            g->b |= 4;
            g->c |= 4;
            w->flags &= ~4;
            w->flags &= ~2;
            w->flags &= ~1;
        } else if (w->flags2 & 1) {
            g->a |= 4;
            g->b |= 4;
            g->c |= 4;
        } else {
            g->a &= ~4;
            g->b &= ~4;
            g->c &= ~4;
        }
    }
}
#pragma thumb reset
