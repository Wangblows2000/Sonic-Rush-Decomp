#include <nitro/types.h>

typedef struct UnkStruct_0231316c_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_0231316c_v;

typedef struct UnkStruct_0231316c_m {
    s32 m[9];
} UnkStruct_0231316c_m;

typedef struct UnkStruct_0231316c_o {
    u8 pad0[0x24];
    UnkStruct_0231316c_m m;
    UnkStruct_0231316c_v pos;
} UnkStruct_0231316c_o;

typedef struct UnkStruct_0231316c {
    u8 pad0[0x20];
    UnkStruct_0231316c_v pos;
    u8 pad2C[0x74 - 0x2c];
    u32 unk74;
    u8 pad78[0xac - 0x78];
    UnkStruct_0231316c_o *unkAC;
    u8 padB0[0xf0 - 0xb0];
    UnkStruct_0231316c_m m;
    u8 pad114[0x1f0 - 0x114];
    s32 unk1F0;
    s32 unk1F4;
} UnkStruct_0231316c;

typedef struct UnkStruct_0231316c_g {
    u8 pad0[4];
    u8 unk4[4];
} UnkStruct_0231316c_g;

extern UnkStruct_0231316c_g *data_ov001_0231f694;

void func_02025268(s32 a, void *b, s32 c);
void Model_Draw(void *p);
void func_ov001_02311efc(UnkStruct_0231316c *p);

static inline void VecSet(UnkStruct_0231316c_v *d, s32 x, s32 y, s32 z) {
    d->x = x;
    d->y = y;
    d->z = z;
}

void func_ov001_0231316c(UnkStruct_0231316c *p) {
    s32 v;

    if (p->unk74 & 1)
        return;
    p->unkAC->m = p->m;
    VecSet(&p->unkAC->pos, p->pos.x, p->pos.y, p->pos.z);
    func_02025268(0x11, 0, 0);
    Model_Draw(data_ov001_0231f694->unk4);
    v = 1;
    func_02025268(0x12, &v, 1);
    if (p->unk1F0 == 0 && p->unk1F4 <= 0)
        return;
    func_ov001_02311efc(p);
}
