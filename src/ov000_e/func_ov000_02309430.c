#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x34];
    u32 unk34;
    u8 pad38[0x64 - 0x38];
} UnkStruct_0236aad8_E;

typedef struct {
    UnkStruct_0236aad8_E e[2];
    u32 unkC8;
} UnkStruct_0236aad8;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[4];
    u8 unk214;
} UnkStruct_02309430;

extern UnkStruct_0236aad8 data_ov000_0236aad8;

void func_ov000_0234232c(s32 a);
void func_ov000_023093f8(void);

void func_ov000_02309430(UnkStruct_02309430 *w) {
    w->unk20C &= ~8;
    w->unk20C |= 0x2000;
    w->unk20C |= 0x40;
    if (data_ov000_0236aad8.unkC8 & 1) {
        data_ov000_0236aad8.e[0].unk34 &= ~0x10000;
        data_ov000_0236aad8.e[1].unk34 &= ~0x10000;
    } else {
        if (w->unk20C & 2)
            data_ov000_0236aad8.e[0].unk34 &= ~0x10000;
        if (w->unk20C & 4)
            data_ov000_0236aad8.e[1].unk34 &= ~0x10000;
    }
    if (!(data_ov000_0236aad8.unkC8 & 1))
        func_ov000_0234232c(w->unk214);
    w->base.unk1F4 |= 0x10;
    w->base.unk68 = func_ov000_023093f8;
}
