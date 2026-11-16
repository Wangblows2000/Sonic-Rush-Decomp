#include <nitro/types.h>

typedef struct UnkWork_02060d00 {
    void (*fn)(struct UnkWork_02060d00 *w);
    u8 pad4[0x1c];
    s32 unk20;
    void *unk24;
} UnkWork_02060d00;
typedef struct { u8 pad0[4]; s32 unk4; } UnkInfo_02060d00;
typedef struct { u8 pad0[8]; s16 unk8; } UnkBlend_02060d00;
extern s16 data_022c4da0;
extern void *data_022b4574;
extern UnkBlend_02060d00 data_0208926c;

void *func_020604dc(s32 a, u8 b);
void func_020371c8(void *t);
UnkInfo_02060d00 *func_02060470(void *p, s32 a);
void func_02060e54(s32 a, s32 b);
void func_02062c9c(void);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_020602dc(void);
u32 func_02060e70(s32 a);
void func_02060288(void);
void func_0206086c(UnkWork_02060d00 *w);
void func_020607d4(UnkWork_02060d00 *w);
void func_02060ce0(UnkWork_02060d00 *w);

#pragma thumb on
void func_02060d00(UnkWork_02060d00 *w) {
    vu32 zero;

    w->unk24 = func_020604dc(w->unk20, data_022c4da0);
    if (w->unk24 == NULL) {
        func_020371c8(data_022b4574);
        return;
    }
    if (func_02060470(w->unk24, 0)->unk4 == 3) {
        func_02060e54(0, 2);
        data_0208926c.unk8 = 0;
    }
    func_02062c9c();
    zero = 0;
    MIi_CpuClear32(zero, (void *)0x06002000, 0xa000);
    func_020602dc();
    if (!(func_02060e70(0) & 2))
        func_02060288();
    func_0206086c(w);
    func_020607d4(w);
    w->fn = func_02060ce0;
}
#pragma thumb reset
