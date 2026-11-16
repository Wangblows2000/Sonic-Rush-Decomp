#include <nitro/types.h>

typedef struct {
    u8 pad0[0x3a];
    s8 unk3A;
    u8 pad3B[0x64 - 0x3b];
} UnkPlayer_0233c37c;

extern UnkPlayer_0233c37c gCamera[];
extern void *gPlayers[];

u8 Camera_GetPlayerScreenMask(void *obj) {
    u8 i;
    u8 r;
    s8 a, b;

    if (obj == NULL)
        return 0;
    a = gCamera[0].unk3A;
    b = gCamera[1].unk3A;
    for (i = 0; i < 8; i++) {
        if (gPlayers[i] == obj) {
            r = 0;
            if (a == i)
                r |= 1;
            if (b == i)
                r |= 2;
            if (r != 0)
                return r;
        }
    }
    return 0;
}
