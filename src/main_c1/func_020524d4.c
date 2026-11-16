#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
    u32 pad24;
    u32 unk28;
} UnkFile_020524d4;

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
    u8 pad14[0xc];
} UnkHdr_020524d4;

extern u32 gPaletteTransferTail;
extern u32 gPaletteTransferHead;

s32 func_0200b99c(void *f, void *buf, u32 size);
BOOL func_0200b930(void *f, u32 pos, u32 whence);
void PaletteTransfer_QueueLoad(void *p, u32 a, u32 b);
void PaletteTransfer_Flush(void);

void func_020524d4(UnkFile_020524d4 *f)
{
    UnkHdr_020524d4 h;
    u8 buf[0x208];
    u32 b;
    u32 a;

    func_0200b99c(f, &h, 0x20);
    func_0200b930(f, h.unk10, 0);
    func_0200b99c(f, buf, 0x204);
    b = gPaletteTransferHead;
    a = gPaletteTransferTail;
    gPaletteTransferTail = 0;
    gPaletteTransferHead = 0;
    PaletteTransfer_QueueLoad(buf, 0, 0x05000400);
    PaletteTransfer_Flush();
    gPaletteTransferHead = b;
    gPaletteTransferTail = a;
    f->unk28 = f->unk20;
}
