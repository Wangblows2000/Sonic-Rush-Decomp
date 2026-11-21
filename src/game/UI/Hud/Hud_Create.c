#include <nitro/types.h>
#include "game/Graphics/dual.h"

typedef struct {
    u8 pad0[0xa];
    u16 vram;
    u8 padC[8];
} UnkEnt_023376c4;

typedef struct {
    u8 pad0[0x18];
    u32 tblOfs;
} UnkRes_023376c4;

typedef struct {
    u32 unk0;
    UnkDual d[5];
    u8 pad270[8];
    s32 scale[4];
} UnkSub_023376c4;

typedef struct {
    u8 pad0[8];
    UnkRes_023376c4 *res;
    u32 vram[4];
    u8 pad1c[4];
    UnkDual d20;
    UnkDual d9c;
    UnkDual d118[3];
    u8 pad28c[4];
    s32 unk290;
    u8 pad294[8];
    UnkDual d29c;
    UnkDual d318;
    UnkDual d394[2];
    UnkDual d48c[2];
    UnkDual d584;
    UnkDual d600;
    u8 pad67c[4];
    s32 unk680;
    u8 pad684[4];
    UnkSub_023376c4 s688;
    u8 pad910[0x1c];
    UnkDual d92c;
    UnkDual d9a8;
    UnkDual da24;
    s32 unkaa0;
    u8 padaa4[4];
} UnkWork_023376c4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_023376c4 *work;
} UnkTask_023376c4;

