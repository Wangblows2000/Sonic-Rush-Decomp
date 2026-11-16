#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} VecFx32_022e552c;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
} UnkStruct_022e552c_obj;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x984 - 0x1c];
    u8 unk984[0xd5e - 0x984];
    u16 unkD5E;
    u8 padD60[0xf54 - 0xd60];
    s32 unkF54;
    s32 unkF58;
    s32 unkF5C;
    s16 unkF60;
    u8 padF62[2];
    s16 unkF64;
    u8 padF66[0xf70 - 0xf66];
    u8 *unkF70[10];
    s32 unkF98;
    u8 padF9C[0x1118 - 0xf9c];
    s32 unk1118;
} UnkStruct_022e552c;

extern UnkStruct_022e552c_obj *gPlayers;
extern s16 data_02070470[];
extern u8 data_ov000_023638c4[];

void Boss6_Unk_ov000_022e5c1c(UnkStruct_022e552c *p);
u8 *MapObject_Spawn(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *out);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void MTX_Inverse33(void *a, void *b);
void MTX_MultVec33(VecFx32_022e552c *a, void *m, VecFx32_022e552c *out);
void ObjRect_SetBox3D(void *b, s16 x1, s16 y1, s16 x2, s16 y2, s16 z1, s16 z2);
u8 *Boss6_Unk_ov000_022dfd80(s32 a, s32 x, s32 y, s32 z);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss6_SelectNextAttack(s32 a);

void Boss6_State_Unk_ov000_022e552c(UnkStruct_022e552c *p) {
    VecFx32_022e552c v1;
    VecFx32_022e552c v2;
    u8 mtx[0x28];
    u8 *e;
    s32 d;

    Boss6_Unk_ov000_022e5c1c(p);
    d = gPlayers->unk10 - p->unk10;
    if (p->unkF54 >= 0) {
        if (p->unk10 > 0x2aa00 || d < -0x8000) {
            p->unkF54 = -p->unkF54;
            p->unkF60 = -p->unkF60;
            p->unkF64 = -p->unkF64;
            p->unkF5C = -p->unkF5C;
        }
    } else {
        if (p->unk10 < 0x9600 || d > 0x8000) {
            p->unkF54 = -p->unkF54;
            p->unkF60 = -p->unkF60;
            p->unkF64 = -p->unkF64;
            p->unkF5C = -p->unkF5C;
        }
    }
    p->unk10 += p->unkF58;
    if (p->unk1118 % 15 == 0) {
        e = MapObject_Spawn(0xd4, 0, 0, 0, 0, 0, 0, 0, 0);
        p->unkF70[p->unkF98] = e;
        if (p->unkD5E != 0) {
            MTX_RotY33_(e + 0x230, data_02070470[0], data_02070470[1]);
            MTX_Concat33(e + 0x230, p->unk984, e + 0x230);
        } else {
            MIi_CpuCopy32(p->unk984, e + 0x230, 0x24);
        }
        ((UnkStruct_022e552c_obj *)e)->unk10 = p->unk10;
        ((UnkStruct_022e552c_obj *)e)->unk14 = p->unk14;
        ((UnkStruct_022e552c_obj *)e)->unk18 = p->unk18;
        MTX_Inverse33(e + 0x230, mtx);
        if (p->unkD5E != 0)
            MTX_Concat33(mtx, data_ov000_023638c4, mtx);
        v1.x = 0x33000;
        v1.y = -0x97000;
        v1.z = 0;
        MTX_MultVec33(&v1, mtx, &v1);
        v1.x >>= 12;
        v1.y >>= 12;
        v2.x = -0x33000;
        v2.y = -0x97000;
        v2.z = 0;
        MTX_MultVec33(&v2, mtx, &v2);
        v2.x >>= 12;
        v2.y >>= 12;
        ObjRect_SetBox3D(e + 0x134, v1.x - 13, v1.y - 13, v1.x + 13, v1.y + 13, -32000, 32000);
        ObjRect_SetBox3D(e + 0x160, v2.x - 13, v2.y - 13, v2.x + 13, v2.y + 13, -32000, 32000);
        e = Boss6_Unk_ov000_022dfd80(2, ((UnkStruct_022e552c_obj *)e)->unk10, ((UnkStruct_022e552c_obj *)e)->unk14, ((UnkStruct_022e552c_obj *)e)->unk18);
        if (p->unkD5E != 0) {
            MTX_RotY33_(e + 0x158, data_02070470[0], data_02070470[1]);
            MTX_Concat33(e + 0x158, p->unk984, e + 0x158);
        } else {
            MIi_CpuCopy32(p->unk984, e + 0x158, 0x24);
        }
        p->unkF98++;
        if (p->unkF98 >= 10)
            p->unkF98 = 0;
        Sound_PlaySeqArc(0, 0, 0x83);
    }
    p->unk1118++;
    if (p->unk1118 < 0x1c2)
        return;
    Boss6_SelectNextAttack(1);
}
