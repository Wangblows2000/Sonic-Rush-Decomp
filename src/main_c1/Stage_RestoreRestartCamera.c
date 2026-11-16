#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x58];
    u16 unk58;
    u16 unk5a;
    u32 unk5c;
} UnkStruct_022c4590;

typedef struct {
    u32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padc[0xbc];
    u32 unkc8;
} UnkStruct_0236aad8;

typedef struct {
    u32 unk0;
    s32 unk4;
    s32 unk8;
} UnkStruct_0236ab3c;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4590 data_022c4590;
extern UnkStruct_0236aad8 gCamera;
extern UnkStruct_0236ab3c data_ov000_0236ab3c;

void Stage_RestoreRestartCamera(void)
{
    s32 x;
    s32 y;

    if ((gGameState.unk18 & 0x40) == 0)
        return;
    y = data_022c4590.unk5a << 8;
    gCamera.unkc8 |= data_022c4590.unk5c;
    x = data_022c4590.unk58 << 8;
    gCamera.unk4 = x;
    gCamera.unk8 = y;
    UnkStruct_0236ab3c *b = (UnkStruct_0236ab3c *)((u32)&data_ov000_0236ab3c | 0);
    b->unk4 = x;
    if ((gCamera.unkc8 & 2) == 0)
        b->unk8 = gCamera.unk8 + 0x11000;
    else
        b->unk8 = gCamera.unk8 - 0x11000;
}
