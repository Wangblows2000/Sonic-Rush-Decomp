#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[0x10];
    s32 unk14;
    s32 unk18;
    u8 pad1c[0x14];
    void *unk30;
} UnkStruct_022cff74;

extern void *data_02250438;
extern void *data_02250434;

void func_0203438c(UnkStruct_022cff74 *p);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_020356bc(void *p, u32 a, u32 b);
void func_02035754(void);
void func_02035704(void *p, s32 a, s32 b, s32 c);

#pragma thumb on
static inline void LoadPal_022cff74(void *src, u32 a, u32 b) {
    void *s0 = data_02250438;
    void *s1 = data_02250434;
    data_02250438 = NULL;
    data_02250434 = NULL;
    func_020356bc(src, a, b);
    func_02035754();
    data_02250438 = s0;
    data_02250434 = s1;
}

void func_ov001_022cff74(UnkStruct_022cff74 *p, u32 n, u32 b, void *buf) {
    p->flags &= ~0x38;
    func_0203438c(p);
    {
        vu32 z = 0;
        MIi_CpuClear32(z, buf, n * 2);
    }
    LoadPal_022cff74(p->unk30, 0, b);
    func_02035704(buf, (u16)n, p->unk14, p->unk18);
}
#pragma thumb reset
