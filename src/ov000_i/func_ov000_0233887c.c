#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[0x20 - 4];
    s16 unk20;
    u8 pad22[0x3a - 0x22];
    s8 unk3A;
    u8 pad3B[0x64 - 0x3b];
} UnkPlayer_0233887c;

typedef struct {
    u8 pad0[0x328];
    s16 unk328;
    u8 pad32A[0x420 - 0x32a];
    u16 unk420;
} UnkObj_0233887c;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    void *unkC;
} UnkStruct_0233887c;

extern UnkPlayer_0233887c data_ov000_0236aad8[];
extern UnkObj_0233887c *data_ov000_0236ac60[];
void func_ov000_02338850(void);

void func_ov000_0233887c(UnkStruct_0233887c *w) {
    UnkPlayer_0233887c *p = data_ov000_0236aad8;
    UnkObj_0233887c *o;

    if (p[2].flags & 0x40000) {
        w->unk8 = 0;
        w->unkC = func_ov000_02338850;
        p[2].flags |= 0x20000;
        return;
    }
    o = data_ov000_0236ac60[p->unk3A];
    if (!(o->unk420 & 0x80) || (o->unk328 != 3 && o->unk328 != 4)) {
        w->unk8 = 0;
        w->unkC = func_ov000_02338850;
        data_ov000_0236aad8[2].flags |= 0x20000;
        return;
    }
    w->unk8 = (p->unk20 << 10) >> 8;
}
