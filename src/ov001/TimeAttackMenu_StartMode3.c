#include <nitro/types.h>

typedef struct { s32 a; u8 pad[0x18]; u8 b; u8 c; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
void TimeAttackMenu_Create(s32 a);

#pragma thumb on
void TimeAttackMenu_StartMode3(void) {
    data_022c4560.a = 0;
    data_022c4560.b = 0;
    data_022c4560.c = 0;
    TimeAttackMenu_Create(3);
}
#pragma thumb reset
