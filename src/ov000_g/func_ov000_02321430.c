#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x2E - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
} UnkStruct_02321430;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[0x62 - 8];
    u16 unk62;
} UnkStruct_02321430_S;

extern UnkStruct_02321430_S data_ov000_0236aad8;

void func_ov000_02321378(void);

void func_ov000_02321430(UnkStruct_02321430 *w) {
    s32 min;

    w->unk68 = func_ov000_02321378;
    w->unk5C |= 4;
    w->unk58 |= 0x100;
    if (data_ov000_0236aad8.unk4 + 0x8000 > w->unk10)
        w->unk5C &= ~1;
    else
        w->unk5C |= 1;
    w->unk2E = 0x2a0;
    w->unk30 = 0xff - (Rand() & 0x1fe);
    if (w->unk5C & 1)
        w->unk2E = -w->unk2E;
    min = (data_ov000_0236aad8.unk62 + 8) << 8;
    if (w->unk14 < min)
        w->unk14 = min;
}
