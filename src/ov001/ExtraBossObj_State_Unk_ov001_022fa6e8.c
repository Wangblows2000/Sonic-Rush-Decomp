#include <nitro/types.h>

void ExtraBoss_SnapObjToCameraX(void *p);
void ExtraBoss_SetObjModelAnims(void *p, s32 a);
void ExtraBossObj_State_Unk_ov001_022fa664(void);

void ExtraBossObj_State_Unk_ov001_022fa6e8(u8 *p) {
    ExtraBoss_SnapObjToCameraX(p);
    *(s32 *)(p + 0x1cc) -= 1;
    if (*(s32 *)(p + 0x1cc) > 0) {
        return;
    }
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_Unk_ov001_022fa664;
    ExtraBoss_SetObjModelAnims(p, 2);
    (*(u8 **)(p + 0x84))[0x10c] &= ~4;
    (*(u8 **)(p + 0x84))[0x10d] &= ~4;
    (*(u8 **)(p + 0x84))[0x10e] &= ~4;
    *(s32 *)(p + 0x1cc) = 0xa;
}
