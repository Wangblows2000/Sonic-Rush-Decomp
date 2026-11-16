#include <nitro/types.h>

typedef struct UnkWork_02056a00 {
    void (*fn)(struct UnkWork_02056a00 *w);
    void *unk4;
    s8 unk8;
    u8 pad9[0x1343];
} UnkWork_02056a00;
typedef struct { u8 pad0[0x10]; UnkWork_02056a00 *work; } UnkTask_02056a00;
typedef struct { u8 pad0[0x14]; s32 unk14; } UnkInfo_02056a00;
extern s16 data_022c4678;
extern UnkTask_02056a00 *data_022b4574;

UnkTask_02056a00 *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_020569b8(void);
void func_020569f4(void);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void *func_02058350(void);
void func_020371c8(UnkTask_02056a00 *t);
void func_02056988(UnkWork_02056a00 *w);

#pragma thumb on
void func_02056a00(void) {
    UnkTask_02056a00 *t = func_020372d0(func_020569b8, func_020569f4, 0, 0x3efe, 3, sizeof(UnkWork_02056a00));
    UnkWork_02056a00 *w;
    vu16 zero;

    if (t != (UnkTask_02056a00 *)func_02006abc(0)) {
        w = t->work;
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_02056a00));
        w->unk4 = func_02058350();
        if (w->unk4 == NULL) {
            func_020371c8(data_022b4574);
            return;
        }
        w->unk8 = data_022c4678;
        w->fn = func_02056988;
    }
}
#pragma thumb reset
