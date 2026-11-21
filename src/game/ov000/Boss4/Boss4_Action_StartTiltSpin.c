/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBossStatus[];
void Boss4_SubState_TiltSpin();
s32 Boss4_SetSpinSpeed();
s32 Boss4_LoadModelPalettes1();
s32 Boss4_LoadModelPalettes2();
s32 Boss4_LoadModelPalettes4();
s32 Boss4_LoadModelPalettes3();

void Boss4_Action_StartTiltSpin(u8 *p0) {
    *(u32 *)((u8 *)gBossStatus + 0x4) = 0x19;
    *(u32 *)((u8 *)p0 + 0x220) = (u32)Boss4_SubState_TiltSpin;
    *(u32 *)((u8 *)p0 + 0x224) = 0;
    Boss4_SetSpinSpeed(p0, 0x60, 0, 0x19);
    *(u32 *)((u8 *)p0 + 0xbc4) = 0x20;
    *(u32 *)((u8 *)p0 + 0xbc8) = -0xc00;
    *(u32 *)((u8 *)p0 + 0xbcc) = -0xc00;
    if ((*(u32 *)((u8 *)p0 + 0x21c) & 2) == 0) {
        return;
    }
    Boss4_LoadModelPalettes1(0, 0x20);
    Boss4_LoadModelPalettes2(0);
    Boss4_LoadModelPalettes4(0);
    Boss4_LoadModelPalettes3(0);
}
