#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u32 unk8;
    s32 unkc;
    void (*cb)(s32);
} UnkCtx_0203c470;

typedef struct {
    u8 pad[0x340];
    u8 unk340[0x184];
    u16 unk4c4;
} UnkWork_0203c470;

extern UnkCtx_0203c470 data_022b8be0;
extern UnkWork_0203c470 data_022b9020;
void WM_ReadStatus(void *p);
void WBT_SetOwnAid(u16 v);
void WBT_RequestSync(s32 a, void (*cb)(void));
void WFSi_OnChildSystemCallback(void);
void WFSi_SetMPData(void);

void WFS_Start(void) {
    data_022b8be0.unk4 = 1;
    if (data_022b8be0.unk0 == 0) {
        UnkWork_0203c470 *w = (UnkWork_0203c470 *)((u32)&data_022b9020 | 0);
        WM_ReadStatus(w->unk340);
        WBT_SetOwnAid(w->unk4c4);
        WBT_RequestSync(1, WFSi_OnChildSystemCallback);
    } else {
        data_022b8be0.unkc = 2;
        if (data_022b8be0.cb != NULL)
            data_022b8be0.cb(0);
    }
    WFSi_SetMPData();
}
