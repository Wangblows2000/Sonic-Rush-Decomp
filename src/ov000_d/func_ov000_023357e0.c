#include <nitro/types.h>

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
} UnkStruct_023357e0;

extern void *data_0208fe80;
extern UnkStruct_023357e0 data_ov000_0236aa9c;

void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

static inline void UnkFree_023357e0(void **pp) {
    void *p;
    u32 size;
    if (*pp != NULL) {
        p = *pp;
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
    *pp = NULL;
}

void func_ov000_023357e0(void) {
    UnkStruct_023357e0 *d = &data_ov000_0236aa9c;

    UnkFree_023357e0(&d->unk0);
    UnkFree_023357e0(&d->unk4);
    UnkFree_023357e0(&d->unk8);
}
