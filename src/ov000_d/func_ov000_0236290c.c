#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1010];
    void *res[11];
} UnkStruct_0236290c;

extern void *data_0208fe78;
extern const char **data_ov000_0236995c[];
extern u8 data_02072cb8;

u32 func_02033ddc(const char *path);
void *func_02033fbc(const char *path, void *dst);
void *func_02034a40(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);

static inline void *UnkAlloc_0236290c(u32 size, s32 align) {
    u32 sz = (size + 0x43) & ~0x1f;
    u8 *p = func_02034a40(data_0208fe78, sz, align);
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

static inline s32 UnkLang_0236290c(u8 cur) {
    switch (cur) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return cur;
    default:
        return 1;
    }
}

#pragma thumb on

void func_ov000_0236290c(UnkStruct_0236290c *w) {
    const char **tbl = data_ov000_0236995c[UnkLang_0236290c(data_02072cb8)];
    s32 i;

    for (i = 0; i < 11; i++) {
        w->res[i] = UnkAlloc_0236290c(func_02033ddc(tbl[i]), -0x20);
        func_02033fbc(tbl[i], w->res[i]);
    }
}

#pragma thumb reset
