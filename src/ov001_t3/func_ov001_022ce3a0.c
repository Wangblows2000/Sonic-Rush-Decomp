#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
} UnkSpr_022ce3a0;

typedef struct {
    u8 pad0[0x78];
    UnkSpr_022ce3a0 a[11];
    u8 pad390[0x3bc - 0x390];
    UnkSpr_022ce3a0 b;
    u8 pad404[0x408 - 0x404];
    UnkSpr_022ce3a0 c[5];
    u8 pad570[0x584 - 0x570];
    UnkSpr_022ce3a0 d[8];
    u8 pad7c4[0x7e4 - 0x7c4];
    UnkSpr_022ce3a0 e[14];
    u8 padbd4[0xc0c - 0xbd4];
    UnkSpr_022ce3a0 f[41];
    u8 pad1794[0x1838 - 0x1794];
    UnkSpr_022ce3a0 g[12];
    u8 pad1b98[0x1de8 - 0x1b98];
    void *ptrs[7];
    u8 pad1e04[4];
    void *res[16];
} UnkStruct_022ce3a0;

extern void *gMainHeap;

void func_ov001_022ce374(UnkSpr_022ce3a0 *s);
void ObjVram_Free(s32 a, void *p);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

#pragma thumb on
static inline void Free_022ce3a0(void *p) {
    u32 size;
    if (OS_GetArenaLo(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
}

void func_ov001_022ce3a0(UnkStruct_022ce3a0 *p) {
    s32 i;

    if (p == NULL) return;
    for (i = 0; i < 11; i++) func_ov001_022ce374(&p->a[i]);
    func_ov001_022ce374(&p->b);
    for (i = 0; i < 5; i++) func_ov001_022ce374(&p->c[i]);
    for (i = 0; i < 8; i++) func_ov001_022ce374(&p->d[i]);
    for (i = 0; i < 14; i++) func_ov001_022ce374(&p->e[i]);
    for (i = 0; i < 41; i++) func_ov001_022ce374(&p->f[i]);
    for (i = 0; i < 12; i++) func_ov001_022ce374(&p->g[i]);
    for (i = 0; i < 7; i++) ObjVram_Free(1, p->ptrs[i]);
    for (i = 0; i < 16; i++) {
        if (p->res[i] != NULL) {
            Free_022ce3a0(p->res[i]);
            p->res[i] = NULL;
        }
    }
}
#pragma thumb reset
