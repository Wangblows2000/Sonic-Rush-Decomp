#include <nitro/types.h>

typedef struct UnkStruct_02356408 {
    u8 pad[0xc];
    u16 unkC;
    u8 pad1;
    u8 unkF;
} UnkStruct_02356408;

typedef struct UnkStruct_02356408_2 {
    u8 pad[0x24];
    u16 unk24;
} UnkStruct_02356408_2;

extern UnkStruct_02356408 gSaveData;
extern UnkStruct_02356408_2 data_022c46c0;

#pragma thumb on
BOOL PlayerSelectMenu_IsFullyUnlocked(void) {
    if (gSaveData.unkC >= 0x1a && data_022c46c0.unk24 >= 0x1c && (gSaveData.unkF & 0x7f) == 0x7f)
        return TRUE;
    return FALSE;
}
#pragma thumb reset
