#include <nitro/types.h>

typedef struct UnkStruct_022dbe6c_a {
    u8 pad[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad1[0x10c - 0x54];
    u8 unk10C;
    u8 pad2[0x138 - 0x10d];
} UnkStruct_022dbe6c_a;

typedef struct UnkStruct_022dbe6c UnkStruct_022dbe6c;
struct UnkStruct_022dbe6c {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x54 - 0x18];
    u32 unk54;
    u8 pad1[0x5c - 0x58];
    u16 unk5C;
    u8 pad2[0x20c - 0x5e];
    UnkStruct_022dbe6c_a unk20C;
    UnkStruct_022dbe6c_a unk344;
    s32 unk47C;
    u8 pad4[0x4d8 - 0x480];
    s32 unk4D8;
    s32 unk4DC;
    s32 unk4E0;
    s32 unk4E4;
    u16 unk4E8;
    u16 unk4EA;
    u16 unk4EC;
    u8 pad5[0x4f0 - 0x4ee];
    void (*unk4F0)(UnkStruct_022dbe6c *s);
    s32 unk4F4;
};

typedef struct UnkTask_022dbe6c {
    u8 pad[0x10];
    UnkStruct_022dbe6c *unk10;
} UnkTask_022dbe6c;

extern UnkTask_022dbe6c *gTaskCurrent;

extern s16 data_0206e470[];

void Model_Animate(UnkStruct_022dbe6c_a *a);
void Model_Draw(UnkStruct_022dbe6c_a *a);
void func_ov000_022db758(s32 *v, u16 *ang, s32 x, s32 y);
void MTX_RotY33_(void *m, s32 s, s32 c);

void Boss4PartD2_Draw(void) {
    UnkStruct_022dbe6c *s = gTaskCurrent->unk10;
    UnkStruct_022dbe6c_a *a;
    if (s->unk5C & 0x20)
        return;
    if (s->unk4D8 != 0) {
        a = &s->unk344;
        Model_Animate(a);
        Model_Draw(a);
        return;
    }
    a = &s->unk20C;
    Model_Animate(a);
    func_ov000_022db758(&a->unk48, &s->unk4EA, s->unk4E0, s->unk14);
    s->unk4EA += 0x4000;
    MTX_RotY33_(s->unk20C.unk24, data_0206e470[((s->unk4EA & 0xffff) >> 4) * 2], data_0206e470[((s->unk4EA & 0xffff) >> 4) * 2 + 1]);
    Model_Draw(a);
    func_ov000_022db758(&a->unk48, &s->unk4EC, s->unk4E4, s->unk14);
    s->unk4EC -= 0x4000;
    MTX_RotY33_(s->unk20C.unk24, data_0206e470[((s->unk4EC & 0xffff) >> 4) * 2], data_0206e470[((s->unk4EC & 0xffff) >> 4) * 2 + 1]);
    Model_Draw(a);
}
