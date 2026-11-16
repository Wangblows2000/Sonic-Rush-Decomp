#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[4];
    s32 unk1c;
    s32 unk20;
    u8 pad24[0xa];
    u16 unk2e;
    u8 pad30[0x24];
    u32 unk54;
    u32 unk58;
    u16 unk5c;
    u8 pad5e[0xa];
    void (*unk68)(void);
} UnkWork_0204da44;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0204da44 *work;
} UnkTask_0204da44;

extern u8 data_0207a1bc[];
extern u8 data_022c45f0[];

UnkTask_0204da44 *Task_Create(void (*fn)(void), void (*fn2)(void), s32 a, s32 b, s32 c, s32 size);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void StageObj_Load2dBAC(UnkWork_0204da44 *w, void *a, u32 b, void *c, s32 d);
void StageObj_AllocSpritePalette(UnkWork_0204da44 *w, s32 a, s32 b);
void StageObj_SetAnimation(UnkWork_0204da44 *w, s32 a);
void StageObj_SetOAMOrder(UnkWork_0204da44 *w, s32 a);
void StageObj_SetOAMPriority(UnkWork_0204da44 *w, s32 a);
void StageObj_Main(void);
void StageObj_Destructor(void);
void GameOver_State_Unk_0204d854(void);

void GameOver_Create(s32 a, s32 b, s32 x, s32 y, u16 d, s16 e) {
    UnkTask_0204da44 *t;
    UnkWork_0204da44 *w;
    vu16 z;

    t = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, 0x134);
    if (t == (UnkTask_0204da44 *)OS_GetArenaLo(0))
        return;
    w = t->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x134);
    StageObj_Load2dBAC(w, data_0207a1bc, 0xffff, data_022c45f0, 0);
    StageObj_AllocSpritePalette(w, 0, 1);
    StageObj_SetAnimation(w, a);
    StageObj_SetOAMOrder(w, 9);
    StageObj_SetOAMPriority(w, 1);
    w->unk5c |= 1;
    w->x = x << 8;
    w->y = y << 8;
    w->unk20 = b;
    w->unk1c = (w->unk20 << 8) + (e << 8);
    w->unk2e = d;
    w->unk58 |= 0x100;
    w->unk58 |= 0x2000;
    w->unk5c |= 0x100;
    w->unk5c |= 0x80;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk68 = GameOver_State_Unk_0204d854;
}
