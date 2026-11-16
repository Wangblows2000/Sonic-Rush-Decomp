#include <nitro/types.h>

typedef struct {
    s32 w0;
    u8 pad4[0xc];
    s32 w10;
    s32 w14;
    s32 w18;
    u8 b1c;
    u8 b1d;
    u16 h1e;
} UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
void StaffRoll_Create(s32 a);

#pragma thumb on
void StaffRoll_Start(void) {
    if (gGameState.w0 == 0) {
        StaffRoll_Create(0);
    } else {
        StaffRoll_Create(1);
    }
}
#pragma thumb reset
