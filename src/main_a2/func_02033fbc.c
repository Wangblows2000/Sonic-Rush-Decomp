#include <nitro/types.h>

typedef struct {
    u8 unk0[0x20];
    u32 top;
    u32 bottom;
    u8 unk28[0x20];
} UnkFile_02033fbc;

extern void *data_0208fe80;

void func_0200be70(UnkFile_02033fbc *f);
BOOL func_0200bb20(UnkFile_02033fbc *f, const char *path);
s32 func_0200b99c(UnkFile_02033fbc *f, void *dst, s32 len);
BOOL func_0200bad8(UnkFile_02033fbc *f);
void *func_02034a40(void *heap, u32 size, s32 align);
void *func_02006abc(u32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);

static inline void *UnkAlloc_02033fbc(u32 size, s32 align) {
    u32 sz = (size + 0x43) & ~0x1f;
    u8 *p = func_02034a40(data_0208fe80, sz, align);
    if (func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, sz);
        *(u32 *)p = sz;
        *(u32 *)(p + sz - 4) = 0x2467531;
        DC_StoreRange(p, sz);
        p += 0x20;
    }
    return p;
}

void *func_02033fbc(const char *path, void *dst) {
    UnkFile_02033fbc file;
    u32 size;
    func_0200be70(&file);
    func_0200bb20(&file, path);
    size = file.bottom - file.top;
    if (dst == NULL) {
        dst = UnkAlloc_02033fbc(size, 0x20);
    } else if (dst == (void *)-1) {
        dst = UnkAlloc_02033fbc(size, -0x20);
    }
    func_0200b99c(&file, dst, size);
    func_0200bad8(&file);
    DC_StoreRange(dst, size);
    return dst;
}
