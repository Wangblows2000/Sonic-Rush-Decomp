#include <nitro/types.h>
#include <main_b3/w50.h>

struct UnkWork_02047ecc;
typedef void (*UnkObjFn_02047ecc)(struct UnkWork_02047ecc *w, void *obj);
typedef struct UnkWork_02047ecc {
    UnkObjFn_02047ecc obj0;   /* 0x0 */
    u8 pad4[0xc];
    UnkObjFn_02047ecc obj10;  /* 0x10 */
    u8 pad14[0x158];
    UnkObjFn_02047ecc obj16C; /* 0x16c */
    u8 pad170[0x90];
    UnkObjFn_02047ecc obj200; /* 0x200 */
    u8 pad204[0x70];
    UnkObjFn_02047ecc obj274; /* 0x274 */
    u8 pad278[0xb8];
    UnkObjFn_02047ecc obj330; /* 0x330 */
    u8 pad334[0xc];
    void (*state)(struct UnkWork_02047ecc *w); /* 0x340 */
    u32 flags;                /* 0x344 */
} UnkWork_02047ecc;

typedef struct { u8 pad0[0x10]; UnkWork_02047ecc *work; } UnkTask_02047ecc;
extern UnkTask_02047ecc *gTaskCurrent;
typedef struct { u8 pad0[0x10]; s32 unk10; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;

void ClearDemo_Unk_02045d10(UnkWork_02047ecc *w);
void ClearDemo_Unk_02047e68(UnkWork_02047ecc *w);
void ClearDemo_Unk_02047e20(UnkWork_02047ecc *w);
void func_0204528c(UnkWork_02047ecc *w);
void ClearDemo_Unk_02045ec0(UnkWork_02047ecc *w);
void ClearDemo_Unk_02047664(UnkWork_02047ecc *w);
void ClearDemo_DrawModel(UnkWork_02047ecc *w);
void ClearDemo_Unk_0204749c(UnkWork_02047ecc *w);
void ClearDemo_Unk_02045d98(UnkWork_02047ecc *w);
void ClearDemo_Unk_02047458(UnkWork_02047ecc *w);
void Task_Destroy(UnkTask_02047ecc *t);
void NNS_SndStopSoundAll(void);
void ClearDemo_Finish(void);

#pragma thumb on
void ClearDemo_Main(void) {
    UnkWork_02047ecc *w = gTaskCurrent->work;
    u32 flags;

    w->state(w);
    flags = w->flags;
    if (!(flags & 0x80000000)) {
        if ((gPad.trg & 1) && w->state != ClearDemo_Unk_02045d10) {
            if (!(flags & 0x400000) && !(flags & 0x100000))
                ClearDemo_Unk_02047e68(w);
            else if (!(flags & 0x800000) && !(flags & 0x200000))
                ClearDemo_Unk_02047e20(w);
        }
        if (w->flags & 8)
            w->obj0(w, &w->obj0);
        if (w->flags & 0x100)
            w->obj16C(w, &w->obj16C);
        if (w->flags & 0x400)
            w->obj200(w, &w->obj200);
        if (w->flags & 0x20)
            w->obj10(w, &w->obj10);
        if (w->flags & 0x1000)
            w->obj274(w, &w->obj274);
        if (w->flags & 0x20000)
            w->obj330(w, &w->obj330);
        if (w->flags & 0x2000) {
            if (gGameState.unk10 == 2)
                func_0204528c(w);
            else
                ClearDemo_Unk_02045ec0(w);
        }
        if (w->flags & 1)
            ClearDemo_Unk_02047664(w);
        if (w->flags & 2)
            ClearDemo_DrawModel(w);
        if (w->flags & 4)
            ClearDemo_Unk_0204749c(w);
        if (w->flags & 0x40000)
            ClearDemo_Unk_02045d98(w);
        ClearDemo_Unk_02047458(w);
    } else {
        Task_Destroy(gTaskCurrent);
        NNS_SndStopSoundAll();
        ClearDemo_Finish();
    }
}
#pragma thumb reset
