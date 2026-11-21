#include <nitro/types.h>

typedef struct {
    u8 pad0[0x134];
    s32 unk134[5];
} UnkWork_0232af54;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0232af54 *work;
} UnkTask_0232af54;

extern UnkTask_0232af54 *gTaskCurrent;

void MarkerTimeList_DrawRow(UnkWork_0232af54 *w, s32 a, s32 b, s32 c, u16 i);

void MarkerTimeList_Draw(void) {
    u16 i;
    UnkWork_0232af54 *w = gTaskCurrent->work;

    for (i = 0; i < 5; i++)
        MarkerTimeList_DrawRow(w, 0x800, i * 0x1000 + 0x7c00, w->unk134[i], i);
}
