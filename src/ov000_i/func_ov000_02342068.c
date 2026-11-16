#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkCfg_02342068;

typedef void (*UnkFunc_02342068)(void *p);

extern UnkCfg_02342068 data_022c4560;
extern UnkFunc_02342068 data_ov000_02368a9c[][3];
extern void *data_ov000_0236ac1c;
extern void *data_0208fe80;

void *func_02034a40(void *heap, u32 size, s32 align);
void *func_02006abc(u32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void DC_StoreRange(const void *addr, u32 size);
void func_ov000_02341358(void);

static inline void *UnkDbgAlloc(u32 size, s32 align) {
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

void func_ov000_02342068(void) {
    void *p;
    vu16 zero;

    if (data_ov000_02368a9c[data_022c4560.unk1C][data_022c4560.unk1D] != NULL) {
        p = UnkDbgAlloc(0xc54, 0x20);
        zero = 0;
        data_ov000_0236ac1c = p;
        MIi_CpuClear16(zero, p, 0xc54);
        data_ov000_02368a9c[data_022c4560.unk1C][data_022c4560.unk1D](data_ov000_0236ac1c);
    }
    func_ov000_02341358();
}
