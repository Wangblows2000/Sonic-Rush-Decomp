#include <nitro/types.h>

typedef struct UnkWork_02060f40 {
    void (*fn)(struct UnkWork_02060f40 *w);
    u8 pad4[0x1c];
    s32 unk20;
    u8 pad24[0x10];
} UnkWork_02060f40;
typedef struct { u8 pad0[0x10]; UnkWork_02060f40 *work; } UnkTask_02060f40;
extern UnkTask_02060f40 *data_022c4da4;
extern u8 data_02072cb8;

UnkTask_02060f40 *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_02060da0(void);
void func_02060dd8(void);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void func_02060d00(UnkWork_02060f40 *w);
void func_02038954(s32 a, s32 b, s32 c);
void func_0206069c(UnkWork_02060f40 *w);
void func_02060ab0(UnkWork_02060f40 *w);

#pragma thumb on
void func_02060f40(void) {
    UnkTask_02060f40 *t;
    UnkWork_02060f40 *w;
    vu16 zero;
    u32 lang;

    t = func_020372d0(func_02060da0, func_02060dd8, 7, 0xeffd, 3, sizeof(UnkWork_02060f40));
    data_022c4da4 = t;
    if (t != (UnkTask_02060f40 *)func_02006abc(0)) {
        w = t->work;
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_02060f40));
        switch (data_02072cb8) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            lang = data_02072cb8;
            break;
        default:
            lang = 1;
            break;
        }
        w->unk20 = lang;
        w->fn = func_02060d00;
        func_02038954(1, 1, 1);
        func_0206069c(w);
        func_02060ab0(w);
    }
}
#pragma thumb reset
