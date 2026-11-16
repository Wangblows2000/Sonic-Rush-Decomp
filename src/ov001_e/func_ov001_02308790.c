#include <nitro/types.h>

typedef struct UnkStruct_02308790_o {
    u8 pad0[0x48];
} UnkStruct_02308790_o;

typedef struct UnkStruct_02308790 {
    u8 pad0[0x198];
    UnkStruct_02308790_o a;
    UnkStruct_02308790_o b;
    UnkStruct_02308790_o c;
} UnkStruct_02308790;

typedef struct UnkStruct_02308790_t {
    u8 pad0[0x148];
    s32 unk148;
} UnkStruct_02308790_t;

extern UnkStruct_02308790 *data_ov001_0231f5bc;

UnkStruct_02308790_t *func_ov001_0230c0a4(void);
void AnimatorSprite_SetAnimation(UnkStruct_02308790_o *s, u16 index);

void func_ov001_02308790(void) {
    UnkStruct_02308790 *g = data_ov001_0231f5bc;
    s32 n = func_ov001_0230c0a4()->unk148;

    AnimatorSprite_SetAnimation(&g->a, n / 100 % 10 + 0x1e);
    AnimatorSprite_SetAnimation(&g->b, n / 10 % 10 + 0x1e);
    AnimatorSprite_SetAnimation(&g->c, n % 10 + 0x1e);
}
