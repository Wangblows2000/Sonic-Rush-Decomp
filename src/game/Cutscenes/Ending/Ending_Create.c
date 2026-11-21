#include <nitro/types.h>

typedef enum EndingCharacter_022c8480 {
    ENDING_CHAR_SONIC = 0,
    ENDING_CHAR_BLAZE = 1
} EndingCharacter_022c8480;

typedef struct UnkTask_022c8480 {
    u8 pad0[0x10];
    void *work;
} UnkTask_022c8480;

typedef struct UnkStruct_022c8480 {
    s32 unk0;
    u8 pad4[0x5c - 4];
    u8 bg5C[0x19c - 0x5c];
    u8 bg19C[0x41c - 0x19c];
    u8 bg41C[0x49c - 0x41c];
    u8 bg49C[0x4dc - 0x49c];
    u8 bg4DC[0x68c - 0x4dc];
    s32 unk68C;
    u8 pad690[0x6b0 - 0x690];
} UnkStruct_022c8480;

typedef struct UnkStruct_022c4560 {
    s32 unk0;
} UnkStruct_022c4560;

typedef struct UnkStruct_022c4680 {
    u8 pad0[0xc];
    u16 unkC;
    u8 padE[0x64 - 0xe];
    u16 unk64;
} UnkStruct_022c4680;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4680 gSaveData;
extern u32 data_ov001_02315ebc[];

UnkTask_022c8480 *Task_Create(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
void MI_CpuFill8(void *dst, u8 v, u32 size);
void Touch_Enable(void);
void Ending_Main(void);
void Ending_Destructor(void);
void Ending_SubState_SonicStart(void);
void Ending_SubState_BlazeStart(void);
void Ending_SetSubState(UnkStruct_022c8480 *w, void (*fn)(void));
void Ending_InitDisplay(UnkStruct_022c8480 *w);
void Ending_LoadAssets(UnkStruct_022c8480 *w);
void Ending_InitBackgroundsAndSprites(UnkStruct_022c8480 *w);
void Ending_InitDisplayOffsets(UnkStruct_022c8480 *w);
void Background_Draw(void *a);
void Sound_PlaySeq(s32 a, u32 b);

void Ending_Create(void) {
    UnkStruct_022c8480 *w;

    *(vu32 *)0x4000000 &= ~0x1f00;
    *(vu32 *)0x4001000 &= ~0x1f00;
    w = Task_Create(Ending_Main, Ending_Destructor, 0, 0x5000, 0x1173, sizeof(UnkStruct_022c8480))->work;
    MI_CpuFill8(w, 0, sizeof(UnkStruct_022c8480));
    Touch_Enable();
    w->unk0 = gGameState.unk0 == 0 ? ENDING_CHAR_SONIC : ENDING_CHAR_BLAZE;
    switch (w->unk0) {
    case 0:
        if (gSaveData.unkC >= 0x1a)
            w->unk68C = 1;
        break;
    case 1:
        if (gSaveData.unk64 >= 0x1c)
            w->unk68C = 1;
        break;
    }
    switch (w->unk0) {
    case 0:
        Ending_SetSubState(w, Ending_SubState_SonicStart);
        break;
    case 1:
        Ending_SetSubState(w, Ending_SubState_BlazeStart);
        break;
    }
    Ending_InitDisplay(w);
    Ending_LoadAssets(w);
    Ending_InitBackgroundsAndSprites(w);
    Ending_InitDisplayOffsets(w);
    Background_Draw(w->bg5C);
    Background_Draw(w->bg19C);
    Background_Draw(w->bg41C);
    Background_Draw(w->bg49C);
    Background_Draw(w->bg4DC);
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x300;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1200;
    Sound_PlaySeq(0, data_ov001_02315ebc[w->unk0]);
}
