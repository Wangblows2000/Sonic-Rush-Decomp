#include <nitro/types.h>

typedef struct UnkStruct_022dd068_a {
    u8 pad[0x24];
    u8 unk24[0x30];
    u8 pad1[0x138 - 0x54];
} UnkStruct_022dd068_a;

typedef struct UnkStruct_022dd068 {
    u8 pad[0x708];
    UnkStruct_022dd068_a unk708;
    u8 pad1[0xbb8 - 0x840];
    s32 unkBB8;
    s32 unkBBC;
} UnkStruct_022dd068;

extern UnkStruct_022dd068 *data_ov000_02369b2c;
extern s16 data_0206e470[];

void MTX_RotY33_(void *m, s32 s, s32 c);
void func_02000e98(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);
void func_020316f4(void *a);
void func_020315e0(void *a);

void func_ov000_022dd068(void) {
    u8 m1[0x24];
    u8 m2[0x24];
    UnkStruct_022dd068 *w = data_ov000_02369b2c;
    UnkStruct_022dd068_a *a = &w->unk708;
    u16 a1 = w->unkBB8;
    u16 a2 = w->unkBBC;
    MTX_RotY33_(m1, data_0206e470[((a1 & 0xffff) >> 4) * 2], data_0206e470[((a1 & 0xffff) >> 4) * 2 + 1]);
    func_02000e98(m2, data_0206e470[((a2 & 0xffff) >> 4) * 2], data_0206e470[((a2 & 0xffff) >> 4) * 2 + 1]);
    func_02000f34(m2, m1, a->unk24);
    func_020316f4(a);
    func_020315e0(a);
}
