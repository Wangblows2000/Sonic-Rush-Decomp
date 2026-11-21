#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0xA6C - 0x210];
    u8 unkA6C;
    u8 padA6D[0xF50 - 0xA6D];
    s32 unkF50;
    u8 padF54[0xF66 - 0xF54];
    u16 unkF66;
    u16 unkF68;
    u16 unkF6A;
    u16 unkF6C;
    u8 padF6E[0x1118 - 0xF6E];
    s32 unk1118;
} UnkStruct_Boss;

void Boss6_State_TiltJump(void);

void Boss6_State_TiltWaitJump(UnkStruct_Boss *w) {
    if (!(w->unkA6C & 2))
        return;
    w->base.unk68 = Boss6_State_TiltJump;
    w->unkF50 = 0;
    w->unkF66 = 0;
    w->unkF68 = 0;
    w->unkF6A = 0;
    w->unkF6C = 0;
    w->unk1118 = w->base.unk10;
    w->base.unk10 = 0x1a000;
    w->unk20C &= ~0x100;
    if (Rand() & 1)
        w->unk20C |= 0x100;
}
