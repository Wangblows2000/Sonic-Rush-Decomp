#include <nitro/types.h>

#pragma thumb on

typedef struct { u16 x; u16 y; u8 unk4[4]; u16 unk8; u16 unkA; u8 unkC[4]; u32 unk10; } UnkStruct_022b65b4;
extern UnkStruct_022b65b4 gTouchState;
BOOL MsgBox_Unk_0204f3dc(void) { u16 x, y; if (!(gTouchState.unk10 & 0x40)) return FALSE; x = gTouchState.unk8; y = gTouchState.unkA; if (x >= 0x68 && x < 0x98 && y >= 0x74 && y < 0x8c) return TRUE; return FALSE; }

#pragma thumb reset
