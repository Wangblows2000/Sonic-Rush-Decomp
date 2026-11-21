#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    u8 pad4[8];
    s16 v;
} UnkSub_02047378;

typedef struct {
    u8 pad[0x10];
    s8 unk10;
} UnkDisp_02047378;

extern UnkDisp_02047378 gDisplayMain;
void ClearDemoBg_State_BeginSlide(void *);

#pragma thumb on
void ClearDemoBg_State_Open(void *o, UnkSub_02047378 *s) {
    s->v += 0x199;
    if (s->v >= 0x1000) {
        s->v = 0x1000;
        s->func = ClearDemoBg_State_BeginSlide;
    }
    gDisplayMain.unk10 = ((fx32)(((s64)s->v * 0xfe000 + 0x800) >> 12) + 0x1000) >> 12;
}
#pragma thumb reset
