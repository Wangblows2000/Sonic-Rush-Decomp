#include <nitro/types.h>

typedef struct UnkStruct_02312d7c_b {
    u8 pad0[0xa];
    u8 unkA;
    u8 padB[0x90 - 0xb];
    u8 anm[4];
} UnkStruct_02312d7c_b;

typedef struct UnkStruct_02312d7c {
    u8 pad0[4];
    UnkStruct_02312d7c_b b;
} UnkStruct_02312d7c;

extern UnkStruct_02312d7c *data_ov001_0231f694;

void func_02021c38(void *p, void (*cb)(void), s32 a, s32 b, s32 c);
void func_ov001_02311e64(void);

void func_ov001_02312d7c(void) {
    UnkStruct_02312d7c_b *b = &data_ov001_0231f694->b;

    func_02021c38(b->anm, func_ov001_02311e64, 0, 6, 3);
    b->unkA = 4;
}
