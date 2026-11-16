#include <nitro/types.h>

typedef struct {
    u32 pad0;
    s32 unk4;
} UnkRes_020575b4;

typedef struct {
    u8 pad0[0x2c];
    UnkRes_020575b4 **res;
    u8 pad30[4];
    void *anim;
} UnkObj_020575b4;

typedef struct {
    UnkObj_020575b4 *obj;
    void **anim;
    u8 unk8;
    u8 pad9;
    u8 unka;
    u8 unkb;
} UnkWork_020575b4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_020575b4 *work;
} UnkTask_020575b4;

extern s16 data_0207613c[];

UnkTask_020575b4 *Task_Create(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_0205755c(void);

#pragma thumb on
void func_020575b4(UnkObj_020575b4 *o) {
    UnkTask_020575b4 *t;
    UnkWork_020575b4 *w;
    vu16 z;

    t = Task_Create(func_0205755c, NULL, 7, 0x3efe, 3, 0xc);
    if (t == (UnkTask_020575b4 *)OS_GetArenaLo(0))
        return;
    w = t->work;
    z = 0;
    MIi_CpuClear16(z, w, 0xc);
    w->obj = o;
    w->anim = &o->anim;
    w->unk8 = 0xc;
    w->unka = (*o->res)->unk4;
    w->unkb = data_0207613c[4];
}
#pragma thumb reset
