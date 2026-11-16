#include <nitro/types.h>

typedef struct {
    u8 pad0[0x4c];
} UnkSpr_022cfe58;

typedef struct {
    u8 pad0[0x64];
    s32 unk64;
    s32 unk68;
    u8 *unk6c;
    u8 *unk70;
    u16 unk74;
} UnkStruct_022cfe58;

void AnimatorSprite_Init(UnkSpr_022cfe58 *s, void *res, u32 idx, u32 c, u32 d, u32 e, void *f, u32 g, void *h);
void AnimatorSprite_ProcessAnimation(UnkSpr_022cfe58 *s, u32 a, u32 b);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void PaletteTransfer_QueueCopy(void *p, u16 a, s32 b, void *c);

#pragma thumb on
void func_ov001_022cfe58(UnkStruct_022cfe58 *p, void *res, u32 idx, s32 n, void *h, void *buf) {
    UnkSpr_022cfe58 spr;

    AnimatorSprite_Init(&spr, res, idx, 0x48, 1, 0, NULL, 0, h);
    AnimatorSprite_ProcessAnimation(&spr, 0, 0);
    {
        vu32 z = 0;
        MIi_CpuClear32(z, buf, n * 2);
    }
    PaletteTransfer_QueueCopy(buf, n, p->unk64, p->unk6c + n * (p->unk74 * 2));
    PaletteTransfer_QueueCopy(buf, n, p->unk68, p->unk70 + n * (p->unk74 * 2));
}
#pragma thumb reset
