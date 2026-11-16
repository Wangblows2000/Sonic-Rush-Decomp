#include <nitro/types.h>

typedef struct {
    u8 pad0[0x3C];
    s32 unk3C;
    s32 unk40;
    u8 pad44[0x51 - 0x44];
    u8 unk51;
} UnkStruct_0233f6f8;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[0x2C - 0xC];
    s32 unk2C;
    s32 unk30;
    u32 unk34;
} UnkPlayer_0233f6f8;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_0233f6f8_4560;

extern UnkStruct_0233f6f8_4560 gGameState;

UnkPlayer_0233f6f8 *Camera_GetBySwapState(void);
UnkPlayer_0233f6f8 *Camera_GetOtherBySwapState(void);
u8 func_ov000_023427bc(s32 idx);

static inline BOOL IsMode_0233f6f8(void) {
    BOOL r = FALSE;
    if (gGameState.unk1C >= 7 && gGameState.unk1C < 9 && gGameState.unk1D < 2) {
        r = TRUE;
    }
    return r;
}

void func_ov000_0233f6f8(UnkStruct_0233f6f8 *w) {
    UnkPlayer_0233f6f8 *p = Camera_GetBySwapState();
    UnkPlayer_0233f6f8 *q = Camera_GetOtherBySwapState();

    if (!(q->unk34 & 1)) {
        if (IsMode_0233f6f8()) {
            q->unk2C = 0;
        } else {
            q->unk2C = (q->unk4 / 12) & 0x1ffff;
        }
        q->unk30 = ((((p->unk8 / 12) >> 8) + 0x110) % 0x118 + 0xe8) << 8;
        if (IsMode_0233f6f8()) {
            w->unk3C = 0;
        } else {
            w->unk3C = (q->unk4 / 12) & 0x1ffff;
        }
        w->unk40 = ((((p->unk8 / 12) >> 8) + 0x110) % 0x118 + 0xe8) << 8;
    }
    w->unk51 = func_ov000_023427bc(0);
}
