#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} UnkStruct_022c4560_cc4c;

extern UnkStruct_022c4560_cc4c gGameState;

#pragma thumb on
BOOL func_ov000_0235cc60(void) {
    switch (gGameState.unk4) { case 0: return FALSE; default: return TRUE; }
}
#pragma thumb reset
