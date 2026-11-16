#include <nitro/types.h>

typedef struct UnkJob_0203383c {
    struct UnkJob_0203383c *next;
    u32 type;
    u32 dstType;
    u32 dst;
    u32 src;
    u32 size;
} UnkJob_0203383c;

extern UnkJob_0203383c *data_0208f7e0;
extern UnkJob_0203383c *data_0208f7dc;
extern u32 data_0208a754[];

u32 func_020030e4(void);
void func_020034cc(u32 state);
void func_02033a68(u32 src, u32 dst, u32 size);
void func_02007da4(u32 ch, u32 src, u32 dst, u32 size);
void func_02007d2c(u32 ch, u32 src, u32 dst, u32 size);
u32 func_02033a5c(u32 src);
u32 func_02033a54(u32 src);
u32 func_02033a48(u32 src);
void func_02008648(u32 src, u32 dst);
void func_02008720(u32 src, u32 dst);
void func_020087f8(u32 src, u32 dst);
void DC_StoreRange(u32 addr, u32 size);
void func_02037e68(UnkJob_0203383c *job);

void func_0203383c(void) {
    u32 saved;
    u32 flags = 0;
    while (data_0208f7e0 != NULL) {
        UnkJob_0203383c *job = data_0208f7e0;
        u32 dst = 0;
        switch (job->dstType) {
        case 0:
            dst = job->dst;
            break;
        case 1: {
            u32 a = job->dst;
            u32 page = a >> 17;
            dst = a + data_0208a754[page] - (page << 17);
            if (!(flags & 2)) {
                saved = func_020030e4();
                flags |= 2;
            }
            break;
        }
        }
        switch (job->type) {
        case 0: {
            u32 src = job->src;
            u32 size = job->size;
            if (src >= 0x2000000 && src < 0x2400000 && dst >= 0x2000000 && dst < 0x2400000) {
                func_02033a68(src, dst, size);
            } else {
                u32 n;
                if (((src | dst) & 3) == 0 && (n = size & ~3) != 0) {
                    func_02007da4(3, src, dst, n);
                    src += n;
                    dst += n;
                    size &= 3;
                }
                if (size != 0) {
                    func_02007d2c(3, src, dst, size);
                }
            }
            break;
        }
        case 1: {
            u32 src = job->src;
            switch (func_02033a5c(src)) {
            case 0x10:
                func_02008648(src, dst);
                break;
            case 0x20:
                func_02008720(src, dst);
                break;
            case 0x30:
                func_020087f8(src, dst);
                break;
            default:
                func_02033a68(func_02033a54(src), dst, func_02033a48(src));
                break;
            }
            DC_StoreRange(dst, func_02033a48(src));
            break;
        }
        }
        func_02037e68(data_0208f7e0);
        data_0208f7e0 = data_0208f7e0->next;
        if (data_0208f7e0 == NULL) {
            data_0208f7dc = NULL;
        }
    }
    if (flags & 2) {
        func_020034cc(saved);
    }
}
