#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10C];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10D];
    s32 unk114;
} UnkStruct_Mdl960;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x960 - 0x210];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD08 - 0x960 - sizeof(UnkStruct_Mdl960)];
    void *unkD08;
    u8 padD0C[0xD60 - 0xD0C];
    UnkStruct_Coll colls[11];
    u8 padF44[0xF4C - 0xD60 - 11 * 0x2C];
    s16 unkF4C;
    u8 padF4E[0xF68 - 0xF4E];
    u16 unkF68;
    u16 unkF6A;
    u16 unkF6C;
    u8 padF6E[0x1118 - 0xF6E];
    s32 unk1118;
} UnkStruct_Boss;

void func_02031a68(void *s, s32 a, void *b, s32 c, s32 d);
void func_ov000_022df9d4(s32 a);
void func_ov000_022dff1c(UnkStruct_Boss *w);
void func_02036754(s32 a, s32 b, s32 c);
void func_ov000_022e285c(void);

void func_ov000_022e291c(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;

    if (m->unk10C & 2) {
        w->base.unk68 = func_ov000_022e285c;
        func_02031a68(m, 0, w->unkD08, 0x16, 0);
        m->unk10C &= ~4;
        m->unk114 = 0x1000;
        func_ov000_022dff1c(w);
        func_02036754(0, 0, 0x84);
    }
    w->unk1118++;
    func_ov000_022df9d4(w->unk1118);
}
