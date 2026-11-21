#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; u32 unk4; u16 unk8; u16 unkA; } UnkCmd_02031e18;
typedef struct { u8 pad[0x10]; u32 unk10; } UnkRes_02031e18;
typedef struct {
    u8 pad0[0x10];
    UnkRes_02031e18 *res;
    u8 pad14[0x8];
    u32 unk1C;
    u8 pad20[0xC];
    u32 flags;
    u8 pad30[0xC];
    volatile u32 mode;
    u32 base;
    u16 unk44;
} UnkStruct_02031e18;

typedef void (*UnkFn_02031e18)(u32 src, u32 mode, u32 dst);
void PaletteTransfer_LoadNow(u32 a, u32 b, u32 c);
void PaletteTransfer_QueueLoad(u32 a, u32 b, u32 c);

BOOL BAC_FrameGroupFunc_Palette(UnkCmd_02031e18 *cmd, UnkStruct_02031e18 *s)
{
    if (!(s->flags & 0x10)) {
        UnkFn_02031e18 fn;
        u32 dst;
        if (s->flags & 0x40)
            fn = PaletteTransfer_LoadNow;
        else
            fn = PaletteTransfer_QueueLoad;
        switch (s->mode) {
        case 0:
            dst = s->base + ((cmd->unkA + s->unk44) << 5);
            break;
        case 2:
        case 4:
            dst = s->base + ((cmd->unkA + s->unk44) << 9);
            break;
        default:
            dst = s->base + (cmd->unkA << 5);
            break;
        }
        fn(cmd->unk4 + ((u32)s->res + s->res->unk10), s->mode, dst);
    }
    s->unk1C += cmd->unk2;
    return TRUE;
}
