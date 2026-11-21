#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 flags;
    u8 zone;
    u8 act;
} UnkState_0204d6d0;

typedef struct {
    u8 pad0[0x10];
    u32 *work;
} UnkTask_0204d6d0;

extern UnkState_0204d6d0 gGameState;

void Stage_InitDisplay(BOOL f);
void Stage_InitDisplayBoss(BOOL f);
void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void StageStart_State_Begin(void);

void StageStart_Create(u32 v) {
    BOOL f;

    if ((gGameState.flags & 0x40) == 0)
        gGameState.flags &= ~2;
    f = (gGameState.flags & 0x10) ? TRUE : FALSE;
    if (gGameState.zone < 9 && gGameState.act < 2)
        Stage_InitDisplay(f);
    else
        Stage_InitDisplayBoss(f);
    *((UnkTask_0204d6d0 *)Task_Create(StageStart_State_Begin, 0, 0, 0, 0, 0xc))->work = v;
}