typedef struct {
    s32 unk0;
    u8 pad4[0x18];
    u8 unk1c;
    u8 unk1d;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;
extern void *gActComArchive;
extern UnkTask_023376c4 *gHudTask;
extern u8 data_ov000_023687e4[];
extern u8 data_ov000_02364d62[];

void *Task_Create(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
void MIi_CpuClear16(u16 data, void *dst, u32 size);
void NNS_FndMountArchive(void *f, void *name, void *arc);
void *NNS_FndGetArchiveFileByIndex(void *f, s32 idx);
void NNS_FndUnmountArchive(void *f);
u32 ObjVram_Alloc(s32 a, u16 n);
void AnimatorSpriteDS_Init(UnkDual *d, void *res, u16 idx, u32 mask, u32 flags, u32 a0, u32 a1, u32 b0, u32 b1, u32 c0, u32 c1, u32 e0, u32 e1);
void AnimatorSpriteDS_ProcessAnimation(UnkDual *d, s32 a, s32 b);
void AnimatorSpriteDS_SetAnimation(UnkDual *d, u16 idx);
void PaletteTransfer_QueueCopy(void *p, s32 a, s32 b, u32 c);
void ObjDraw_LoadPaletteRow(void *res, u16 id, u8 a, s32 b);
void Hud_Main(void);
void Hud_Destructor(void);
void Hud_SetOamPriority0(void);

#define IS_MAIN() (gGameState.unk1c < 9 && gGameState.unk1d < 2)

static inline void InitDigits(UnkWork_023376c4 *work, UnkDual *d, u16 idx, u32 *vram) {
    u32 m;
    u32 i;
    if (!IS_MAIN())
        m = 2;
    else
        m = 0;
    AnimatorSpriteDS_Init(d, work->res, idx, m, 0x30, 0, vram[0], 0, 0x5000200, 0, vram[1], 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;
    for (i = 0; i < 10; i++) {
        AnimatorSpriteDS_ProcessAnimation(d, 0, 0);
        AnimatorSpriteDS_SetAnimation(d, d->sub.unk8 + 1);
        d->unk5c[0] += 0x80;
        d->unk5c[1] += 0x80;
    }
}

void Hud_Create(void) {
    u32 f[0x5c / 4];
    UnkDual tmp;
    UnkSub_023376c4 *sub;
    UnkDual *d;
    UnkDual *dd;
    u32 mask;
    UnkWork_023376c4 *work;
    UnkDual *d3;
    u32 i;
    s32 idx;
    vu16 zero;

    if (!IS_MAIN())
        mask = 2;
    else
        mask = 0;

    gHudTask = Task_Create(Hud_Main, Hud_Destructor, 0, 0x3800, 3, sizeof(UnkWork_023376c4));
    work = gHudTask->work;
    zero = 0;
    MIi_CpuClear16(zero, work, sizeof(UnkWork_023376c4));
    work->unk290 = -1;
    work->unk680 = -1;
    work->unkaa0 = -1;

    NNS_FndMountArchive(f, data_ov000_023687e4, gActComArchive);
    work->res = NNS_FndGetArchiveFileByIndex(f, 0);
    NNS_FndUnmountArchive(f);

    work->vram[0] = ObjVram_Alloc(0, 0x14);
    if (IS_MAIN())
        work->vram[1] = ObjVram_Alloc(1, 0x14);
    d = &tmp;
    InitDigits(work, d, 0, &work->vram[0]);

    work->vram[2] = ObjVram_Alloc(0, 0x14);
    if (IS_MAIN())
        work->vram[3] = ObjVram_Alloc(1, 0x14);
    dd = &tmp;
    InitDigits(work, dd, 10, &work->vram[2]);

    d = &work->d20;
    AnimatorSpriteDS_Init(d, work->res, 0x21, mask, 0x4010, 0, ObjVram_Alloc(0, 8), 0, 0x5000200, 0, !IS_MAIN() ? 0 : ObjVram_Alloc(1, 8), 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;
    AnimatorSpriteDS_ProcessAnimation(d, 0, 0);
    d->sub.flags |= 8;

    d = &work->d9c;
    AnimatorSpriteDS_Init(d, work->res, 0x22, mask, 0x4010, 0, ObjVram_Alloc(0, 0x10), 0, 0x5000200, 0, !IS_MAIN() ? 0 : ObjVram_Alloc(1, 0x10), 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;
    AnimatorSpriteDS_ProcessAnimation(d, 0, 0);
    d->sub.flags |= 8;

    d3 = work->d118;
    for (i = 0; i < 3; i++) {
        AnimatorSpriteDS_Init(d3, work->res, 0x22, mask, 0x4018, 0, 0, 0, 0x5000200, 0, 0, 0, 0x5000600);
        d3->unk74[0] = 1;
        d3->unk74[1] = 1;
        d3++;
    }

    d = &work->d29c;
    AnimatorSpriteDS_Init(d, work->res, 0x14, mask, 0x4010, 0, ObjVram_Alloc(0, 2), 0, 0x5000200, 0, !IS_MAIN() ? 0 : ObjVram_Alloc(1, 2), 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;
    AnimatorSpriteDS_ProcessAnimation(d, 0, 0);

    d = &work->d318;
    AnimatorSpriteDS_Init(d, work->res, 0, mask, 0x4018, 0, 0, 0, 0x5000200, 0, 0, 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;

    for (i = 0; i < 2; i++) {
        d = &work->d394[i];
        AnimatorSpriteDS_Init(d, work->res, 0, mask, 0x4018, 0, 0, 0, 0x5000200, 0, 0, 0, 0x5000600);
        d->unk74[0] = 1;
        d->unk74[1] = 1;
    }
    for (i = 0; i < 2; i++) {
        d = &work->d48c[i];
        AnimatorSpriteDS_Init(d, work->res, 0, mask, 0x4018, 0, 0, 0, 0x5000200, 0, 0, 0, 0x5000600);
        d->unk74[0] = 1;
        d->unk74[1] = 1;
    }

    d = &work->d584;
    AnimatorSpriteDS_Init(d, work->res, 0x15, mask, 0x4010, 0, ObjVram_Alloc(0, 2), 0, 0x5000200, 0, !IS_MAIN() ? 0 : ObjVram_Alloc(1, 2), 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;
    AnimatorSpriteDS_ProcessAnimation(d, 0, 0);

    d = &work->d600;
    AnimatorSpriteDS_Init(d, work->res, 0x15, mask, 0x4010, 0, ObjVram_Alloc(0, 2), 0, 0x5000200, 0, !IS_MAIN() ? 0 : ObjVram_Alloc(1, 2), 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;
    AnimatorSpriteDS_ProcessAnimation(d, 0, 0);

    sub = &work->s688;
    for (i = 0; i < 4; i++)
        sub->scale[i] = 0x1000;

    d = &sub->d[0];
    AnimatorSpriteDS_Init(d, work->res, 0x1d, mask, 0x4210, 0, ObjVram_Alloc(0, 2), 0, 0x5000200, 0, !IS_MAIN() ? 0 : ObjVram_Alloc(1, 2), 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;
    AnimatorSpriteDS_ProcessAnimation(d, 0, 0);
    d->sub.flags |= 8;

    d = &sub->d[1];
    AnimatorSpriteDS_Init(d, work->res, 0x1e, mask, 0x4210, 0, ObjVram_Alloc(0, 2), 0, 0x5000200, 0, !IS_MAIN() ? 0 : ObjVram_Alloc(1, 2), 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;
    AnimatorSpriteDS_ProcessAnimation(d, 0, 0);
    d->sub.flags |= 8;

    d = &sub->d[2];
    AnimatorSpriteDS_Init(d, work->res, 0x1b, mask, 0x4010, 0, ObjVram_Alloc(0, 8), 0, 0x5000200, 0, !IS_MAIN() ? 0 : ObjVram_Alloc(1, 8), 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;
    AnimatorSpriteDS_ProcessAnimation(d, 0, 0);
    d->sub.flags |= 8;

    d = &sub->d[3];
    AnimatorSpriteDS_Init(d, work->res, 0x1c, mask, 0x4010, 0, ObjVram_Alloc(0, 8), 0, 0x5000200, 0, !IS_MAIN() ? 0 : ObjVram_Alloc(1, 8), 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;
    AnimatorSpriteDS_ProcessAnimation(d, 0, 0);
    d->sub.flags |= 8;

    d = &sub->d[4];
    AnimatorSpriteDS_Init(d, work->res, 0x23, mask, 0x4010, 0, ObjVram_Alloc(0, 8), 0, 0x5000200, 0, !IS_MAIN() ? 0 : ObjVram_Alloc(1, 8), 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;
    AnimatorSpriteDS_ProcessAnimation(d, 0, 0);

    PaletteTransfer_QueueCopy(data_ov000_02364d62, 1, 0, 0x5000222);
    PaletteTransfer_QueueCopy(data_ov000_02364d62, 1, 0, 0x5000622);

    if (gGameState.unk0 == 0)
        idx = 0x1f;
    else
        idx = 0x20;
    d = &work->d92c;
    AnimatorSpriteDS_Init(d, work->res, idx, mask, 0x4010, 0,
                  ObjVram_Alloc(0, ((UnkEnt_023376c4 *)((u8 *)work->res + work->res->tblOfs + 0x14))[(u16)idx].vram), 0, 0x5000200, 0,
                  !IS_MAIN() ? 0 : ObjVram_Alloc(1, ((UnkEnt_023376c4 *)((u8 *)work->res + work->res->tblOfs + 0x14))[(u16)idx].vram), 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;
    AnimatorSpriteDS_ProcessAnimation(d, 0, 0);
    d->sub.flags |= 8;

    d = &work->d9a8;
    AnimatorSpriteDS_Init(d, work->res, 0x19, mask, 0x4010, 0, ObjVram_Alloc(0, 2), 0, 0x5000200, 0, !IS_MAIN() ? 0 : ObjVram_Alloc(1, 2), 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;
    AnimatorSpriteDS_ProcessAnimation(d, 0, 0);

    d = &work->da24;
    AnimatorSpriteDS_Init(d, work->res, 0, mask, 0x4018, 0, 0, 0, 0x5000200, 0, 0, 0, 0x5000600);
    d->unk74[0] = 1;
    d->unk74[1] = 1;

    ObjDraw_LoadPaletteRow(work->res, 0x21, 1, 0);
    ObjDraw_LoadPaletteRow(work->res, 0x21, 1, 1);
    Hud_SetOamPriority0();
}
