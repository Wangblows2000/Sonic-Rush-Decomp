#include <nitro/types.h>

typedef struct UnkStruct_022de240_b {
    void *unk0;
    u8 pad4[0x18 - 4];
    u8 objs[6][0x138];
    void *res[6];
} UnkStruct_022de240_b;

extern void *data_0208fe80;

void func_02031768(void *o);
void func_0202566c(void *r);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

static inline void Free_022de240(void *p) {
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

void func_ov001_022de240(void *w, UnkStruct_022de240_b *b) {
    u8 i;

    for (i = 0; i < 6; i++) {
        func_02031768(b->objs[i]);
        if (b->res[i] != NULL) {
            func_0202566c(b->res[i]);
            Free_022de240(b->res[i]);
        }
        b->res[i] = NULL;
    }
    if (b->unk0 != NULL) {
        Free_022de240(b->unk0);
        b->unk0 = NULL;
    }
}
