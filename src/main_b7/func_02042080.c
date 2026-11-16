#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    u8 pad4[0x16];
    u16 unk1a;
} UnkObj_02042080;

extern u32 data_02087cb8;
extern u8 data_0224ffa0[];
u32 func_0203e944(void);
void func_0203dc28(void);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void func_02041fac(void *);

void func_02042080(UnkObj_02042080 *o) {
    if ((func_0203e944() & ~1) == 0) {
        data_02087cb8 &= ~0x100;
        data_02087cb8 |= 0x200;
        func_0203dc28();
        {
            vu32 v = 0;
            MIi_CpuClear32(v, data_0224ffa0, 4);
        }
        o->unk1a = 0;
        o->func = func_02041fac;
    }
}
