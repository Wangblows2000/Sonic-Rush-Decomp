#include <nitro/types.h>

typedef struct {
    u32 base;
    u8 *src;
    u8 *dst;
    u32 size;
} UnkBuf_02017410;

void MI_CpuFill8(void *dest, u8 data, u32 size);
void MI_CpuCopy8(const void *src, void *dest, u32 size);

void func_02017410(UnkBuf_02017410 *b, u32 start, u32 end, BOOL clear)
{
    u32 base = b->base;
    u32 limit;
    if (start < 0x4000) {
        start = 0x4000;
    }
    if (end > 0x8000) {
        end = 0x8000;
    }
    if (start < base) {
        start = base;
    }
    limit = base + b->size;
    if (end > limit) {
        end = limit;
    }
    if (start >= end) {
        return;
    }
    if (clear) {
        MI_CpuFill8(b->dst + start, 0, end - start);
    } else {
        MI_CpuCopy8(b->src + start, b->dst + start, end - start);
    }
}
