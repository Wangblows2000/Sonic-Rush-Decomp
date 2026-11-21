#include <nitro/types.h>

u8 *ExtraBoss_GetWork(void);
void ExtraBossPlayer_Action_Dash(void *p);
void ExtraBossPlayer_ChangeState(void *p, s32 a);

BOOL ExtraBossPlayer_CheckActionInput(u8 *p) {
    u32 *a = *(u32 **)(p + 0x1dc);
    u16 held;
    BOOL r = FALSE;
    held = *(u16 *)(ExtraBoss_GetWork() + 0x18);
    u16 trg = *(u16 *)(ExtraBoss_GetWork() + 0x1c);
    u16 *keys;
    if (a[0] & 4) {
        return r;
    }
    keys = (u16 *)a[2];
    if (trg & keys[0]) {
        ExtraBossPlayer_Action_Dash(p);
        r = TRUE;
    } else if (trg & keys[1]) {
        ExtraBossPlayer_ChangeState(p, 8);
        r = TRUE;
        *(u32 *)(p + 0x14) &= ~0x700;
    } else if (!(a[1] & 2) && (held & 0xf0)) {
        ExtraBossPlayer_ChangeState(p, 2);
        r = TRUE;
        *(u32 *)(p + 0x14) &= ~0x700;
    }
    return r;
}
