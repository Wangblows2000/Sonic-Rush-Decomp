#include <nitro/types.h>

typedef struct UnkWork_02058364 {
    void (*fn)(struct UnkWork_02058364 *w);
    u8 pad4[0xc];
    s32 unk10;
    u8 pad14[0x24];
} UnkWork_02058364;
typedef struct { u8 pad0[0x10]; UnkWork_02058364 *work; } UnkTask_02058364;
extern UnkTask_02058364 *data_022c467c;
extern u8 gGameLanguage;

UnkTask_02058364 *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_02058280(void);
void func_020582e4(void);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void func_02058218(UnkWork_02058364 *w);
void func_020574d8(UnkWork_02058364 *w);
void func_02058034(UnkWork_02058364 *w);

#pragma thumb on
void func_02058364(void) {
    UnkTask_02058364 *t;
    UnkWork_02058364 *w;
    vu16 zero;
    u32 lang;

    t = Task_Create(func_02058280, func_020582e4, 0, 0x3efe, 3, sizeof(UnkWork_02058364));
    data_022c467c = t;
    if (t != (UnkTask_02058364 *)func_02006abc(0)) {
        w = t->work;
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_02058364));
        switch (gGameLanguage) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            lang = gGameLanguage;
            break;
        default:
            lang = 1;
            break;
        }
        w->unk10 = lang;
        w->fn = func_02058218;
        func_020574d8(w);
        func_02058034(w);
    }
}
#pragma thumb reset
