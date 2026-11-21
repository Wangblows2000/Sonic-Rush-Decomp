#include <nitro/types.h>

typedef struct { u8 pad0[4]; u32 flags; } UnkStruct_02058350;
extern s16 gTalkDemoId;

void SaveData_SetProgressFlags(u32 a);
u32 TalkDemo_GetFlags(s32 a);
UnkStruct_02058350 *PlayerDemo_GetWork(void);

#pragma thumb on
void TalkDemo_MarkSeen(void) {
    switch (gTalkDemoId) {
    case 0xa:
        SaveData_SetProgressFlags(0x20000);
        break;
    case 0xb:
        SaveData_SetProgressFlags(0x40000);
        break;
    case 0xf:
        SaveData_SetProgressFlags(0x80000);
        break;
    case 0x10:
        SaveData_SetProgressFlags(0x100000);
        break;
    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x38:
    case 0x39:
        if (TalkDemo_GetFlags(0) & 0x400)
            PlayerDemo_GetWork()->flags |= 0x200000;
        break;
    case 0xe: {
        UnkStruct_02058350 *p = PlayerDemo_GetWork();
        p->flags &= ~0x100000;
        p->flags &= ~0x800000;
        break;
    }
    }
}
#pragma thumb reset
