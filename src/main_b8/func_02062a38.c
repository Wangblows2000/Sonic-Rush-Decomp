#include <nitro/types.h>

typedef struct {
    s32 a;
    s32 b;
} UnkSub_02062a38;

typedef struct {
    u8 pad0[0x1c];
    UnkSub_02062a38 sub;
} UnkEnt_02062a38;

typedef struct {
    void (*state)(void *);
    u8 pad4[0x64];
    void *unk68;
    u8 pad6c[8];
    s32 unk74;
} UnkWork_02062a38;

UnkEnt_02062a38 *func_02060470(void *p, u8 i);
void func_020603dc(u8 a);
void func_02062100(void);
void func_020617a4(void (*f)(void));
void func_02061eac(void);
void func_02062a10(void *w);

#pragma thumb on
void func_02062a38(UnkWork_02062a38 *w) {
    u8 buf[0x30] = {0};
    UnkEnt_02062a38 *e;
    UnkSub_02062a38 *s;

    e = func_02060470(w->unk68, w->unk74);
    s = &e->sub;
    func_020603dc(s->a);
    func_02062100();
    if (s->b == 2)
        func_020617a4(func_02061eac);
    w->state = func_02062a10;
}
#pragma thumb reset
