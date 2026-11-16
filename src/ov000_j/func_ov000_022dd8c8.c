#include <nitro/types.h>

typedef struct {
    u8 pad0[0x224];
    s32 unk224;
    u8 pad228[0xbb8 - 0x228];
    s32 unkBB8;
    u8 padBBC[4];
    s32 unkBC0;
    u8 padBC4[4];
    s32 unkBC8;
} UnkStruct_022dd8c8;

void func_ov000_022db954(UnkStruct_022dd8c8 *p, s32 a);
void func_ov000_022de304(UnkStruct_022dd8c8 *p);

void func_ov000_022dd8c8(UnkStruct_022dd8c8 *p) {
    p->unkBB8 += p->unkBC0;
    func_ov000_022db954(p, p->unkBC8);
    if (p->unk224 <= 0xb4)
        return;
    func_ov000_022de304(p);
}
