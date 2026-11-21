#include <nitro/types.h>

typedef struct { s32 a; u8 pad[0x18]; u8 b; u8 c; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
void TimeAttackMenu_Create(s32 a);

#pragma thumb on
void TimeAttackMenu_StartMode3(void) {
    gGameState.a = 0;
    gGameState.b = 0;
    gGameState.c = 0;
    TimeAttackMenu_Create(3);
}
#pragma thumb reset
