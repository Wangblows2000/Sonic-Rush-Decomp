#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[0xc];
    u16 unk10;
    u8 pad12[2];
    s32 unk14;
    u8 *unk18;
    u32 *unk1c;
} UnkWork_02068094;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02068094 *work;
} UnkTask_02068094;

extern u16 gDemoInputMode;
extern u32 gRandSeed;

UnkTask_02068094 *Task_Create(void (*fn)(void), void (*fn2)(void), s32 a, s32 b, s32 c, s32 size);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
u32 *File_Load(void *p, s32 a);
void DemoInput_Main(void);
void DemoInput_Destructor(void);

void DemoInput_Create(s32 a, void *res) {
    UnkTask_02068094 *t;
    UnkWork_02068094 *w;
    u16 prio;
    vu16 z;

    prio = 0x10;
    if (res == NULL)
        prio |= 0x20;
    t = Task_Create(DemoInput_Main, DemoInput_Destructor, prio, 1, 0, 0x20);
    if (t == (UnkTask_02068094 *)OS_GetArenaLo(0))
        return;
    w = t->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x20);
    w->unk14 = a;
    if (res == NULL)
        return;
    w->flags |= 2;
    gDemoInputMode = 2;
    w->unk1c = File_Load(res, 0);
    w->flags |= 4;
    gRandSeed = *w->unk1c;
    w->unk18 = (u8 *)w->unk1c + 4;
    w->unk10 = 8;
}
