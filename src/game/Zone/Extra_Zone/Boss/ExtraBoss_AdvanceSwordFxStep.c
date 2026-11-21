#include <nitro/types.h>

void ExtraBoss_SetObjModelAnims(void *p, s32 a);

void ExtraBoss_AdvanceSwordFxStep(u8 *p) {
    *(s32 *)(p + 0x1d0) += 1;
    switch (*(s32 *)(p + 0x1d0)) {
    case 1:
        ExtraBoss_SetObjModelAnims(p, 0);
        *(u32 *)(p + 0xf0) |= 0x200;
        break;
    case 2:
        ExtraBoss_SetObjModelAnims(p, 1);
        *(u32 *)(p + 0xf0) &= ~0x200;
        break;
    case 3:
        ExtraBoss_SetObjModelAnims(p, 0);
        *(u32 *)(p + 0xf0) |= 0x200;
        break;
    }
}
