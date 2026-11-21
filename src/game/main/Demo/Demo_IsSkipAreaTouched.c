#include <nitro/types.h>

#pragma thumb on

typedef struct { u16 x; u16 y; u8 unk4[0xc]; u32 unk10; } UnkStruct_022b65b4;
extern UnkStruct_022b65b4 gTouchState;
BOOL Demo_IsSkipAreaTouched(void) { if (gTouchState.unk10 & 0x40) { u16 x = gTouchState.x; u16 y = gTouchState.y; if (x >= 0x40 && x <= 0xc0 && y >= 0x78 && y <= 0x90) return TRUE; } return FALSE; }

#pragma thumb reset
