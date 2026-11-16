#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    void *ptr;
    u16 val;
} UnkWork_02057244;

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkTask_02057244;

typedef struct {
    u8 pad0[0x24];
    UnkTask_02057244 *child;
} UnkMgr_02057244;

extern UnkTask_02057244 *data_022c467c;

UnkTask_02057244 *func_020372d0(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_02057174(void);
void func_02057210(void);

#pragma thumb on
void func_02057244(void *p, u16 v) {
    UnkMgr_02057244 *m;
    UnkTask_02057244 *t;
    UnkWork_02057244 *w;
    vu16 z;

    if (p == NULL || data_022c467c == NULL)
        return;
    m = data_022c467c->work;
    if (m->child != NULL)
        return;
    t = func_020372d0(func_02057174, func_02057210, 0, 0x3efe, 3, 0x14);
    if (t == (UnkTask_02057244 *)func_02006abc(0))
        return;
    w = t->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x14);
    m->child = t;
    w->val = v;
    w->ptr = p;
}
#pragma thumb reset
