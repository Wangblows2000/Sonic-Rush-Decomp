#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 unkc;
    u8 pad10[0x2c - 0x10];
    s32 unk2c;
} UnkSub_022cb708;

typedef struct {
    u8 pad0[0x30];
    UnkSub_022cb708 sub;
    u8 pad60[0x1e2c - 0x60];
    void *res;
} UnkWork_022cb708;

typedef struct {
    u8 pad0[0x30];
} UnkAnim_022cb708;

typedef struct {
    u8 pad0[0x208];
    u8 unk208[0x290 - 0x208];
    UnkAnim_022cb708 anim[4];
    u8 pad350[0x3ac - 0x350];
    UnkWork_022cb708 *work;
} UnkStruct_022cb708;

void KeyframeAnim_Init(void *p, void *res, s32 a);
void KeyframeAnim_PlayReverse(void *p);
void func_ov001_022cc044(UnkStruct_022cb708 *p);

#pragma thumb on
void func_ov001_022cb708(UnkStruct_022cb708 *p, s32 a, s32 b) {
    UnkWork_022cb708 *w = p->work;
    void *res = w->res;
    UnkSub_022cb708 *sub = &w->sub;
    UnkAnim_022cb708 *anim;

    KeyframeAnim_Init(&p->anim[a], res, sub->unkc);
    anim = &p->anim[b];
    KeyframeAnim_Init(anim, res, sub->unkc);
    KeyframeAnim_PlayReverse(anim);
    KeyframeAnim_Init(p->unk208, res, sub->unk2c);
    func_ov001_022cc044(p);
}
#pragma thumb reset
