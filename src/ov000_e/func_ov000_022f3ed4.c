#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[4];
    s32 unk4;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
    u8 pad20[4];
    s32 unk24;
} UnkStruct_022c4534;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    u16 unk434;
} UnkStruct_022f3ed4;

extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern UnkStruct_022c4534 data_022c4534;

void func_ov000_022f4ce4(UnkStruct_022f3ed4 *s, s32 a, s32 b, s32 c);
void func_ov000_022f9044(UnkStruct_022f3ed4 *s);
void func_ov000_022f4cbc(UnkStruct_022f3ed4 *s);
void func_ov000_022f9188(void);
void func_ov000_022f3e9c(void);

void func_ov000_022f3ed4(UnkStruct_022f3ed4 *s) {
    func_ov000_022f4ce4(s, 6, 0x1000, 1);
    func_ov000_022f9044(s);
    func_ov000_022f4cbc(s);
    s->base.unk5C |= 1;
    func_ov000_022f9188();
    data_022c4534.unk1C = 0;
    data_022c4534.unk24 = 0x340;
    s->unk434 = (Rand() & 0x3f) + 0x1e;
    data_ov000_02369ac0.unk4 = 0x2f;
    s->base.unk68 = func_ov000_022f3e9c;
}
