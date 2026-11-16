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
    u8 padD0C[0xF4A - 0xD0C];
    s16 unkF4A;
    u8 padF4C[0xF58 - 0xF4C];
    s32 unkF58;
} UnkStruct_Boss;

void func_ov000_022e5c1c(UnkStruct_Boss *w);
void func_02031a68(void *s, s32 a, void *b, s32 c, s32 d);
void func_02036754(s32 a, s32 b, s32 c);
void func_ov000_022e4298(void);

void func_ov000_022e434c(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;

    func_ov000_022e5c1c(w);
    w->base.unk10 += w->unkF58;
    if (w->unkF58 != 0)
        return;
    if (w->unkF4A != 0)
        return;
    w->base.unk68 = func_ov000_022e4298;
    func_02031a68(m, 0, w->unkD08, 7, 0);
    m->unk10C &= ~4;
    m->unk114 = 0x1000;
    func_02036754(0, 0, 0x84);
}
