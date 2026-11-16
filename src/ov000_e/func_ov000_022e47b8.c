#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10C];
    u8 unk10C;
} UnkStruct_Mdl960;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x960 - 0x210];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD5E - 0x960 - sizeof(UnkStruct_Mdl960)];
    u16 unkD5E;
    u8 padD60[0x1118 - 0xD60];
    s32 unk1118;
    s32 unk111C;
} UnkStruct_Boss;

void func_ov000_022e03f4(UnkStruct_Boss *w);
void func_ov000_022e035c(UnkStruct_Boss *w);
void func_ov000_022df9d4(s32 a);
void func_ov000_022e1b48(s32 a);

void func_ov000_022e47b8(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;

    w->unk111C--;
    if (w->unk111C == 0)
        func_ov000_022e03f4(w);
    if (!(m->unk10C & 2))
        return;
    func_ov000_022e035c(w);
    w->unkD5E ^= 1;
    func_ov000_022e1b48(2);
    w->unk20C &= ~4;
}
