#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x224 - 0x20C];
    s32 unk224;
    u8 pad228[0x22C - 0x228];
    s16 unk22C;
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x42E - 0x20C];
    u16 unk42E;
    u8 pad430[0x440 - 0x430];
    void *unk440;
    u8 pad444[0x464 - 0x444];
    u16 unk464;
    u8 pad466[2];
    s16 unk468;
    u8 pad46A[2];
    s32 unk46C;
    s32 unk470;
} UnkStruct_022e822c;

extern UnkStruct_Boss *data_ov000_02369b78;
extern u16 data_ov000_02363c04[][4];
extern s16 data_ov000_02363c64[][4];

void func_ov000_022e8538(UnkStruct_022e822c *w);
UnkStruct_StageObj *MapObject_Spawn(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void func_ov000_022e8120(void);

void func_ov000_022e822c(UnkStruct_022e822c *w) {
    UnkStruct_Boss *b;
    UnkStruct_StageObj *o;

    if (w->base.unk30 >= 0) {
        w->base.unk30 = 0;
        w->base.unk58 &= ~0x80;
    }
    w->unk468 += 0x40;
    w->unk46C += 0x4cc;
    func_ov000_022e8538(w);
    if (w->unk46C > 0x30000)
        w->unk46C = 0x30000;
    if (w->unk468 > 0x1555)
        w->unk468 = 0x1555;
    if (w->unk46C != 0x30000)
        return;
    if (w->unk468 != 0x1555)
        return;
    b = data_ov000_02369b78;
    o = MapObject_Spawn(0xd6, b->unk224, 0x20000, 0, 0, 0, 0, 0, 0);
    o->unkC = data_ov000_02363c04[w->unk42E][w->unk464 - 1];
    w->unk470 = o->unkC;
    b->unk22C = data_ov000_02363c64[w->unk42E][w->unk464 - 1];
    w->unk440 = func_ov000_022e8120;
}
