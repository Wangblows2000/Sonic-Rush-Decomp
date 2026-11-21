#include <nitro/types.h>

typedef struct { u32 flags; s32 v4; u8 pad[0xc]; void *task; u8 pad2[0x320]; u16 h338; } UnkStruct_0231f52c;
extern UnkStruct_0231f52c *gExtraBossWork;
void Vram_SetBankForSubBG(s32 a);
void Vram_SetBankForSubOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void GX_ResetBankForSubOBJ(void);
void GX_ResetBankForSubBG(void);
void GXS_SetGraphicsMode(s32 a);
void *Task_Create(void (*f)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void ExtraBoss_UpdateDualScreen3D(void);

void ExtraBoss_SetupSubDisplay(BOOL on) {
    Vram_SetBankForSubBG(4);
    Vram_SetBankForSubOBJ(8, 0x100010, 0x20, 0, 0x100);
    if (on) {
        gExtraBossWork->h338 = 1;
        gExtraBossWork->v4 = 1;
        GX_ResetBankForSubOBJ();
    } else {
        gExtraBossWork->h338 = 0;
        gExtraBossWork->v4 = 2;
        GX_ResetBankForSubBG();
    }
    GXS_SetGraphicsMode(5);
    *(vu32 *)0x04001000 &= ~0x1f00;
    gExtraBossWork->task = Task_Create(ExtraBoss_UpdateDualScreen3D, 0, 0, 0xf000, 0, 0);
    gExtraBossWork->flags |= 1;
}
