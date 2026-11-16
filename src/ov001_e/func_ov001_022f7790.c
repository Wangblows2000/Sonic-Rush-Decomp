#include <nitro/types.h>

typedef struct { u8 pad[4]; s16 x; s16 y; u8 pad2[0x40]; } UnkStruct_022f7790_o;

typedef struct UnkStruct_022f7790 {
    u8 pad0[0x18];
    UnkStruct_022f7790_o objs[0x30];
    u8 pad[0xe5e - 0x18 - 0x30 * 0x48];
    s16 dig[3][6];
} UnkStruct_022f7790;

extern UnkStruct_022f7790 *data_ov001_0231f084;
extern s32 data_ov001_02316a80[];
void AnimatorSprite_Draw(void *p);

void func_ov001_022f7790(void) {
    s32 k;
    s32 i;
    s16 x;
    s16 y;
    UnkStruct_022f7790_o *o;

    for (k = 0, y = 0x54; k <= 1; k++, y += 0x10) {
        o = &data_ov001_0231f084->objs[data_ov001_02316a80[k]];
        o->x = 0x56;
        o->y = y;
        AnimatorSprite_Draw(o);
        for (i = 0, x = 0x60; i < 6; i++, x += 10) {
            s32 d = data_ov001_0231f084->dig[k][i];
            if (d != -1) {
                o = &data_ov001_0231f084->objs[d];
                o->x = x;
                o->y = y;
                AnimatorSprite_Draw(o);
            }
        }
    }
    o = &data_ov001_0231f084->objs[23];
    o->x = 0x96;
    o->y = 0x6c;
    AnimatorSprite_Draw(o);
}
