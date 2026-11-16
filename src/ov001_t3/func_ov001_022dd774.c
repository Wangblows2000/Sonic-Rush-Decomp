#include <nitro/types.h>

typedef struct {
    void *ptr[2];
    u8 pad8[0x20];
} UnkA_022dd774;

typedef struct {
    void *ptr[2];
    u8 pad8[0x2c];
} UnkB_022dd774;

typedef struct {
    u8 pad0[0x40];
    UnkA_022dd774 a[2];
    u8 pad90[0x9c - 0x90];
    UnkB_022dd774 b[8];
    u8 pad[0xdac - (0x9c + 8 * 0x34)];
    void *buf;
} UnkStruct_022dd774;

extern void *data_0208fe80;

void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
void func_02039bb0(s32 a, void *p);

#pragma thumb on
static inline void Free_022dd774(void *p) {
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

void func_ov001_022dd774(UnkStruct_022dd774 *p) {
    s32 i;
    s32 j;

    if (p->buf != NULL) {
        Free_022dd774(p->buf);
        p->buf = NULL;
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if (p->a[i].ptr[j] != NULL) {
                func_02039bb0(1, p->a[i].ptr[j]);
                p->a[i].ptr[j] = NULL;
            }
        }
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 2; j++) {
            if (p->b[i].ptr[j] != NULL) {
                func_02039bb0(1, p->b[i].ptr[j]);
                p->b[i].ptr[j] = NULL;
            }
        }
    }
}
#pragma thumb reset
