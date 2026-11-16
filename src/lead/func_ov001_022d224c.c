#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[0x20 - 4];
    u32 unk20;
    u16 unk24;
    u16 unk26;
    u8 pad28[0x3c - 0x28];
    u16 unk3c;
    u8 pad3e[0x40 - 0x3e];
} UnkBg_022d224c;

typedef struct {
    u8 pad0[0xc];
    s32 offs;
} UnkRes_022d224c;

typedef struct {
    u8 pad0[0x18];
    UnkRes_022d224c *res[15];
    UnkBg_022d224c bg[10];
    u8 pad2d4[0xbbc - 0x2d4];
    s32 scroll;
} UnkWork_022d224c;

typedef struct {
    u16 x;
    u16 y;
} UnkOfs_022d224c;

extern UnkWork_022d224c *data_ov001_0231f050;
extern u8 data_ov001_023160f8[];
extern u32 gScreenTransferHead;
extern u32 gScreenTransferTail;
extern UnkOfs_022d224c gDisplayMain[];
extern UnkOfs_022d224c gDisplaySub[];

void ScreenTransfer_QueueRect(void *p, s32 a, u16 b, u16 c, u32 d, u32 e, u16 f, u16 g, s32 h, s32 i, s32 j, s32 k);
void ScreenTransfer_Flush(void);

static inline void LoadBg_022d224c(void *ptr, u16 f3c, u32 flags, u32 f20, u16 f24, u16 f26, u16 q) {
    u32 s0;
    u32 s1;

    s0 = gScreenTransferHead;
    s1 = gScreenTransferTail;
    gScreenTransferHead = 0;
    gScreenTransferTail = 0;
    ScreenTransfer_QueueRect(ptr, 0, q, f3c, flags, f20, f24, f26, 0, 0, 0x20, 0x19);
    ScreenTransfer_Flush();
    gScreenTransferHead = s0;
    gScreenTransferTail = s1;
}

#pragma thumb on
void func_ov001_022d224c(void) {
    s32 x;
    UnkBg_022d224c *bg;
    UnkRes_022d224c *res;
    u32 v;
    s32 q;
    s32 k;
    s32 r;

    x = data_ov001_0231f050->scroll;
    q = x / 8;
    r = x % 8;
    for (k = 4; k < 7; k++) {
        res = data_ov001_0231f050->res[k];
        bg = &data_ov001_0231f050->bg[k];
        LoadBg_022d224c((u8 *)res + res->offs, bg->unk3c, bg->flags & 0x1000, bg->unk20, bg->unk24, bg->unk26, q);
        gDisplayMain[data_ov001_023160f8[k]].x = 0;
        gDisplayMain[data_ov001_023160f8[k]].y = r;
    }
    v = ((u16)r << 16) & 0x1ff0000;
    *(vu32 *)0x4000010 = v;
    *(vu32 *)0x4000014 = v;
    *(vu32 *)0x4000018 = v;
    for (k = 7; k < 10; k++) {
        res = data_ov001_0231f050->res[k];
        bg = &data_ov001_0231f050->bg[k];
        x = data_ov001_0231f050->scroll + 0x110;
        r = x % 8;
        LoadBg_022d224c((u8 *)res + res->offs, bg->unk3c, bg->flags & 0x1000, bg->unk20, bg->unk24, bg->unk26, x / 8);
        gDisplaySub[data_ov001_023160f8[k]].x = 0;
        gDisplaySub[data_ov001_023160f8[k]].y = r;
    }
    v = ((u16)r << 16) & 0x1ff0000;
    *(vu32 *)0x4001010 = v;
    *(vu32 *)0x4001014 = v;
    *(vu32 *)0x4001018 = v;
}
#pragma thumb reset
