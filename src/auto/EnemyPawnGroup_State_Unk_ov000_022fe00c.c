/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjRect_SetBox2D();
s32 EnemyPawnGroup_Action_Walk();
s32 StageObj_HandleCollider();

void EnemyPawnGroup_State_Unk_ov000_022fe00c(u8 *p0) {
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) != 0) {
        *(u16 *)((u8 *)p0 + 0x238) = (*(s16 *)((u8 *)p0 + 0x238) - 0x1);
        if (*(s16 *)((u8 *)p0 + 0x238) == 0) {
            ObjRect_SetBox2D(((u8 *)p0 + 0x20c), -120, -48, -16, -16);
            EnemyPawnGroup_Action_Walk(p0);
            return;
        }
    }
    StageObj_HandleCollider(p0, ((u8 *)p0 + 0x20c));
}
