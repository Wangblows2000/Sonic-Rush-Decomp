#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    const u8 *table;
    u16 count;
    u8 padA[2];
    s32 unkc;
    s32 unk10;
    s16 alpha[32];
    u16 delay[32];
} UnkSub_02047164;

typedef struct {
    u8 pad0[0x344];
    u32 flags;
} UnkObj_02047164;

void ClearDemoName_State_Done(void *);

#pragma thumb on
void ClearDemoName_State_FadeIn(UnkObj_02047164 *o, UnkSub_02047164 *s) {
    u32 i;
    for (i = 0; i < s->count; i++) {
        if (s->delay[i] == 0) {
            s->alpha[i] += 0x333;
            if (s->alpha[i] > 0x1000)
                s->alpha[i] = 0x1000;
        } else {
            s->delay[i]--;
        }
    }
    if (s->alpha[s->count - 1] == 0x1000) {
        s->func = ClearDemoName_State_Done;
        o->flags |= 0x60400;
    }
}
#pragma thumb reset
