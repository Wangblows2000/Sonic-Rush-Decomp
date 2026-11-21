#include <nitro/types.h>

typedef struct { u8 pad0[4]; u32 flags; } UnkStruct_02058350;
extern s16 gTalkDemoId;

u32 SaveData_GetProgressFlags(void);
void TalkDemo_SetFlags(s32 a, s32 b);
UnkStruct_02058350 *PlayerDemo_GetWork(void);

#pragma thumb on
void TalkDemo_InitSkippable(void) {
    switch (gTalkDemoId) {
    case 0xa:
        if (SaveData_GetProgressFlags() & 0x20000)
            TalkDemo_SetFlags(0, 0x100);
        break;
    case 0xb:
        if (SaveData_GetProgressFlags() & 0x40000)
            TalkDemo_SetFlags(0, 0x100);
        break;
    case 0xf:
        if (SaveData_GetProgressFlags() & 0x80000)
            TalkDemo_SetFlags(0, 0x100);
        break;
    case 0x10:
        if (SaveData_GetProgressFlags() & 0x100000)
            TalkDemo_SetFlags(0, 0x100);
        break;
    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x38:
    case 0x39:
        if (SaveData_GetProgressFlags() & 0x1000)
            TalkDemo_SetFlags(0, 0x100);
        break;
    case 0xe:
        if (SaveData_GetProgressFlags() & 0x1000) {
            TalkDemo_SetFlags(0, 0x100);
            PlayerDemo_GetWork()->flags |= 0x100000;
        }
        break;
    }
}
#pragma thumb reset
