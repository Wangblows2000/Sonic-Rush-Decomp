#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u32 flags;
    u16 unkc;
} UnkStruct_022c4680;

typedef struct {
    u8 pad0[0x24];
    u16 unk24;
} UnkStruct_022c46c0;

typedef struct {
    s32 unk0;
    s32 unk4;
} UnkWork_022cf3b0;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022cf3b0 *work;
} UnkTask_022cf3b0;

extern UnkStruct_022c4680 gSaveData;
extern UnkStruct_022c46c0 data_022c46c0;

void SysSound_Init(void);
void PostStaffRoll_InitDisplay(void);
UnkTask_022cf3b0 *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void PostStaffRoll_State_PlayerSelectUnlockNotice(void);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void SysSound_Release(void);
void PostStaffRoll_ShowSoundTestUnlockNotice(s32 a);

#pragma thumb on
void PostStaffRoll_ShowPlayerSelectUnlockNotice(s32 a) {
    UnkWork_022cf3b0 *w;

    if ((gSaveData.unkc >= 0x1a || data_022c46c0.unk24 >= 0x1c) && (gSaveData.flags & 0x100) == 0) {
        SysSound_Init();
        PostStaffRoll_InitDisplay();
        w = Task_Create(PostStaffRoll_State_PlayerSelectUnlockNotice, NULL, 0, 0x1000, 0, sizeof(UnkWork_022cf3b0))->work;
        w->unk0 = 0;
        w->unk4 = a;
        MsgBox_Create(0x14, 3, 5, 0, 0, 0, 0, 0x200010);
        return;
    }
    SysSound_Release();
    PostStaffRoll_ShowSoundTestUnlockNotice(a);
}
#pragma thumb reset
