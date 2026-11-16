#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkCfg_02341f78;

typedef void (*UnkFunc_02341f78)(void *p);

extern UnkCfg_02341f78 data_022c4560;
extern u32 data_02087cb8;
extern UnkFunc_02341f78 data_ov000_023689c4[][3];
extern void *data_ov000_0236ac1c;
extern void *data_0208fe80;

void *func_02006abc(u32 arg);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void DC_StoreRange(const void *addr, u32 size);
void func_020349b8(void *heap, void *p);

static inline void UnkDbgFree(void *ptr) {
    u8 *p = ptr;
    u32 sz;
    if (func_02006abc(0) != p) {
        p -= 0x20;
        sz = *(u32 *)p;
        if (*(u32 *)(p + sz - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, sz);
            DC_StoreRange(p, sz);
        }
    }
    func_020349b8(data_0208fe80, p);
}

void func_ov000_02341f78(void) {
    UnkFunc_02341f78 f;
    void *p;

    data_02087cb8 &= ~0xc00;
    f = data_ov000_023689c4[data_022c4560.unk1C][data_022c4560.unk1D];
    if (f != NULL)
        f(data_ov000_0236ac1c);
    p = data_ov000_0236ac1c;
    if (p == NULL)
        return;
    UnkDbgFree(p);
    data_ov000_0236ac1c = NULL;
}
