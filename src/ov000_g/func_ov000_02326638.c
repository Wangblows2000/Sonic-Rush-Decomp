#include <ov000_b/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x10 - 1];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x40 - 0x18];
    s16 unk40;
    s16 unk42;
    u8 pad44[0x4A - 0x44];
    s16 unk4A;
    u8 pad4C[0x54 - 0x4C];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x6C - 0x5E];
    void *unk6C;
    u8 pad70[4];
    void *unk74;
    void *unk78;
    u8 pad7C[0x134 - 0x7C];
    UnkStruct_Coll unk134;
    u8 pad160[0x184 - 0x160];
    UnkStruct_ObjParam *unk184;
    s32 unk188;
    s32 unk18C;
} UnkStruct_02326638;

extern u8 data_ov000_023685ec[];

void func_ov000_023265a8(void);
void func_ov000_02326538(void);
void func_ov000_023264dc(void);

void func_ov000_02326638(UnkStruct_02326638 *w, UnkStruct_ObjParam *p, s32 x, s32 y) {
    w->unk6C = func_ov000_023265a8;
    w->unk54 |= 0x40;
    w->unk74 = func_ov000_02326538;
    w->unk78 = func_ov000_023264dc;
    w->unk188 = x >> 8;
    w->unk18C = y >> 8;
    w->unk10 = x;
    w->unk14 = y;
    w->unk0 = 4;
    w->unk40 = 0x2a;
    w->unk42 = 0xf00;
    w->unk54 |= 1;
    w->unk58 |= 0x200;
    func_02068e40(w, -2, -6, 2, 0);
    w->unk5C |= 1;
    w->unk134.unk28 = 0x12;
    func_0206e2b4(&w->unk134, 0, 10);
    w->unk134.unkA &= ~4;
    if (p != NULL) {
        w->unk184 = p;
        w->unk184->unk0 = 0xff;
    }
    w->unk4A = 0x108 - data_ov000_023685ec[p->unk2];
}
