#include <nitro/types.h>

typedef struct UnkStruct_02311efc_v {
    s32 x, y, z;
} UnkStruct_02311efc_v;

typedef struct UnkStruct_02311efc_m {
    s32 m[9];
} UnkStruct_02311efc_m;

typedef struct UnkStruct_02311efc_r {
    u8 pad0[0x24];
    UnkStruct_02311efc_m rot;
    UnkStruct_02311efc_v pos;
} UnkStruct_02311efc_r;

typedef struct UnkStruct_02311efc {
    u8 pad0[0x6c];
    u32 flags;
    u8 pad70[0xac - 0x70];
    UnkStruct_02311efc_r *unkAC;
    u8 padB0[0x160 - 0xb0];
    UnkStruct_02311efc_v pos[3];
    UnkStruct_02311efc_m rot[3];
    u8 pad1F0[0x228 - 0x1f0];
    s32 unk228;
} UnkStruct_02311efc;

void func_02022270(u32 index, u32 value);
void func_02025268(u32 op, const u32 *args, u32 n);
void func_020315e0(void *o);

static inline void VecSet(UnkStruct_02311efc_v *d, s32 x, s32 y, s32 z) {
    d->x = x;
    d->y = y;
    d->z = z;
}

void func_ov001_02311efc(UnkStruct_02311efc *p) {
    u32 one;
    u32 one2;
    s32 i;
    s32 k;

    func_02022270(0, 0x6182);
    func_02022270(1, 0x6182);
    func_02022270(2, 0x6182);
    if (p->unk228 == 0x12 || p->unk228 == 0x13) {
        if (p->flags & 1) {
            for (i = 0; i < 3; i++) {
                p->unkAC->rot = p->rot[i];
                VecSet(&p->unkAC->pos, p->pos[i].x, p->pos[i].y, p->pos[i].z);
                func_02025268(0x11, 0, 0);
                func_020315e0(p->unkAC);
                one = 1;
                func_02025268(0x12, &one, 1);
            }
        }
    } else {
        if (p->flags & 1)
            k = 0;
        else if (p->flags & 2)
            k = 1;
        else
            k = 2;
        p->unkAC->rot = p->rot[k];
        VecSet(&p->unkAC->pos, p->pos[k].x, p->pos[k].y, p->pos[k].z);
        func_02025268(0x11, 0, 0);
        func_020315e0(p->unkAC);
        one2 = 1;
        func_02025268(0x12, &one2, 1);
    }
    func_02022270(0, 0x7fff);
    func_02022270(1, 0x4631);
    func_02022270(2, 0x2108);
}
