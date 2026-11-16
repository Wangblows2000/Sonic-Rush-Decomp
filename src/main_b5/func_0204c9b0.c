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

void *func_ov000_023355d0(void);
void *func_ov000_023355c0(void);
void MIi_CpuCopy32(const void *src, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);

static inline void MI_CpuCopy32(const void *src, void *dest, u32 size) {
    MIi_CpuCopy32(src, dest, size);
}

static inline void MI_CpuCopy16(const void *src, void *dest, u32 size) {
    MIi_CpuCopy16(src, dest, size);
}

void func_0204c9b0(void) {
    UnkStruct_022c4520 *s;

    if (!(data_022c4560.flags & 0x40))
        return;
    s = &data_022c4520;
    if ((s->size0 & 3) == 0)
        MI_CpuCopy32(s->buf0, func_ov000_023355d0(), s->size0);
    else
        MI_CpuCopy16(s->buf0, func_ov000_023355d0(), s->size0);
    if ((s->size1 & 3) == 0)
        MI_CpuCopy32(s->buf1, func_ov000_023355c0(), s->size1);
    else
        MI_CpuCopy16(s->buf1, func_ov000_023355c0(), s->size1);
}
