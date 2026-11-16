#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022f89a8_plr;

typedef struct {
    u8 pad0[0x24];
    s32 unk24;
} UnkStruct_022c4534;

extern UnkStruct_022f89a8_plr *data_ov000_0236ac60;
extern UnkStruct_022c4534 data_022c4534;

BOOL func_ov000_022f89a8(void) {
    s32 v;

    v = (data_ov000_0236ac60->unk10 >> 8) + 0xc;
    if (v < data_022c4534.unk24)
        data_022c4534.unk24 = v;
    if (data_022c4534.unk24 > 0x340)
        return FALSE;
    data_022c4534.unk24 = 0x340;
    return TRUE;
}
