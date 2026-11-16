#include <nitro/types.h>

typedef struct {
    u16 flags;
    s16 bright;
} UnkWork_02043abc;

typedef struct {
    u8 pad0[0x48];
    s16 bright;
} UnkDisp_02043abc;

extern UnkDisp_02043abc gDisplayMain;
extern UnkDisp_02043abc gDisplaySub;

void GXx_SetMasterBrightness_(vu16 *reg, s32 b);

void BrightnessFade_Apply(UnkWork_02043abc *w) {
    s16 b;

    if (w->bright > 0x1000)
        b = 0x10;
    else
        b = w->bright >> 8;
    if (!(w->flags & 1))
        b = -b;
    if (w->flags & 0x300) {
        if (w->flags & 0x100)
            gDisplayMain.bright = b;
        if (w->flags & 0x200)
            gDisplaySub.bright = b;
    } else {
        gDisplayMain.bright = b;
        gDisplaySub.bright = b;
    }
    if (!(w->flags & 0x40))
        return;
    w->flags &= ~0x40;
    GXx_SetMasterBrightness_((vu16 *)0x0400006c, gDisplayMain.bright);
    GXx_SetMasterBrightness_((vu16 *)0x0400106c, gDisplaySub.bright);
}
