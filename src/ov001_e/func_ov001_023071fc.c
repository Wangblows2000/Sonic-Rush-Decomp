#include <nitro/types.h>

typedef struct UnkStruct_023071fc_o {
    u8 pad0[0xf4];
} UnkStruct_023071fc_o;

typedef struct UnkStruct_023071fc_a {
    u8 pad0[0x138];
} UnkStruct_023071fc_a;

typedef struct UnkStruct_023071fc {
    UnkStruct_023071fc_o objs[0x20];
    u8 pad1E80[0x1f00 - 0x20 * 0xf4];
    UnkStruct_023071fc_a anims[0x20];
    u8 pad4600[0x4680 - (0x1f00 + 0x20 * 0x138)];
    u8 *unk4680;
    u8 pad4684[0x46a0 - 0x4684];
    void *unk46A0;
} UnkStruct_023071fc;

typedef struct UnkStruct_023071fc_arc {
    u8 pad[0x60];
} UnkStruct_023071fc_arc;

extern void *data_0208fe80;
extern UnkStruct_023071fc *data_ov001_0231f5b8;
extern char data_ov001_0231df78[];

void func_02031768(void *o);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
BOOL func_02021310(void *arc, const char *name, void *narc);
u32 func_02021264(void *arc, u32 idx);
void func_0202566c(u32 p);
BOOL func_020212e0(void *arc);
void func_ov001_02310bf4(void *p);

static inline void Free(void *ptr) {
    u8 *blk = ptr;
    if ((u8 *)func_02006abc(0) != blk) {
        u32 size;
        blk -= 0x20;
        size = *(u32 *)blk;
        if (*(u32 *)(blk + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, blk, size);
            DC_StoreRange(blk, size);
        }
    }
    func_020349b8(data_0208fe80, blk);
}

void func_ov001_023071fc(void) {
    UnkStruct_023071fc *g = data_ov001_0231f5b8;
    UnkStruct_023071fc_arc arc;
    s32 i;

    for (i = 0; i < 0x20; i++)
        func_02031768(&g->objs[i]);
    for (i = 0; i < 0x20; i++)
        func_02031768(&g->anims[i]);
    func_02021310(&arc, data_ov001_0231df78, g->unk4680);
    func_0202566c(func_02021264(&arc, 0));
    func_0202566c(func_02021264(&arc, 3));
    func_020212e0(&arc);
    Free(g->unk4680);
    g->unk4680 = NULL;
    func_ov001_02310bf4(g->unk46A0);
    g->unk46A0 = NULL;
    Free(data_ov001_0231f5b8);
    data_ov001_0231f5b8 = NULL;
}
