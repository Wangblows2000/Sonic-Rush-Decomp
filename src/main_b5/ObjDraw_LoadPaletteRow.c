#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x10];
    u8 *pal;
    u16 idx;
    u8 pad46[2];
} UnkBg_0206d190;

extern u16 gObjDrawPalette[];
extern u16 gObjDrawTintPalette[];

void AnimatorSprite_Init(UnkBg_0206d190 *bg, void *res, u16 col, s32 c, s32 d, s32 e, u32 f, s32 g, u32 h);
void AnimatorSprite_ProcessAnimation(UnkBg_0206d190 *bg, s32 a, s32 b);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
u16 ObjDraw_ShiftColor(u16 c, s32 a, s32 b, s32 d);

void ObjDraw_LoadPaletteRow(void *res, u16 col, u16 idx, s32 screen) {
    s32 i;
    UnkBg_0206d190 bg;

    if (screen == 0)
        AnimatorSprite_Init(&bg, res, col, 0x40, 0, 0, 0, 0, 0x05000200);
    else
        AnimatorSprite_Init(&bg, res, col, 0x40, 1, 0, 0, 0, 0x05000600);
    bg.flags |= 8;
    bg.idx = idx;
    AnimatorSprite_ProcessAnimation(&bg, 0, 0);
    MIi_CpuCopy16(bg.pal + bg.idx * 32, (u8 *)gObjDrawPalette + idx * 32, 0x20);
    for (i = bg.idx * 16; i < (bg.idx + 1) * 16; i++)
        gObjDrawTintPalette[i] = ObjDraw_ShiftColor(gObjDrawPalette[i], -10, -1, 0);
}
