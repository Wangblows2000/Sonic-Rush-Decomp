#include <nitro/types.h>

typedef struct {
    u8 pad[0x28];
    s32 unk28;
} UnkCtx_0203d8b4;

typedef struct {
    u32 a : 4;
    u32 b : 4;
    u32 c : 24;
    u32 unk4;
    u8 unk8;
} UnkMsg_0203d8b4;

extern UnkCtx_0203d8b4 data_022b8be0;
BOOL func_02015168(u16 bitmap, const void *src, s32 size, void (*cb)(void));
void func_0203d930(void);

void func_0203d8b4(u32 a, u16 bitmap, u32 d, u32 c) {
    UnkMsg_0203d8b4 m;
    m.a = a;
    m.c = c;
    m.unk4 = d;
    m.b = (u8)(data_022b8be0.unk28 >> 8);
    m.unk8 = data_022b8be0.unk28;
    func_02015168(bitmap, &m, 9, func_0203d930);
}
