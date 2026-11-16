#include <nitro/types.h>

typedef struct UnkStruct_0230f8b4 {
    void *unk0;
    u8 objs[9][0xf4];
    void *res[5];
    u8 anims[5][0x138];
    void *unkEC4;
} UnkStruct_0230f8b4;

extern UnkStruct_0230f8b4 *data_ov001_0231f658;
extern void *data_0208fe80;

void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
void func_02031768(void *o);
void func_0202566c(void *res);

static inline void Free_0230f8b4(void *p) {
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

void func_ov001_0230f8b4(void) {
    UnkStruct_0230f8b4 *g = data_ov001_0231f658;
    s32 i;
    s32 j;

    for (i = 0; i < 9; i++)
        func_02031768(g->objs[i]);
    for (i = 0; i < 5; i++) {
        func_0202566c(g->res[i]);
        func_02031768(g->anims[i]);
        Free_0230f8b4(g->res[i]);
    }
    if (g->unk0 != NULL) {
        Free_0230f8b4(g->unk0);
        g->unk0 = NULL;
    }
    Free_0230f8b4(g->unkEC4);
    g->unkEC4 = NULL;
    Free_0230f8b4(data_ov001_0231f658);
    data_ov001_0231f658 = NULL;
}
