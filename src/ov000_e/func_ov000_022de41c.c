#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u8 *unk4;
} UnkStruct_022de51c_R;

typedef struct {
    u8 pad0[4];
    UnkStruct_022de51c_R *unk4;
    u32 unk8;
    u8 padC[0x24 - 0xC];
    s32 unk24;
    u8 pad28[0x92 - 0x28];
    u8 unk92;
    u8 pad93[0xAE - 0x93];
    u8 unkAE;
} UnkStruct_022de51c;

typedef struct {
    u8 pad0[0xAF4];
    u32 unkAF4;
    u8 padAF8[0xB88 - 0xAF8];
    u8 unkB88[4];
} UnkStruct_022de51c_M;

extern UnkStruct_022de51c_M *data_ov000_02369b2c;
extern u8 data_ov000_023635b0[];

s32 func_02025edc(void *dict, void *name);
void func_ov000_022de5e8(void *a, UnkStruct_022de51c *o, s32 b);

void func_ov000_022de41c(UnkStruct_022de51c *o) {
    UnkStruct_022de51c_M *m = data_ov000_02369b2c;
    s32 id;

    if (o->unk8 & 0x10)
        id = o->unkAE;
    else
        id = -1;
    if (id != func_02025edc(o->unk4->unk4 + 0x40, data_ov000_023635b0))
        return;
    func_ov000_022de5e8(m->unkB88, o, 0x10);
    if ((m->unkAF4 & 0x10) == 0x10) {
        o->unk24 = 0;
        o->unk92 = 0;
    }
}
