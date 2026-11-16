#include <nitro/types.h>

typedef struct UnkStruct_02301118_r {
    s32 unk0;
    u8 pad4[0x30];
    u32 unk34;
} UnkStruct_02301118_r;

typedef struct UnkStruct_02301118 {
    void *unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
    u8 padC[8];
    u32 unk14;
    u8 pad18[0x84 - 0x18];
    void *unk84;
    void *unk88;
    void *unk8C;
    UnkStruct_02301118_r *unk90;
    void *unk94;
    u8 pad98[0x1dc - 0x98];
    void *unk1DC;
} UnkStruct_02301118;

extern u8 *data_ov001_0231f530;
extern void *data_0208fe80;

void func_ov001_02302ac8(UnkStruct_02301118 *w);
void func_ov001_022fd4f4(UnkStruct_02301118 *w);
u8 *func_ov001_022ffce8(void);
void func_02039bb0(s32 i, u32 addr);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
void func_02031768(void *p);
void func_020371c8(void *p);

#define ARR(off, i) (((u32 *)(data_ov001_0231f530 + (off)))[i])

void func_ov001_02301118(UnkStruct_02301118 *w) {
    vu32 fill1;
    vu32 fill2;
    vu32 fill3;
    vu32 fill4;
    vu32 fill5;
    vu32 fill6;
    void *task = w->unk0;
    u8 *p;
    u32 size;

    switch (w->unk4) {
    case 0:
        ARR(4, w->unkA) = 0;
        func_ov001_02302ac8(w);
        break;
    case 1:
        ARR(0x44, w->unkA) = 0;
        func_ov001_022fd4f4(w);
        break;
    case 2:
        ARR(8, w->unkA) = 0;
        *(u16 *)(func_ov001_022ffce8() + 0x304) = 0xffff;
        break;
    case 3:
        ARR(0x1c, w->unkA) = 0;
        break;
    }
    ARR(0xbc, w->unk8) = 0;
    if (w->unk90 != NULL) {
        if (!(w->unk14 & 0x10000000))
            func_02039bb0(w->unk90->unk0, w->unk90->unk34);
        if (!(w->unk14 & 0x20000000)) {
            p = (u8 *)w->unk90;
            if ((u8 *)func_02006abc(0) != p) {
                p -= 0x20;
                size = *(u32 *)p;
                if (*(u32 *)(p + size - 4) == 0x02467531) {
                    fill1 = 0xcccccccc;
                    MIi_CpuClear32(fill1, p, size);
                    DC_StoreRange(p, size);
                }
            }
            func_020349b8(data_0208fe80, p);
        }
    }
    if (w->unk94 != NULL) {
        if (!(w->unk14 & 0x10000000))
            func_02031768(w->unk94);
        if (!(w->unk14 & 0x20000000)) {
            p = (u8 *)w->unk94;
            if ((u8 *)func_02006abc(0) != p) {
                p -= 0x20;
                size = *(u32 *)p;
                if (*(u32 *)(p + size - 4) == 0x02467531) {
                    fill2 = 0xcccccccc;
                    MIi_CpuClear32(fill2, p, size);
                    DC_StoreRange(p, size);
                }
            }
            func_020349b8(data_0208fe80, p);
        }
    }
    if (w->unk84 != NULL) {
        if (w->unk88 != NULL) {
            func_02031768(w->unk88);
            func_02031768(w->unk8C);
            if (!(w->unk14 & 0x20000000)) {
                p = (u8 *)w->unk88;
                if ((u8 *)func_02006abc(0) != p) {
                    p -= 0x20;
                    size = *(u32 *)p;
                    if (*(u32 *)(p + size - 4) == 0x02467531) {
                        fill3 = 0xcccccccc;
                        MIi_CpuClear32(fill3, p, size);
                        DC_StoreRange(p, size);
                    }
                }
                func_020349b8(data_0208fe80, p);
                p = (u8 *)w->unk8C;
                if ((u8 *)func_02006abc(0) != p) {
                    p -= 0x20;
                    size = *(u32 *)p;
                    if (*(u32 *)(p + size - 4) == 0x02467531) {
                        fill4 = 0xcccccccc;
                        MIi_CpuClear32(fill4, p, size);
                        DC_StoreRange(p, size);
                    }
                }
                func_020349b8(data_0208fe80, p);
            }
        } else {
            func_02031768(w->unk84);
            if (!(w->unk14 & 0x20000000)) {
                p = (u8 *)w->unk84;
                if ((u8 *)func_02006abc(0) != p) {
                    p -= 0x20;
                    size = *(u32 *)p;
                    if (*(u32 *)(p + size - 4) == 0x02467531) {
                        fill5 = 0xcccccccc;
                        MIi_CpuClear32(fill5, p, size);
                        DC_StoreRange(p, size);
                    }
                }
                func_020349b8(data_0208fe80, p);
            }
        }
    }
    p = (u8 *)w->unk1DC;
    if (p != NULL) {
        if ((u8 *)func_02006abc(0) != p) {
            p -= 0x20;
            size = *(u32 *)p;
            if (*(u32 *)(p + size - 4) == 0x02467531) {
                fill6 = 0xcccccccc;
                MIi_CpuClear32(fill6, p, size);
                DC_StoreRange(p, size);
            }
        }
        func_020349b8(data_0208fe80, p);
    }
    func_020371c8(task);
}
