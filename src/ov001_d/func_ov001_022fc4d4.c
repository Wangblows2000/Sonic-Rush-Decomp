#include <nitro/types.h>

typedef struct UnkStruct_022fc4d4_g {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022fc4d4_g;

extern UnkStruct_022fc4d4_g data_022c4560;

u8 *func_ov001_022fd428(s32 a, void *b, s32 c, s32 d, s32 e);
void *func_ov001_02300c94(void);
u8 *func_ov001_022ffce8(void);
void func_ov001_022fe8d4(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void func_ov001_022fc494(void);
void func_02036754(void *p, u32 a, u32 b);

static inline BOOL IsMode23(void) {
    BOOL r = TRUE;
    if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
        r = FALSE;
    return r;
}

static inline s32 GetMode(void) {
    if (IsMode23())
        return 1;
    return data_022c4560.unk14;
}

void func_ov001_022fc4d4(void *p, s32 y, s32 z) {
    u8 *t;
    s32 x = *(s32 *)(func_ov001_022ffce8() + 0x31c);
    t = func_ov001_022fd428(6, p, x, y, z);
    if (t == NULL)
        return;
    *(void (**)(void))(t + 0x74) = func_ov001_022fc494;
    *(u32 *)(t + 0x14) |= 0x10000;
    *(void **)(t + 0x80) = func_ov001_02300c94();
    *(u16 *)(t + 0x98) = 0xffff;
    *(s32 *)(t + 0x1cc) = GetMode() == 1 ? 0x10 : 0x18;
    func_ov001_022fe8d4(t + 0xe8, t, 1, 1, 0x80000, 0x10000, 0x9000, 0, 0, 0, 0);
    *(u32 *)(t + 0xf0) |= 0x2200;
    *(u8 **)(t + 0xa0) = t + 0xe8;
    func_02036754(0, 0, 0xa8);
}
