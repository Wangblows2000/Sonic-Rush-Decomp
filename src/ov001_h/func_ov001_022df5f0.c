#include <nitro/types.h>

typedef struct UnkObj_022df5f0 {
    u8 pad0[0x10c];
    u8 flags[5];
    u8 pad111[0x138 - 0x111];
} UnkObj_022df5f0;

typedef struct UnkStruct_022df5f0_o {
    u8 pad0[4];
    UnkObj_022df5f0 obj;
    u8 unk13C;
    u8 pad13D[0x140 - 0x13d];
    s32 unk140;
    s32 unk144;
    s32 unk148;
    s32 unk14C;
    s32 unk150;
} UnkStruct_022df5f0_o;

typedef struct UnkTask_022df5f0 {
    u8 pad0[0x10];
    UnkStruct_022df5f0_o *work;
} UnkTask_022df5f0;

typedef struct UnkTex_022df5f0 {
    u8 pad0[4];
    void *tex[5];
    u8 pad18[0x768 - 0x18];
    void *res[6];
} UnkTex_022df5f0;

typedef struct UnkStruct_022df5f0 {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x454 - 0x14];
    UnkTex_022df5f0 unk454;
} UnkStruct_022df5f0;

extern u32 gRandSeed;

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

UnkTask_022df5f0 *Task_Create(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
UnkTask_022df5f0 *func_02006abc(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_ov001_022df530(void);
void Model_Init(UnkObj_022df5f0 *o, s32 a);
void Model_SetResource(UnkObj_022df5f0 *o, void *res, s32 a);
void *func_02025e8c(void *res);
void Model_SetAnimation(UnkObj_022df5f0 *o, u32 slot, void *res, u32 idx, void *tex);

void func_ov001_022df5f0(UnkStruct_022df5f0 *w) {
    UnkTask_022df5f0 *task;
    UnkStruct_022df5f0_o *o;
    UnkTex_022df5f0 *t;
    UnkObj_022df5f0 *obj;
    void *tex;
    u8 j;
    u8 k;

    task = Task_Create(func_ov001_022df530, 0, 0, 0x4000, 3, sizeof(UnkStruct_022df5f0_o));
    if (task == func_02006abc(0))
        return;
    o = task->work;
    {
        vu16 zero = 0;
        MIi_CpuClear16(zero, o, sizeof(UnkStruct_022df5f0_o));
    }
    o->unk13C = Rand() % 3;
    o->unk140 = ((Rand() & 0x1f) - 0x10) << 12;
    o->unk144 = 0xfa000;
    o->unk148 = ((Rand() & 0x1f) - 0x10) << 12;
    o->unk150 = ((Rand() & 0xf) + 1) << 12;
    {
        s32 v;
        if (w->unk10 == 2)
            v = -(((Rand() & 7) + 2) << 11);
        else
            v = -(((Rand() & 7) + 5) << 11);
        o->unk14C = v;
    }
    t = &w->unk454;
    k = o->unk13C + 2;
    obj = &o->obj;
    Model_Init(obj, 0);
    Model_SetResource(obj, t->res[k], 0);
    tex = func_02025e8c(t->res[k]);
    for (j = 0; j < 5; j++) {
        if (j == 2 || j == 3) {
            if (tex != NULL) {
                Model_SetAnimation(obj, j, t->tex[j], k, tex);
                obj->flags[j] |= 4;
            }
        } else {
            Model_SetAnimation(obj, j, t->tex[j], k, 0);
            obj->flags[j] |= 4;
        }
    }
}
