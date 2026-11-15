#include <nitro/types.h>

typedef struct UnkStruct_022cedd8 {
    u8 pad[0x1308];
    s32 unk1308;
    s32 unk130C;
    s32 unk1310;
    s32 unk1314;
} UnkStruct_022cedd8;

void func_ov000_022cedd8(UnkStruct_022cedd8 *s, s32 v) {
    if (s->unk130C < v) {
        s->unk1310 = v;
        s->unk1308 = 0xa3000;
    } else {
        s->unk1314 = v;
        s->unk1308 = -0xa3000;
    }
}
