#include <nitro/types.h>

typedef struct { void *player; u16 timer; u8 pad6[2]; } UnkWork_020573c0;
typedef struct { u8 pad0[0x10]; UnkWork_020573c0 *work; } UnkTask_020573c0;
typedef struct { u8 pad0[0x30]; void *snd0; void *snd1; } UnkPlayer_020573c0;
extern s16 data_022c4678;
extern void *data_02250464;

UnkTask_020573c0 *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_0205736c(void);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void func_020572b4(u32 n, u32 flags);
void func_0201c070(void *p, s32 a);

#pragma thumb on
void func_020573c0(UnkPlayer_020573c0 *player) {
    UnkTask_020573c0 *t = func_020372d0(func_0205736c, NULL, 0, 0x3efd, 3, sizeof(UnkWork_020573c0));
    UnkWork_020573c0 *w;
    vu16 zero;

    if (t != (UnkTask_020573c0 *)func_02006abc(0)) {
        w = t->work;
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_020573c0));
        w->player = player;
        w->timer = 0x3c;
        func_020572b4(0x3c, 7);
        if (data_022c4678 == 0x2a) {
            if (data_02250464 != NULL)
                func_0201c070(&data_02250464, 0x3c);
        } else if (player->snd0 != NULL) {
            func_0201c070(&player->snd0, 0x3c);
        }
        if (player->snd1 != NULL)
            func_0201c070(&player->snd1, 0x3c);
    }
}
#pragma thumb reset
