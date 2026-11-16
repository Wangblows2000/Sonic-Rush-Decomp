#include <nitro/types.h>

typedef struct {
    u8 pad0[0xE14];
    void *res[8];
} UnkStruct_0235f024;

extern void *data_0208fe78;
extern const char *data_ov000_023692b4[];
extern const char *data_ov000_023692d4[];
extern u8 data_02072cb8;

u32 func_02033ddc(const char *path);
void *func_02033fbc(const char *path, void *dst);
void *func_02034a40(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);

static inline void *UnkAlloc_0235f024(u32 size, s32 align) {
    u8 *p;
    size = (size + 0x43) & ~0x1f;
    p = func_02034a40(data_0208fe78, size, align);
    if (func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x2467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

static inline s32 UnkLang_0235f024(u8 cur) {
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

void func_ov000_0235f024(UnkStruct_0235f024 *w) {
    s32 i;
    const char **tbl;
    u8 cur;

    i = 0;
    tbl = data_ov000_023692b4;
    cur = data_02072cb8;
    for (; i < 8; i++) {
        if (i == 1) {
            w->res[i] = UnkAlloc_0235f024(func_02033ddc(data_ov000_023692d4[UnkLang_0235f024(cur)]), -0x20);
            func_02033fbc(data_ov000_023692d4[UnkLang_0235f024(cur)], w->res[i]);
        } else {
            w->res[i] = UnkAlloc_0235f024(func_02033ddc(tbl[i]), -0x20);
            func_02033fbc(tbl[i], w->res[i]);
        }
    }
}

#pragma thumb reset
