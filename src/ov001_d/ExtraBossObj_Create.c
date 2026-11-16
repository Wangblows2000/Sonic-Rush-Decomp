#include <nitro/types.h>

typedef struct UnkStruct_023014f4_task {
    u8 pad0[0x10];
    void *work;
} UnkStruct_023014f4_task;

typedef struct UnkStruct_023014f4_w {
    UnkStruct_023014f4_task *task;
    s32 kind;
    u16 idx;
    u16 slot;
    u8 padC[0x44 - 0xc];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
} UnkStruct_023014f4_w;

typedef struct UnkStruct_023014f4_g {
    u8 pad0[4];
    UnkStruct_023014f4_w *a4[1];
    UnkStruct_023014f4_w *a8[5];
    UnkStruct_023014f4_w *a1C[10];
    UnkStruct_023014f4_w *a44[0x1e];
    UnkStruct_023014f4_w *all[0x2e];
} UnkStruct_023014f4_g;

extern UnkStruct_023014f4_g *gExtraBossPlayerManager;

UnkStruct_023014f4_task *Task_Create(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
void ExtraBossObj_Main(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void Task_Destroy(UnkStruct_023014f4_task *t);

UnkStruct_023014f4_w *ExtraBossObj_Create(s32 kind, u16 id) {
    vu16 zero;
    UnkStruct_023014f4_task *t;
    UnkStruct_023014f4_w *w;
    UnkStruct_023014f4_w **slot;
    s32 n;
    s32 i;

    t = Task_Create(ExtraBossObj_Main, 0, 0, id, 1, 0x1e0);
    w = t->work;
    zero = 0;
    MIi_CpuClear16(zero, w, 0x1e0);
    switch (kind) {
    case 0:
        n = 1;
        slot = gExtraBossPlayerManager->a4;
        break;
    case 1:
        n = 0x1e;
        slot = gExtraBossPlayerManager->a44;
        break;
    case 2:
        n = 5;
        slot = gExtraBossPlayerManager->a8;
        break;
    case 3:
        n = 10;
        slot = gExtraBossPlayerManager->a1C;
        break;
    }
    for (i = 0; i < n; i++, slot++) {
        if (*slot == NULL)
            break;
    }
    if (i >= n) {
        Task_Destroy(t);
        return NULL;
    }
    *slot = w;
    w->slot = i;
    for (i = 0; i < 0x2e; i++) {
        if (gExtraBossPlayerManager->all[i] == NULL)
            break;
    }
    gExtraBossPlayerManager->all[i] = w;
    w->task = t;
    w->idx = i;
    w->kind = kind;
    w->unk4C = 0x1000;
    w->unk48 = w->unk4C;
    w->unk44 = w->unk48;
    return w;
}
