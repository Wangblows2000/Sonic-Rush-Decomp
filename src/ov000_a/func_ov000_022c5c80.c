#include <nitro/types.h>

typedef struct UnkStruct_022c5c80 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
} UnkStruct_022c5c80;

extern void *gZoneActArchive;
extern u8 data_ov000_02364f00[];
extern UnkStruct_022c5c80 gDisplayMain;

void func_02021310(void *ctx, void *a, void *b);
void *func_02021264(void *ctx, u32 id);
void Background_Init(void *buf, void *res, s32 a, s32 b, s32 c, s32 d, s32 e);
void Background_Draw(void *buf);
void func_020212e0(void *ctx);

void func_ov000_022c5c80(u32 id) {
    u8 buf[0x40];
    u8 ctx[0x5c];
    void *r;
    u32 v;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 0x700;
    func_02021310(ctx, data_ov000_02364f00, gZoneActArchive);
    r = func_02021264(ctx, id);
    Background_Init(buf, r, 0x38, 0, 2, 0x20, 0x18);
    Background_Draw(buf);
    gDisplayMain.unk8 = 0;
    gDisplayMain.unkA = 0;
    v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v | 4) << 8);
    func_020212e0(ctx);
}
