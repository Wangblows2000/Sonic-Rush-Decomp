#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20c];
    u32 unk20C;
} UnkStruct_02308c20_o;

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u8 pad58[0x68 - 0x58];
    void *unk68;
} UnkStruct_02308c20;

s32 func_ov000_02307e84(UnkStruct_02308c20 *p);
UnkStruct_02308c20_o *func_ov000_0234237c(s32 a, s32 b);
void func_ov000_02308b88(UnkStruct_02308c20 *p);

void func_ov000_02308c20(UnkStruct_02308c20 *p) {
    UnkStruct_02308c20_o *o = func_ov000_0234237c(1, func_ov000_02307e84(p));

    if (o == NULL) {
        p->unk54 |= 4;
    } else {
        o->unk20C |= 0x20;
        p->unk68 = func_ov000_02308b88;
    }
}
