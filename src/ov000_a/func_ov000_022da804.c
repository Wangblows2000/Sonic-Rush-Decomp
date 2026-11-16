#include <nitro/types.h>

typedef struct UnkStruct_022da804 {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022da804;

void func_ov000_022c5e80(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);

BOOL func_ov000_022da804(UnkStruct_022da804 *a, UnkStruct_022da804 *b) {
    u16 a1;
    u16 a2;
    func_ov000_022c5e80(0xf42f8, 0x5fe435, a->unk10 << 4, NULL, NULL, &a1);
    func_ov000_022c5e80(0xf42f8, 0x5fe435, b->unk10 << 4, NULL, NULL, &a2);
    return (u16)(a2 - a1) >= 0x8000;
}
