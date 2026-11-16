#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02326b2c_V;

typedef struct {
    UnkStruct_02326b2c_V a;
    UnkStruct_02326b2c_V b;
} UnkStruct_02326b2c_Out;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 pad4[0x10 - 4];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x58 - 0x24];
    u32 unk58;
} UnkStruct_02326b2c;

extern s16 data_0206e470[];

u16 FX_Atan2(s32 y, s32 x);
void MTX_Identity33_(void *m);
void MTX_RotX33_(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_RotZ33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *c);
void MTX_MultVec33(void *a, void *m, void *b);

void func_ov000_02326b2c(UnkStruct_02326b2c_Out *out, UnkStruct_02326b2c *w, s32 len) {
    u8 mtx[0x24];
    u8 rot[0x24];
    UnkStruct_02326b2c_V v2;
    UnkStruct_02326b2c_V v1;
    u16 ang = 0;
    s32 idx;

    v1.x = 0;
    v1.y = -len / 2;
    v1.z = 0;
    v2.x = 0;
    v2.y = len / 2;
    v2.z = 0;
    if (w->unk58 & 0x10)
        ang = FX_Atan2((w->unk14 - w->unk20) >> 8, (w->unk10 - w->unk1C) >> 8);
    MTX_Identity33_(mtx);
    idx = (s32)((u16)(w->unk2 << 8) & 0xffff) >> 4;
    MTX_RotX33_(rot, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
    MTX_Concat33(mtx, rot, mtx);
    idx = (s32)((u16)(w->unk3 << 8) & 0xffff) >> 4;
    MTX_RotY33_(rot, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
    MTX_Concat33(mtx, rot, mtx);
    idx = (s32)((u16)(ang + (w->unk1 << 8)) & 0xffff) >> 4;
    MTX_RotZ33_(rot, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
    MTX_Concat33(mtx, rot, mtx);
    MTX_MultVec33(&v1, mtx, &v1);
    out->b.x = w->unk10 + v1.x;
    out->b.y = w->unk14 + v1.y;
    out->b.z = w->unk18 + v1.z;
    MTX_MultVec33(&v2, mtx, &v2);
    out->a.x = w->unk10 + v2.x;
    out->a.y = w->unk14 + v2.y;
    out->a.z = w->unk18 + v2.z;
}
