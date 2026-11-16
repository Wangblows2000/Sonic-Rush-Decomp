#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2C];
} UnkStruct_023623e0_Obj;

typedef struct {
    u8 pad0[0x850];
    UnkStruct_023623e0_Obj obj[19];
    u8 padB94[0x1028 - 0xB94];
    void *unk1028;
} UnkStruct_023623e0;

void KeyframeAnim_Init(UnkStruct_023623e0_Obj *o, void *res, s32 a);

#pragma thumb on

void func_ov000_023623e0(UnkStruct_023623e0 *w) {
    void *res = w->unk1028;

    KeyframeAnim_Init(&w->obj[0], res, 0);
    KeyframeAnim_Init(&w->obj[1], res, 1);
    KeyframeAnim_Init(&w->obj[2], res, 2);
    KeyframeAnim_Init(&w->obj[3], res, 3);
    KeyframeAnim_Init(&w->obj[4], res, 4);
    KeyframeAnim_Init(&w->obj[5], res, 5);
    KeyframeAnim_Init(&w->obj[6], res, 7);
    KeyframeAnim_Init(&w->obj[7], res, 8);
    KeyframeAnim_Init(&w->obj[8], res, 9);
    KeyframeAnim_Init(&w->obj[9], res, 10);
    KeyframeAnim_Init(&w->obj[10], res, 11);
    KeyframeAnim_Init(&w->obj[11], res, 12);
    KeyframeAnim_Init(&w->obj[12], res, 13);
    KeyframeAnim_Init(&w->obj[13], res, 14);
    KeyframeAnim_Init(&w->obj[14], res, 15);
    KeyframeAnim_Init(&w->obj[15], res, 16);
    KeyframeAnim_Init(&w->obj[16], res, 17);
    KeyframeAnim_Init(&w->obj[17], res, 18);
}

#pragma thumb reset
