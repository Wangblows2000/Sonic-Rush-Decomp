#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10C];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10D];
    s32 unk114;
} UnkStruct_Mdl960;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x960 - 0x20C];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD08 - 0x960 - sizeof(UnkStruct_Mdl960)];
    void *unkD08;
    u8 padD0C[0xF22 - 0xD0C];
    u16 unkF22;
    u8 padF24[0x1128 - 0xF24];
    s32 unk1128;
} UnkStruct_Boss;

void func_02031a68(void *s, s32 a, void *b, s32 c, s32 d);
void func_ov000_022dff1c(UnkStruct_Boss *w);
void func_02036754(s32 a, s32 b, s32 c);
void func_ov000_022e3204(void);
void func_ov000_022e29dc(void);

void func_ov000_022e2ab4(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;

    if (!(m->unk10C & 2))
        return;
    w->unk1128--;
    if (w->unk1128 != 0) {
        w->base.unk68 = func_ov000_022e3204;
        func_02031a68(m, 0, w->unkD08, 0x10, 0);
        m->unk10C &= ~4;
        m->unk114 = 0x1000;
    } else {
        w->base.unk68 = func_ov000_022e29dc;
        func_02031a68(m, 0, w->unkD08, 0x16, 0);
        m->unk10C &= ~4;
        m->unk114 = 0x1000;
        w->base.unk14 = 0x20000;
        func_ov000_022dff1c(w);
        func_02036754(0, 0, 0x84);
    }
    w->unkF22 &= ~4;
    w->unkF22 |= 0x1000;
}
