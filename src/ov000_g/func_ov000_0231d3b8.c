#include <nitro/types.h>

typedef struct {
    u8 pad0[3];
    u8 unk3;
    u8 pad4[0x32C - 4];
    u32 unk32C;
    u8 pad330[0x3E8 - 0x330];
    void *unk3E8;
} UnkStruct_0231d3b8_P;

typedef struct {
    u8 pad0[0x24];
    s32 mtx[9];
} UnkStruct_0231d3b8_M;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x54 - 0x10];
    u32 unk54;
    u8 pad58[0x5C - 0x58];
    u16 unk5C;
    u8 pad5E[0x128 - 0x5E];
    UnkStruct_0231d3b8_M *unk128;
    u8 pad12C[0x1F4 - 0x12C];
    u32 unk1F4;
    u8 pad1F8[0x204 - 0x1F8];
    UnkStruct_0231d3b8_P *unk204;
} UnkStruct_0231d3b8;

extern s16 data_0206e470[];

s32 ObjSpd_DownSet(s32 a, s32 b);
void func_02000dfc(void *m);
void MTX_RotY33_(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);

void func_ov000_0231d3b8(UnkStruct_0231d3b8 *w) {
    u8 mtx[0x24];
    UnkStruct_0231d3b8_M *m = w->unk128;
    UnkStruct_0231d3b8_P *p = w->unk204;
    s32 old;
    s32 idx;

    if (p != NULL) {
        if (p->unk3E8 != w) {
            w->unk5C |= 0x200;
            w->unk54 &= ~2;
            w->unk204 = NULL;
            return;
        }
        if (p->unk32C & 2) {
            w->unk8 -= w->unkC;
            w->unk8 &= 0xffff;
            w->unkC = ObjSpd_DownSet(w->unkC, 6);
        } else {
            old = w->unk8;
            w->unk8 = (u16)((u8)(p->unk3 - 0x10) << 8);
            w->unkC = (old - w->unk8) & 0xffff;
        }
        func_02000dfc(m->mtx);
        idx = (s32)((u16)(-w->unk8) & 0xffff) >> 4;
        MTX_RotY33_(mtx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
        func_02000f34(m->mtx, mtx, m->mtx);
        if (p->unk3 < 0x80)
            w->unk1F4 |= 1;
        else
            w->unk1F4 &= ~1;
    } else {
        w->unk8 -= w->unkC;
        w->unk8 &= 0xffff;
        w->unkC = ObjSpd_DownSet(w->unkC, 6);
        func_02000dfc(m->mtx);
        idx = (s32)((u16)(-w->unk8) & 0xffff) >> 4;
        MTX_RotY33_(mtx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
        func_02000f34(m->mtx, mtx, m->mtx);
    }
}
