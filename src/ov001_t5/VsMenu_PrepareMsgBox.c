#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkSprite_022da450;

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    u8 pad1c[0x210 - 0x1c];
    UnkSprite_022da450 spr[31];
} UnkStruct_022da450;

void TitleMenu_SetupMsgBoxDisplayForVsMenu(UnkStruct_022da450 *p);

#pragma thumb on
void VsMenu_PrepareMsgBox(UnkStruct_022da450 *p) {
    s32 i;

    TitleMenu_SetupMsgBoxDisplayForVsMenu(p);
    for (i = 0; i < 31; i++) {
        p->spr[i].flags &= ~0x60000;
        p->spr[i].flags |= 0x20000;
    }
    p->unk18 = 1;
}
#pragma thumb reset
