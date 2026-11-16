#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 flags;
} UnkStruct_022c4560;

typedef struct {
    u8 *buf0;
    u32 size0;
    u8 *buf1;
    u32 size1;
} UnkStruct_022c4520;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_022c4520 data_022c4520;
extern void *data_0208fe78;

u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

static inline void Free_0204cb10(u8 *p) {
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
    func_020349b8(data_0208fe78, p);
}

void func_0204cb10(void) {
    UnkStruct_022c4520 *s;

    if (data_022c4560.flags & 0x40)
        return;
    s = &data_022c4520;
    if (s->buf0 != NULL) {
        Free_0204cb10(s->buf0);
        s->buf0 = NULL;
        s->size0 = 0;
    }
    if (s->buf1 != NULL) {
        Free_0204cb10(s->buf1);
        s->buf1 = NULL;
        s->size1 = 0;
    }
}
