#include <nitro/types.h>

typedef struct UnkJob_02035754 {
    struct UnkJob_02035754 *next;
    u32 type;
    u32 dstType;
    u32 dst;
    union {
        u32 src;
        u16 arg;
    } u;
    u16 len;
} UnkJob_02035754;

extern UnkJob_02035754 *data_02250438;
extern UnkJob_02035754 *data_02250434;
extern u32 data_0208922c[];
extern u32 data_02087d04[];
extern u32 data_0208a754[];

u32 func_0200311c(void);
u32 func_020030f8(void);
u32 func_02003080(void);
u32 func_02003058(void);
u32 func_020030d0(void);
void func_02003788(u32 s);
void func_020036d0(u32 s);
void func_02003204(u32 s);
void func_02003178(u32 s);
void func_020033d8(u32 s);
void MIi_CpuCopyFast(const void *src, void *dst, u32 size);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
u32 func_02035b30(u32 src);
u32 func_02035b24(u32 src);
u32 func_02033a54(u32 src);
void func_02033a68(u32 src, u32 dst, u32 size);
void func_02008648(u32 src, u32 dst);
void func_02008720(u32 src, u32 dst);
void func_020087f8(u32 src, u32 dst);
void DC_StoreRange(u32 addr, u32 size);
void func_02035660(u32 dst, u16 n);
void func_020355a8(u32 dst, u16 n);
void func_02035508(u32 dst, u32 a, u16 n);
void func_020354b8(u32 dst, u16 n);
void func_02035418(u32 dst, u32 a, u16 n);
void func_020353b8(u32 dst, u16 n);
void func_02037e68(UnkJob_02035754 *job);

static inline void UnkCopy_02035754(const u8 *src, u8 *d, u32 size) {
    u32 n;
    if ((((u32)src | (u32)d) & 3) == 0) {
        n = size & ~0x1f;
        if (n != 0) {
            MIi_CpuCopyFast(src, d, n);
            src += n;
            d += n;
            size &= 0x1f;
        }
        n = size & ~3;
        if (n != 0) {
            MIi_CpuCopy32(src, d, n);
            src += n;
            d += n;
            size &= 3;
        }
    }
    if ((((u32)src | (u32)d) & 1) == 0) {
        n = size & ~1;
        if (n != 0) {
            MIi_CpuCopy16(src, d, n);
            src += n;
            d += n;
            size &= 1;
        }
    }
    if (size != 0) {
        MI_CpuCopy8(src, d, size);
    }
}

void func_02035754(void) {
    u32 saved[5];
    u32 flags = 0;
    while (data_02250438 != NULL) {
        UnkJob_02035754 *job = data_02250438;
        u32 dst = 0;
        switch (job->dstType) {
        case 0:
            dst = job->dst;
            break;
        case 1: {
            u32 a = job->dst;
            u32 page = a >> 13;
            dst = a + data_0208922c[page + 31] - (page << 13);
            if (!(flags & 2)) {
                saved[0] = func_0200311c();
                flags |= 2;
            }
            break;
        }
        case 2: {
            u32 a = job->dst;
            u32 page = a >> 13;
            dst = a + data_0208922c[page + 35] - (page << 13);
            if (!(flags & 4)) {
                saved[1] = func_020030f8();
                flags |= 4;
            }
            break;
        }
        case 3: {
            u32 a = job->dst;
            u32 page = a >> 13;
            dst = a + data_02087d04[page + 31] - (page << 13);
            if (!(flags & 8)) {
                saved[2] = func_02003080();
                flags |= 8;
            }
            break;
        }
        case 4: {
            u32 a = job->dst;
            u32 page = a >> 13;
            dst = a + data_02087d04[page + 35] - (page << 13);
            if (!(flags & 0x10)) {
                saved[3] = func_02003058();
                flags |= 0x10;
            }
            break;
        }
        case 5: {
            u32 a = job->dst;
            u32 page = a >> 14;
            dst = a + data_0208a754[page + 4] - (page << 14);
            if (!(flags & 0x20)) {
                saved[4] = func_020030d0();
                flags |= 0x20;
            }
            break;
        }
        }
        switch (job->type) {
        case 0:
            UnkCopy_02035754((u8 *)job->u.src, (u8 *)dst, job->len * 2);
            break;
        case 1: {
            u32 src = job->u.src;
            switch (func_02035b30(src)) {
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
                func_02033a68(func_02033a54(src), dst, func_02035b24(src));
                break;
            }
            DC_StoreRange(dst, func_02035b24(src));
            break;
        }
        case 2:
            func_02035660(dst, job->u.arg);
            break;
        case 3:
            func_020355a8(dst, job->u.arg);
            break;
        case 4:
            func_02035508(dst, job->u.src, job->len);
            break;
        case 5:
            func_020354b8(dst, job->u.arg);
            break;
        case 6:
            func_02035418(dst, job->u.src, job->len);
            break;
        case 7:
            func_020353b8(dst, job->u.arg);
            break;
        }
        func_02037e68(data_02250438);
        data_02250438 = data_02250438->next;
        if (data_02250438 == NULL) {
            data_02250434 = NULL;
        }
    }
    if (flags & 2) {
        func_02003788(saved[0]);
    }
    if (flags & 4) {
        func_020036d0(saved[1]);
    }
    if (flags & 8) {
        func_02003204(saved[2]);
    }
    if (flags & 0x10) {
        func_02003178(saved[3]);
    }
    if (flags & 0x20) {
        func_020033d8(saved[4]);
    }
}
