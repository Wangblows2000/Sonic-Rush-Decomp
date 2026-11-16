#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    u8 unk14;
    u8 pad15;
    u8 unk16;
    u8 unk17;
    u8 pad18[0xc];
    u8 win[0xc8];  /* 0x24 */
    u16 pal[4];    /* 0xec */
} UnkWork_0205fb68;
extern UnkWork_0205fb68 *data_022c4d9c;

void func_020073b8(void *p, s32 a, s32 b, void (*cb)(void), s32 i);

#pragma thumb on
void func_0205fb68(void) {
    UnkWork_0205fb68 *w = data_022c4d9c;
    s16 v;

    if (w != NULL) {
        while (!(*(vu16 *)0x04000004 & 2))
            ;
        *(vu16 *)0x05000000 = w->pal[w->unk14];
        w->unk14 = (w->unk14 + 1) % 4;
        switch (w->unk14) {
        case 0:
            v = w->unk16 - 1;
            break;
        case 1:
            v = w->unk16 + 2;
            break;
        case 2:
            v = w->unk17 - 4;
            break;
        case 3:
            v = w->unk17 - 1;
            break;
        }
        func_020073b8(w->win, v, 5, func_0205fb68, w->unk14);
    }
}
#pragma thumb reset
