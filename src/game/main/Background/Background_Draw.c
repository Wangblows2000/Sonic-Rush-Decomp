#include <nitro/types.h>

typedef struct {
    u32 flags;
    u32 x;
    u32 y;
    u32 tblIdx;
    u8 slot;
    u8 pad11[3];
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    s32 unk20;
    u16 unk24;
    u16 unk26;
    u16 unk28;
    u16 unk2A;
    u16 unk2C;
    u16 unk2E;
    u32 unk30;
    u32 unk34;
    void *unk38;
    u16 unk3C;
} UnkStruct_0203438c;

extern u32 gPaletteTransferHead;
extern u32 gPaletteTransferTail;
extern u32 gVRAMTransferHead;
extern u32 gVRAMTransferTail;
extern u32 gScreenTransferHead;
extern u32 gScreenTransferTail;
typedef struct { u16 x; u16 y; } UnkPos_0203438c;
extern UnkPos_0203438c *gDisplayTable[];

void PaletteTransfer_QueueLoad(u32 a, u32 b, u32 c);
void PaletteTransfer_Flush(void);
void VRAMTransfer_QueueLoad(u32 a, u32 b, u32 c);
void VRAMTransfer_Flush(void);
void ScreenTransfer_QueueRect(void *src, u16 x, u16 y, u16 a, u32 flip, s32 mode, u16 c, u16 d, u16 e, u16 f, u16 w, u16 h);
void ScreenTransfer_Flush(void);

static inline void Imm_020356bc(u32 a, u32 b, u32 c)
{
    u32 head = gPaletteTransferHead;
    u32 tail = gPaletteTransferTail;
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueLoad(a, b, c);
    PaletteTransfer_Flush();
    gPaletteTransferHead = head;
    gPaletteTransferTail = tail;
}

static inline void Imm_020337a4(u32 a, u32 b, u32 c)
{
    u32 head = gVRAMTransferHead;
    u32 tail = gVRAMTransferTail;
    gVRAMTransferHead = 0;
    gVRAMTransferTail = 0;
    VRAMTransfer_QueueLoad(a, b, c);
    VRAMTransfer_Flush();
    gVRAMTransferHead = head;
    gVRAMTransferTail = tail;
}

static inline void Imm_02035d04(void *src, u16 x, u16 y, u16 a, u32 flip, s32 mode, u16 c, u16 d, u16 e, u16 f, u16 w, u16 h)
{
    u32 head = gScreenTransferHead;
    u32 tail = gScreenTransferTail;
    gScreenTransferHead = 0;
    gScreenTransferTail = 0;
    ScreenTransfer_QueueRect(src, x, y, a, flip, mode, c, d, e, f, w, h);
    ScreenTransfer_Flush();
    gScreenTransferHead = head;
    gScreenTransferTail = tail;
}

void Background_Draw(UnkStruct_0203438c *s)
{
    u32 flags;
    u16 w, h;

    if (s->unk30 != 0 && !(s->flags & 1)) {
        if (s->flags & 8)
            Imm_020356bc(s->unk30, s->unk14, s->unk18);
        else
            PaletteTransfer_QueueLoad(s->unk30, s->unk14, s->unk18);
    }
    if (s->unk34 != 0 && !(s->flags & 2)) {
        if (s->flags & 0x10)
            Imm_020337a4(s->unk34, 0, s->unk1C);
        else
            VRAMTransfer_QueueLoad(s->unk34, 0, s->unk1C);
    }
    if (s->unk38 == NULL)
        return;
    flags = s->flags;
    if (flags & 4)
        return;
    w = s->unk2C;
    h = s->unk2E;
    if (flags & 0x400) {
        w++;
        h++;
    }
    if (flags & 0x20)
        Imm_02035d04(s->unk38, s->x >> 3, s->y >> 3, s->unk3C, flags & 0x1000, s->unk20, s->unk24, s->unk26, s->unk28, s->unk2A, w, h);
    else
        ScreenTransfer_QueueRect(s->unk38, s->x >> 3, s->y >> 3, s->unk3C, flags & 0x1000, s->unk20, s->unk24, s->unk26, s->unk28, s->unk2A, w, h);
    if (s->flags & 0x800) {
        UnkPos_0203438c *tbl = gDisplayTable[s->tblIdx] + s->slot;
        tbl->x = (s->unk28 << 3) + (s->x & 7);
        tbl->y = (s->unk2A << 3) + (s->y & 7);
    }
}
