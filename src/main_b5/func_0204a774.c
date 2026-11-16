#include <nitro/types.h>

typedef struct {
    const char *name;
    u32 unk4;
    u32 unk8;
    u32 unkc;
} UnkEntry_0204a774;

typedef struct {
    u8 pad0[0x5c];
    void *res;
} UnkWork_0204a774;

extern UnkEntry_0204a774 data_02074408[];
extern u8 *data_ov000_02369b18[];
extern void *data_0208fe80;
extern u32 data_022c4560;

s32 func_0202c348(const char *a, const char *b);
void func_020257f8(void *p);
u32 func_020380d8(void *p);
void func_020380a0(void *p, void *buf);
void func_0205453c(s32 a, u32 b);
void func_0204525c(void);
u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);

static inline u8 *Alloc_0204a774(u32 sz) {
    u32 size = sz;
    u8 *p = func_02034a40(data_0208fe80, size, 0x20);
    if ((u8 *)func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

void func_0204a774(UnkWork_0204a774 *w, const char *name) {
    UnkEntry_0204a774 *tbl;
    s32 i;

    tbl = data_02074408;
    for (i = 0; i < 5; i++) {
        if (func_0202c348(tbl[i].name, name) == 0)
            break;
    }
    func_020257f8(w->res);
    data_ov000_02369b18[i] = Alloc_0204a774((func_020380d8(w->res) + 0x43) & ~0x1f);
    func_020380a0(w->res, data_ov000_02369b18[i]);
    if (i == 4) {
        func_0205453c(1, data_022c4560);
        func_0204525c();
    }
}
