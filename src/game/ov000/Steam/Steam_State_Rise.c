#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_EffObj base;
    u8 pad134[0x2a8 - 0x134];
    u32 unk2A8;
} UnkWork_0231a5a0;

extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]

void Steam_State_Rise(UnkWork_0231a5a0 *w) {
    w->unk2A8++;
    w->base.unk2E = ((SIN((u16)(w->unk2A8 << 11)) << 7) >> 12) / 3;
    if (w->base.unk5C & 8)
        w->base.unk54 |= 4;
}
