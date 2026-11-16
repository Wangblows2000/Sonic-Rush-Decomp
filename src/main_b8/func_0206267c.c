#include <nitro/types.h>

typedef struct {
    void *def;
    void **anim;
} UnkSlot_0206267c;

typedef struct {
    void (*state)(void *);
    u8 pad4[8];
    s32 unkc;
    u8 pad10[0x70];
    u16 unk80;
    u8 pad82[0x4e];
    UnkSlot_0206267c slot;
} UnkWork_0206267c;

typedef struct {
    u32 pad0;
    s32 unk4;
} UnkRes_0206267c;

typedef struct {
    u8 pad0[0x24];
    s32 unk24;
} UnkTbl_0206267c;

extern UnkTbl_0206267c data_02076e58;

void func_02061738(u16 a);
UnkRes_0206267c **func_02060384(void);
void func_0201c070(void *a, s32 b);
void func_0201bf1c(void *a);
void func_02036754(void *a, s32 b, s32 c);
void func_02062664(void *w);

#pragma thumb on
void func_0206267c(UnkWork_0206267c *w) {
    UnkSlot_0206267c *s;
    UnkRes_0206267c *r;
    s32 v;

    func_02061738(w->unk80);
    s = &w->slot;
    r = *func_02060384();
    if (r != NULL) {
        v = r->unk4;
        if (*s->anim != NULL) {
            func_0201c070(s->anim, 0);
            func_0201bf1c(s->anim);
        }
        func_02036754(s->anim, v, data_02076e58.unk24);
    }
    w->unkc = 1;
    w->state = func_02062664;
}
#pragma thumb reset
