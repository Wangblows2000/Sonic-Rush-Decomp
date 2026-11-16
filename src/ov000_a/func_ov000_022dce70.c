#include <nitro/types.h>

typedef struct UnkStruct_022dce70_m43 {
    s32 m[12];
} UnkStruct_022dce70_m43;

typedef struct UnkStruct_022dce70_a {
    u8 pad[0x24];
    UnkStruct_022dce70_m43 unk24;
    u8 pad1[0x138 - 0x54];
} UnkStruct_022dce70_a;

typedef struct UnkStruct_022dce70 {
    u8 pad[0x5d0];
    UnkStruct_022dce70_a unk5D0;
    u8 pad1[0xb28 - 0x708];
    UnkStruct_022dce70_m43 unkB28;
    u8 pad2[0xbbc - 0xb58];
    s32 unkBBC;
} UnkStruct_022dce70;

extern UnkStruct_022dce70 *data_ov000_02369b2c;
extern s16 data_0206e470[];

void func_02000e98(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);
void func_020316f4(void *a);
void func_020315e0(void *a);

void func_ov000_022dce70(void) {
    u8 mtx[0x24];
    UnkStruct_022dce70 *w = data_ov000_02369b2c;
    UnkStruct_022dce70_a *a = &w->unk5D0;
    u16 ang;
    a->unk24 = w->unkB28;
    ang = -w->unkBBC;
    func_02000e98(mtx, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
    func_02000f34(mtx, &a->unk24, &a->unk24);
    func_020316f4(a);
    func_020315e0(a);
}
