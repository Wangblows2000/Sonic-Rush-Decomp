#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1cc4];
    s32 unk1cc4;
} UnkSub_022e606c;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[0x14 - 8];
    s32 state;
    u8 pad18[0x74 - 0x18];
    s32 unk74;
    u8 pad78[0x90 - 0x78];
    s32 unk90;
    s32 unk94;
    u8 pad98[0xa4 - 0x98];
    UnkSub_022e606c *sub;
} UnkWork_022e606c;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e606c *work;
} UnkTask_022e606c;

extern UnkTask_022e606c *data_022b4574;

void func_ov001_022e7114(void);
void func_ov001_022e3180(s32 a, s32 b);
void func_ov001_022e5f04(void);

#pragma thumb on
void func_ov001_022e606c(void) {
    UnkWork_022e606c *w;

    w = data_022b4574->work;
    switch (w->state) {
    case 1:
    case 2:
    case 3:
    case 6:
    case 8:
    case 12:
    case 13:
    case 14:
        w->unk74 = 0;
        data_022b4574->func = func_ov001_022e7114;
        return;
    case 15:
        func_ov001_022e3180(w->sub->unk1cc4, 0xd);
        return;
    case 16:
        func_ov001_022e3180(w->sub->unk1cc4, 0xf);
        return;
    case 7:
        if (w->unk4 == 0x13) {
            w->unk74 = 0;
            w->unk90 = 0;
            w->unk94 = 0x14;
            func_ov001_022e5f04();
            return;
        }
        break;
    case 9:
        if (w->unk4 == 0x18) {
            w->unk74 = 0;
            w->unk90 = 0;
            w->unk94 = 0x19;
            func_ov001_022e5f04();
        }
        break;
    }
}
#pragma thumb reset
