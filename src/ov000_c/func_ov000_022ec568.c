#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x3D4 - 0x20C];
    s32 unk3D4;
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x214 - 0x20C];
    UnkStruct_Boss *unk214;
} UnkStruct_022ec568;

void func_ov000_022ee198(UnkStruct_Boss *p, s32 a);
void func_ov000_022e9b70(UnkStruct_Boss *p, s32 a, s32 b);
void func_ov000_022ec564(void);

void func_ov000_022ec568(UnkStruct_022ec568 *w) {
    UnkStruct_Boss *p = w->unk214;
    s32 st;

    p->base.unk10 += p->base.unk2E;
    if (p->base.unk2E < 0) {
        if (-p->base.unk2E > 10)
            p->base.unk2E = p->base.unk2E + 10;
        else
            p->base.unk2E = 0;
    } else if (p->base.unk2E > 0) {
        if (p->base.unk2E > 10)
            p->base.unk2E = p->base.unk2E - 10;
        else
            p->base.unk2E = 0;
    }
    st = w->unk214->unk3D4;
    if (st == 5 && (p->base.unk5C & 8)) {
        func_ov000_022ee198(p, 6);
        p->base.unk5C |= 4;
    } else if (p->base.unk2E < -0x40 && st == 6 && (p->base.unk5C & 8)) {
        func_ov000_022ee198(p, 7);
        p->base.unk5C &= ~4;
    } else if (st == 7 && (p->base.unk5C & 8)) {
        func_ov000_022ee198(p, 0x3c);
        p->base.unk5C |= 4;
    }
    func_ov000_022e9b70(p, 0, 0);
    if (w->unk214->unk3D4 != 0x3c)
        return;
    if (p->base.unk2E == 0)
        w->base.unk68 = func_ov000_022ec564;
}
