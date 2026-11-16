#include <nitro/types.h>

typedef struct UnkStruct_022dcd80 {
    u8 pad[0x21c];
    u32 unk21C;
    u8 pad1[0xbd0 - 0x220];
    s32 unkBD0;
    s32 unkBD4;
    s32 unkBD8;
} UnkStruct_022dcd80;

typedef struct UnkStruct_022dcd80_7 {
    u8 pad[8];
    u16 unk8;
} UnkStruct_022dcd80_7;

extern UnkStruct_022dcd80_7 gBossStatus;
extern u8 data_ov000_02363550[];

void func_ov000_022dbb18(s32 a);
void func_ov000_022dccc8(UnkStruct_022dcd80 *s);
void func_ov000_022dcc10(UnkStruct_022dcd80 *s);
BOOL func_ov000_022db704(u32 mask, s32 n);

void func_ov000_022dcd80(UnkStruct_022dcd80 *s) {
    s->unkBD4--;
    if (s->unkBD8 == 1) {
        if (s->unkBD4 & 6)
            func_ov000_022dbb18(1);
        else
            func_ov000_022dbb18(0);
    }
    if (s->unkBD4 > 0)
        return;
    if (s->unkBD8 == 1) {
        func_ov000_022dbb18(0);
        func_ov000_022dccc8(s);
        return;
    }
    if (s->unkBD0 == 1) {
        if (func_ov000_022db704((s->unk21C & 1) ? 0xff : data_ov000_02363550[gBossStatus.unk8 - 1], 8)) {
            func_ov000_022dcc10(s);
            return;
        }
        func_ov000_022dccc8(s);
        return;
    }
    s->unkBD8 = 1;
    s->unkBD4 = 0x3c;
}
