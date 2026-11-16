#include <nitro/types.h>

typedef struct {
    u8 pad0[0x38];
    s32 unk38;
    s32 unk3C;
} UnkStruct_0233e960;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[0x2C - 0xC];
    s32 unk2C;
    s32 unk30;
    u32 unk34;
} UnkPlayer_0233e960;

UnkPlayer_0233e960 *Camera_GetBySwapState(void);
UnkPlayer_0233e960 *Camera_GetOtherBySwapState(void);

void func_ov000_0233e960(UnkStruct_0233e960 *w) {
    UnkPlayer_0233e960 *q;

    Camera_GetBySwapState();
    q = Camera_GetOtherBySwapState();
    if (q->unk34 & 1) {
        return;
    }
    q->unk2C = (q->unk4 / 14) & 0xffff;
    q->unk30 = ((((q->unk8 / 14) >> 8) + 0x110 & 0xff) + 0x100) << 8;
    w->unk38 = (q->unk4 / 14) & 0xffff;
    w->unk3C = ((((q->unk8 / 14) >> 8) + 0x110 & 0xff) + 0x100) << 8;
}
