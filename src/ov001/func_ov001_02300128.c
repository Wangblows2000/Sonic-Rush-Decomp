#include <nitro/types.h>

typedef struct { u32 flags; s32 v4; u8 pad[0xc]; void *task; u8 pad2[0x320]; u16 h338; } UnkStruct_0231f52c;
extern UnkStruct_0231f52c *gExtraBossWork;
void Vram_SetBankForSubBG(s32 a);
void Vram_SetBankForSubOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_020030a8(void);
void func_020030bc(void);
void GXS_SetGraphicsMode(s32 a);
void *Task_Create(void (*f)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void func_ov001_022feac8(void);

void func_ov001_02300128(BOOL on) {
    Vram_SetBankForSubBG(4);
    Vram_SetBankForSubOBJ(8, 0x100010, 0x20, 0, 0x100);
    if (on) {
        gExtraBossWork->h338 = 1;
        gExtraBossWork->v4 = 1;
        func_020030a8();
    } else {
        gExtraBossWork->h338 = 0;
        gExtraBossWork->v4 = 2;
        func_020030bc();
    }
    GXS_SetGraphicsMode(5);
    *(vu32 *)0x04001000 &= ~0x1f00;
    gExtraBossWork->task = Task_Create(func_ov001_022feac8, 0, 0, 0xf000, 0, 0);
    gExtraBossWork->flags |= 1;
}
