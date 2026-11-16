#include <nitro/types.h>

typedef struct {
    u8 pad0[0x34];
    void *unk34;
    u8 pad38[0x7c - 0x38];
    void *unk7C;
} UnkSub_0231a9b0;

typedef struct {
    u8 pad0[0x20c];
    UnkSub_0231a9b0 sub;
    u8 pad28C[0x29c - 0x28c];
    void *unk29C;
    u32 unk2A0;
} UnkWork_0231a9b0;

extern void *gMainHeap;

void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void ObjVram_Free(s32 a, void *p);

void func_ov000_0231a9b0(UnkWork_0231a9b0 *w) {
    void *p;
    u32 size;
    UnkSub_0231a9b0 *s;

    s = &w->sub;
    if (s->unk34 != NULL)
        ObjVram_Free(0, s->unk34);
    if (s->unk7C != NULL)
        ObjVram_Free(1, s->unk7C);
    if (!(w->unk2A0 & 1))
        return;
    p = w->unk29C;
    if (func_02006abc(0) != p) {
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
