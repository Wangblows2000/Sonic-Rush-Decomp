#include <nitro/types.h>

typedef struct { u32 flags; s32 v4; u8 pad[0xc]; void *task; } UnkStruct_0231f52c;
extern UnkStruct_0231f52c *gExtraBossWork;
void Vram_SetBankForSubBG(s32 a);
void Vram_SetBankForSubOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void GXS_SetGraphicsMode(s32 a);
void Task_Destroy(void *p);

void ExtraBoss_Unk_ov001_0230007c(void) {
    Vram_SetBankForSubBG(0x80);
    Vram_SetBankForSubOBJ(0x100, 0x100010, 0x40, 0, 0x100);
    *(vu16 *)0x0400100c = (*(vu16 *)0x0400100c & 0x43) | 0x404;
    GXS_SetGraphicsMode(0);
    gExtraBossWork->v4 = 0;
    if (gExtraBossWork->task != NULL) {
        Task_Destroy(gExtraBossWork->task);
        gExtraBossWork->task = NULL;
    }
    gExtraBossWork->flags &= ~1;
}
