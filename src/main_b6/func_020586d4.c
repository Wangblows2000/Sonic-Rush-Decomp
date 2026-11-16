#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u8 b2;
    u8 pad3[0x55];
} UnkSub_020586d4;

typedef struct {
    u32 unk0;
    u8 pad4[8];
    UnkSub_020586d4 sub[2];
    u8 padbc[0x16];
    u16 tblD2[0x69];
    u8 pad1a4[4];
    u16 tbl1A8[0x14];
    u8 pad1d0[0x144];
} UnkSave_020586d4;

void MIi_CpuClear32(u32 data, void *dest, u32 size);
u32 func_020588bc(UnkSave_020586d4 *s);

void func_020586d4(UnkSave_020586d4 *s) {
    u16 i;
    UnkSub_020586d4 *sub;
    u16 *t;

    {
        vu32 z = 0;
        MIi_CpuClear32(z, s, sizeof(UnkSave_020586d4));
    }
    sub = s->sub;
    sub[0].b2 = 2;
    sub[1].b2 = 2;
    t = s->tblD2;
    for (i = 0; i < 0x69; i++)
        t[i] = 0xffff;
    t = s->tbl1A8;
    for (i = 0; i < 0x14; i++)
        t[i] = 0xffff;
    s->unk0 = func_020588bc(s);
}
