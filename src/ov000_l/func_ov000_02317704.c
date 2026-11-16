#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 unk1C;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x1c];
    s32 unk1C;
    s32 unk20;
} UnkStruct_022c4590;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_022c4590 data_022c4590;

s32 func_ov000_02317704(u8 *e, s32 x, s32 y) {
    if (!(data_022c4560.unk18 & 2) && (!(data_022c4560.unk18 & 8) || data_022c4560.unk1C >= 7)) {
        data_022c4590.unk1C = x >> 8;
        data_022c4590.unk20 = y >> 8;
    }
    *e = 0xff;
    return 0;
}
