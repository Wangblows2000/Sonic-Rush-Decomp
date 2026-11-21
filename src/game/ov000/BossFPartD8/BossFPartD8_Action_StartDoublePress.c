/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartD8_InitHitboxes();
s32 ObjRect_SetBox3D();
s32 BossFPartD8_DisableHitboxes();
extern u8 gBossStatus[];
void BossFPartD8_State_DoublePressStart();

void BossFPartD8_Action_StartDoublePress(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x436) = 0;
    BossFPartD8_InitHitboxes(p0, ((u8 *)p0 + 0x400), 0);
    ObjRect_SetBox3D(((u8 *)p0 + 0x290), -120, -48, 0x10, 0x38, -0x7d00, 0x7d00);
    ObjRect_SetBox3D(((u8 *)p0 + 0x314), -20, -48, 0x74, 0x38, -0x7d00, 0x7d00);
    BossFPartD8_DisableHitboxes(p0);
    *(u32 *)((u8 *)gBossStatus + 0x4) = 0x31;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_DoublePressStart;
}
