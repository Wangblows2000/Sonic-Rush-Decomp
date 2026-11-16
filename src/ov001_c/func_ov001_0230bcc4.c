#include <nitro/types.h>

typedef struct UnkStruct_0230bcc4_o {
    void *res[7];
    void *res2[4];
    u8 *unk2C;
} UnkStruct_0230bcc4_o;

typedef struct UnkStruct_0230bcc4 {
    u8 pad0[0x1d0];
    void *heap;
    u8 *mem;
} UnkStruct_0230bcc4;

extern UnkStruct_0230bcc4 *data_ov001_0231f5c0;
extern void *data_0208fe80;

void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
void func_ov001_0230b09c(u8 *blk);
void func_0201b348(void);
void func_0201da54(void *h);
void func_0201db14(void *h);
void func_02036894(void);

static inline void Free_0230bcc4(void *p) {
    u32 size;
    if (func_02006abc(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    func_020349b8(data_0208fe80, p);
}

void func_ov001_0230bcc4(UnkStruct_0230bcc4_o *o) {
    UnkStruct_0230bcc4 *g = data_ov001_0231f5c0;
    s32 i;

    for (i = 0; i < 7; i++) {
        Free_0230bcc4(o->res[i]);
        o->res[i] = NULL;
    }
    for (i = 0; i < 4; i++) {
        Free_0230bcc4(o->res2[i]);
        o->res2[i] = NULL;
    }
    func_ov001_0230b09c(o->unk2C);
    o->unk2C = NULL;
    func_0201b348();
    func_0201da54(g->heap);
    func_0201db14(g->heap);
    Free_0230bcc4(g->mem);
    func_02036894();
}
