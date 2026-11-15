#include <nitro/types.h>

typedef struct UnkStruct_022c5c80 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
} UnkStruct_022c5c80;

extern void *data_022c4488;
extern u8 data_ov000_02364f00[];
extern UnkStruct_022c5c80 data_0208922c;

void func_02021310(void *ctx, void *a, void *b);
void *func_02021264(void *ctx, u32 id);
void func_0203473c(void *buf, void *res, s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0203438c(void *buf);
void func_020212e0(void *ctx);

void func_ov000_022c5c80(u32 id) {
    u8 buf[0x40];
    u8 ctx[0x5c];
    void *r;
    u32 v;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 0x700;
    func_02021310(ctx, data_ov000_02364f00, data_022c4488);
    r = func_02021264(ctx, id);
    func_0203473c(buf, r, 0x38, 0, 2, 0x20, 0x18);
    func_0203438c(buf);
    data_0208922c.unk8 = 0;
    data_0208922c.unkA = 0;
    v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v | 4) << 8);
    func_020212e0(ctx);
}
