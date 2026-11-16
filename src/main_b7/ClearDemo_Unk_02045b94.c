#include <nitro/types.h>

typedef struct {
    u8 pad0[0x340];
    void (*func)(void *);
    u8 pad344[0x162e - 0x344];
    s16 v;
} UnkObj_02045b94;

typedef struct {
    u8 pad[8];
    s16 bright;
} UnkDisp_02045b94;

extern UnkDisp_02045b94 data_02087d44;
extern UnkDisp_02045b94 data_0208926c;
void ClearDemo_Unk_02045b7c(void *);

#pragma thumb on
void ClearDemo_Unk_02045b94(UnkObj_02045b94 *o) {
    o->v += 0xcc;
    if (o->v >= 0x1000) {
        o->v = 0x1000;
        o->func = ClearDemo_Unk_02045b7c;
    }
    data_02087d44.bright = (fx32)(((s64)o->v * 0x10000 + 0x800) >> 12) >> 12;
    data_0208926c.bright = data_02087d44.bright;
}
#pragma thumb reset
