#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    const u8 *table;
    u16 count;
    u8 padA[2];
    s32 unkc;
    s32 unk10;
    s32 work[20];
    u16 unk64;
    u16 unk66;
} UnkSub_02046ef4;

extern u16 data_020733dc[];
void ClearDemoActText_State_Hold(void *);

#pragma thumb on
void ClearDemoActText_State_Spread(void *o, UnkSub_02046ef4 *s) {
    u32 i;
    s32 acc;
    BOOL done;
    acc = 0;
    done = TRUE;
    for (i = 0; i < s->count; i++) {
        s->work[i] += 0x2000;
        if (s->work[i] >= acc)
            s->work[i] = acc;
        else
            done = FALSE;
        acc += data_020733dc[s->table[i]] << 12;
    }
    if (done) {
        s->unk66 = 0;
        s->func = ClearDemoActText_State_Hold;
    }
}
#pragma thumb reset
