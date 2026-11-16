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

extern UnkDisp_02047378 data_0208922c;
void func_02047344(void *);

#pragma thumb on
void func_02047378(void *o, UnkSub_02047378 *s) {
    s->v += 0x199;
    if (s->v >= 0x1000) {
        s->v = 0x1000;
        s->func = func_02047344;
    }
    data_0208922c.unk10 = ((fx32)(((s64)s->v * 0xfe000 + 0x800) >> 12) + 0x1000) >> 12;
}
#pragma thumb reset
