#include <nitro/types.h>

typedef struct {
    u8 pad0[0x34];
    s32 vram;
    u8 pad38[0x10];
} UnkSpr_02055878;

typedef struct {
    u8 pad0[0x10];
    u8 res10[8];
    u8 res18[8];
    UnkSpr_02055878 spr[4];
} UnkWork_02055878;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02055878 *work;
} UnkTask_02055878;

extern u32 gSystemFlags;

void ObjData_Release(void *p);
void ObjVram_Free(s32 a, s32 b);
void System_Unfreeze(void);
void NNS_SndPlayerPauseByPlayerNo(s32 idx, s32 frames);

void PauseMenu_Destructor(UnkTask_02055878 *t) {
    UnkWork_02055878 *w = t->work;
    u16 i;
    s32 k;

    ObjData_Release(w->res10);
    ObjData_Release(w->res18);
    ObjData_Release(w->res18);
    ObjData_Release(w->res18);
    for (i = 0; i < 4; i++)
        ObjVram_Free(0, w->spr[i].vram);
    if (gSystemFlags & 8)
        System_Unfreeze();
    for (k = 3; k < 10; k++)
        NNS_SndPlayerPauseByPlayerNo(k, 0);
}
