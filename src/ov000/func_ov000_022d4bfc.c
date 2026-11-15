#include <nitro/types.h>

typedef struct UnkStruct_022d4bfc {
    u8 pad[0x24];
    s32 unk24;
    u8 pad1[0x92 - 0x28];
    u8 unk92;
} UnkStruct_022d4bfc;

extern s32 data_ov000_02365100;

BOOL func_ov000_022d4b5c(UnkStruct_022d4bfc *s, s32 a, s32 b);

void func_ov000_022d4bfc(UnkStruct_022d4bfc *s) {
    if (func_ov000_022d4b5c(s, 0x1b, data_ov000_02365100)) {
        s->unk24 = 0;
        s->unk92 = 0;
    }
}
