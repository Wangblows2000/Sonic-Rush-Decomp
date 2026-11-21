#include <nitro/types.h>

void ExtraBoss_SnapObjToCameraX(void *p);
void ExtraBoss_SetObjModelAnims(void *p, s32 a);
u32 *ExtraBoss_GetWork(void);
void ExtraBoss_StartRingSuction(s32 a);
void ExtraBoss_SpawnSuctionFx(s32 a, s32 b, s32 c);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void ExtraBossObj_State_SpacebActive(void);

void ExtraBossObj_State_SpacebStart(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    ExtraBoss_SnapObjToCameraX(p);
    if (!(q[0x10c] & 2)) {
        return;
    }
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_SpacebActive;
    ExtraBoss_SetObjModelAnims(p, 1);
    (*(u8 **)(p + 0x84))[0x10c] |= 4;
    (*(u8 **)(p + 0x84))[0x10d] |= 4;
    (*(u8 **)(p + 0x84))[0x10e] |= 4;
    *(s32 *)(p + 0x1cc) = 0x12c;
    *ExtraBoss_GetWork() |= 0x40;
    ExtraBoss_GetWork()[0x310 / 4] = *(s32 *)(p + 0x1c);
    ExtraBoss_StartRingSuction(*(s32 *)(p + 0x1c));
    ExtraBoss_SpawnSuctionFx(0, *(s32 *)(p + 0x1c), *(s32 *)(p + 0x20));
    Sound_PlaySeqArc(0, 0, 0xb9);
}
