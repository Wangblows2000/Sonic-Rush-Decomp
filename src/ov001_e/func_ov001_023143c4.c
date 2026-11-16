#include <nitro/types.h>

typedef struct UnkStruct_023143c4_s {
    u8 pad0[0xc];
    s32 count;
} UnkStruct_023143c4_s;

typedef struct UnkStruct_023143c4 {
    u8 pad0[4];
    s32 frame;
    s32 steps;
    u8 padC[4];
    s32 unk10;
    UnkStruct_023143c4_s *script;
    s32 cur;
} UnkStruct_023143c4;

extern UnkStruct_023143c4 *data_ov001_0231f69c;

BOOL func_ov001_02314088(UnkStruct_023143c4 *p);

void func_ov001_023143c4(void) {
    UnkStruct_023143c4 *p = data_ov001_0231f69c;

    if (p->cur >= p->script->count)
        return;
    while (p->steps > 0) {
        if (!func_ov001_02314088(p)) {
            p->steps = 0;
            break;
        }
        p->steps--;
    }
    p->frame++;
}
