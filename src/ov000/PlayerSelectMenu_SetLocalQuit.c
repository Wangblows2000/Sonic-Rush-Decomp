#include <nitro/types.h>

typedef struct UnkStruct_0235650c {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_0235650c;

u8 *func_020514e4(void);

#pragma thumb on
void PlayerSelectMenu_SetLocalQuit(UnkStruct_0235650c *s) {
    if (s->unkC == 1)
        func_020514e4()[2] = 1;
}
#pragma thumb reset
