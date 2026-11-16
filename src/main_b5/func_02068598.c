#include <nitro/types.h>

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

typedef struct {
    u8 *buf;
    u16 ref;
} UnkRef_02068598;

extern void *data_0208fe80;
extern char data_0207cf8c[];
extern char data_0207cf90[];

void func_02021310(void *f, void *path, void *arc);
void func_020212e0(void *f);
void *func_020687b8(UnkRef_02068598 *s, const char *name, void *arc);
void func_0202c474(char *dst, const char *src);
u32 func_02033ddc(const char *path);
void MIi_CpuCopy32(const void *src, void *dest, u32 size);

static inline u8 *Alloc_02068598(u32 sz) {
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

void func_02068598(UnkRef_02068598 *s, const char *name, void *arc) {
    void *src;
    u32 size;
    char path[32] = {0};
    u32 f[0x17];

    if (s->ref != 0)
        return;
    func_02021310(f, data_0207cf8c, arc);
    src = func_020687b8(s, name, arc);
    func_0202c474(path, data_0207cf90);
    func_0202c474(path, name);
    size = func_02033ddc(path);
    s->buf = Alloc_02068598((size + 0x43) & ~0x1f);
    s->ref &= ~0x8000;
    MIi_CpuCopy32(src, s->buf, size);
    func_020212e0(f);
}
