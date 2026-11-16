#include <nitro/types.h>

typedef struct UnkStruct_02314b28_m {
    s32 m[16];
} UnkStruct_02314b28_m;

typedef struct UnkStruct_02314b28_e {
    u8 pad0[0x48];
    u16 rot;
    u8 pad4A[2];
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02314b28_e;

extern UnkStruct_02314b28_e *data_ov001_0231f6a4;
extern s16 data_0206e470[];

void func_02004988(s32 s, s32 c, s32 x, s32 y, s32 z, s32 a, s32 b, void *out);
void func_02025268(s32 a, void *b, s32 c);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void func_ov001_02314b28(s32 idx) {
    UnkStruct_02314b28_e *e = &data_ov001_0231f6a4[idx];
    s32 a;
    UnkStruct_02314b28_m m;

    func_02004988(SIN(e->rot), COS(e->rot), e->x, e->y, e->z, 0x1000, 0, &m);
    a = 0;
    func_02025268(0x10, &a, 1);
    func_02025268(0x15, 0, 0);
    func_02025268(0x16, &m, 0x10);
}
