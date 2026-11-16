#include <nitro/types.h>

typedef struct {
    u8 pad0[0x144];
    s32 unk144;
} UnkStruct_0235a918;

void func_ov000_022c5e80(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);
void func_ov000_0235ac1c(s32 x, s32 y, s32 z, s32 w);

void func_ov000_0235a918(UnkStruct_0235a918 *w, s32 *pos) {
    s32 ox, oy;
    func_ov000_022c5e80(0x1152f0, 0x6cd988, pos[0] << 4, &ox, &oy, NULL);
    func_ov000_0235ac1c(ox, -pos[1] << 4, oy, w->unk144);
}
