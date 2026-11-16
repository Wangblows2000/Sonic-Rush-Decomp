#include <nitro/types.h>

typedef struct UnkStruct_022d4c30_b {
    u8 pad[0x1320];
    u16 unk1320;
} UnkStruct_022d4c30_b;

typedef struct UnkStruct_022d4c30 {
    u8 pad[0x24];
    s32 unk24;
    u8 pad1[0x92 - 0x28];
    u8 unk92;
} UnkStruct_022d4c30;

extern UnkStruct_022d4c30_b *data_ov000_02369ae0;
extern s32 data_ov000_0236511c[];

BOOL func_ov000_022d4b5c(UnkStruct_022d4c30 *s, s32 a, s32 b);

void func_ov000_022d4c30(UnkStruct_022d4c30 *s) {
    switch (data_ov000_02369ae0->unk1320) {
    case 0:
        if (func_ov000_022d4b5c(s, 0x1e, data_ov000_0236511c[0]))
            data_ov000_02369ae0->unk1320++;
        break;
    case 1:
        if (func_ov000_022d4b5c(s, 0x1d, data_ov000_0236511c[1]))
            data_ov000_02369ae0->unk1320++;
        break;
    case 2:
        if (func_ov000_022d4b5c(s, 0x1c, data_ov000_0236511c[2])) {
            data_ov000_02369ae0->unk1320 = 0;
            s->unk24 = 0;
            s->unk92 = 0;
        }
        break;
    }
}
