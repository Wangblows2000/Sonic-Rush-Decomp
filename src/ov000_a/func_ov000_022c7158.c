#include <nitro/types.h>

typedef struct UnkStruct_022c7158_m {
    s32 m[9];
} UnkStruct_022c7158_m;

typedef struct UnkStruct_022c7158 {
    u8 pad[0x2cc];
    u16 unk2CC;
    u8 pad1[0x30c - 0x2ce];
    u8 unk30C[0x24];
    UnkStruct_022c7158_m unk330;
} UnkStruct_022c7158;

extern s16 data_0206e470[];
extern s16 data_0206f470[];

void MTX_RotY33_(void *m, s32 s, s32 c);
void func_02000e60(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);

void func_ov000_022c7158(UnkStruct_022c7158 *s) {
    UnkStruct_022c7158_m m;
    u8 *p = s->unk30C;
    MTX_RotY33_(p + 0x24, data_0206e470[((s->unk2CC & 0xffff) >> 4) * 2], data_0206e470[((s->unk2CC & 0xffff) >> 4) * 2 + 1]);
    func_02000e60(&m, data_0206f470[0], data_0206f470[1]);
    func_02000f34(p + 0x24, &m, p + 0x24);
}
