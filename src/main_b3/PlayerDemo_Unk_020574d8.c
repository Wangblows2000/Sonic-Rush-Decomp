#include <nitro/types.h>

typedef struct { u8 pad0[4]; u32 flags; } UnkPlayer_020574d8;
extern s16 gPlayerDemoId;

u32 func_02059b1c(void);
void PlayerDemo_Unk_02058348(void);

#pragma thumb on
void PlayerDemo_Unk_020574d8(UnkPlayer_020574d8 *p) {
    switch (gPlayerDemoId) {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        p->flags |= 0x100000;
        break;
    case 0x28:
    case 0x29:
    case 0x2a:
        if (func_02059b1c() & 0x1000)
            p->flags |= 0x100000;
        break;
    }
    PlayerDemo_Unk_02058348();
}
#pragma thumb reset
