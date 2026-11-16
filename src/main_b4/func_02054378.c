#include <nitro/types.h>

typedef struct {
    u8 data[0x44];
} UnkElem_02054378;

extern s32 data_0207b8a4;
extern s32 data_0207b894;
extern s32 data_02075674[];
extern s32 data_0207565c[];
extern UnkElem_02054378 *data_022c466c;
extern UnkElem_02054378 *data_022c4664;
extern s32 data_022c4670;
extern s32 data_022c4668;
extern void *data_0208fe80;

void func_0200bad8(UnkElem_02054378 *e);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

static inline void Free_02054378(void *ptr) {
    u8 *p = ptr;
    if ((u8 *)func_02006abc(0) != p) {
        u32 size;
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    func_020349b8(data_0208fe80, p);
}

void func_02054378(void) {
    s32 i;
    UnkElem_02054378 *e;
    s32 j;
    UnkElem_02054378 *e2;

    e = data_022c466c;
    for (i = 0; i < data_02075674[data_0207b8a4]; i++) {
        func_0200bad8(e);
        e++;
    }
    e2 = data_022c4664;
    for (j = 0; j < data_0207565c[data_0207b8a4]; j++) {
        func_0200bad8(e2);
        e2++;
    }
    Free_02054378(data_022c466c);
    data_022c466c = NULL;
    Free_02054378(data_022c4664);
    data_022c4664 = NULL;
    data_022c4670 = 0;
    data_022c4668 = 0;
    data_0207b894 = -1;
    data_0207b8a4 = -1;
}
