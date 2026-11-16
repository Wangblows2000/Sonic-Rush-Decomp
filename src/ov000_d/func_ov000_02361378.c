#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    void (*unk8)(void);
} UnkStruct_02361378_Task;

extern UnkStruct_02361378_Task *gTaskCurrent;

s32 StageLoader_LoadZoneFiles(void);
void func_02054dbc(void);
u32 WH_GetBitmap(void);
void func_020516b4(void);
void func_ov000_0236133c(void);

#pragma thumb on

void func_ov000_02361378(void) {
    s32 s = StageLoader_LoadZoneFiles();

    if (s == 2) {
        func_02054dbc();
        return;
    }
    if (WH_GetBitmap() & 0) {
        func_02054dbc();
        return;
    }
    if (s == 1) {
        func_020516b4();
        gTaskCurrent->unk8 = func_ov000_0236133c;
    }
}

#pragma thumb reset
