#include <nitro/types.h>

typedef struct {
    u8 pad0[0x50];
    u32 flags;
    u8 pad54[0x28];
} UnkSpr_02048898;

typedef struct {
    u32 timer;
    u16 idx;
    u16 flags;
    u8 pad8[4];
    UnkSpr_02048898 spr0;
    UnkSpr_02048898 spr1;
} UnkWork_02048898;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02048898 *work;
} UnkTask_02048898;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1c;
    u8 unk1d;
    u8 pad1e[2];
    s32 unk20;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
} UnkStruct_022b65b4;

typedef struct {
    u16 limit;
    u8 pad2[0x20];
} UnkEntry_02073bd8;

extern UnkTask_02048898 *gTaskCurrent;
extern UnkStruct_022c4560 gGameState;
extern u16 gDemoInputMode;
extern UnkStruct_022b65b4 gTouchState;
extern UnkEntry_02073bd8 data_02073bd8[];

void BrightnessFade_Create(s32 a, s32 b);
BOOL BrightnessFade_IsDone(void);
void func_02053bec(s32 a);
void StageExit_Create(void (*f)(void), s32 a);
void SceneChange_ToTitleScreen(void);
void SceneChange_ToSegaLogo(void);
BOOL Display_IsMainScreenOnBottom(void);
void AnimatorSpriteDS_Draw(UnkSpr_02048898 *s);

void DemoPlay_Main(void) {
    UnkWork_02048898 *w = gTaskCurrent->work;

    if ((w->flags & 4) && gGameState.unk20 != 0) {
        BrightnessFade_Create(3, 0x40);
        w->flags &= ~4;
    }
    if (w->flags & 1) {
        if (BrightnessFade_IsDone()) {
            gDemoInputMode = 5;
            if (gGameState.unk1c >= 9 || gGameState.unk1d >= 2)
                func_02053bec(1);
            if (w->flags & 2)
                StageExit_Create(SceneChange_ToTitleScreen, 3);
            else
                StageExit_Create(SceneChange_ToSegaLogo, 3);
            return;
        }
    } else {
        if (((u16)(((*(vu16 *)0x4000130 | *(vu16 *)0x27fffa8) ^ 0x2fff) & 0x2fff) & 0xc0b) || (gTouchState.unk10 & 0x40)) {
            if (gGameState.unk20 != 0) {
                BrightnessFade_Create(5, 0x400);
                w->flags |= 1;
                w->flags |= 2;
            }
        }
        if (w->timer > data_02073bd8[w->idx].limit) {
            BrightnessFade_Create(5, 0x100);
            w->flags |= 1;
        }
    }
    if (w->timer & 0x40) {
        if (Display_IsMainScreenOnBottom()) {
            w->spr0.flags &= ~2;
            w->spr0.flags |= 1;
            w->spr1.flags |= 2;
            w->spr1.flags &= ~1;
        } else {
            w->spr0.flags &= ~1;
            w->spr0.flags |= 2;
            w->spr1.flags |= 1;
            w->spr1.flags &= ~2;
        }
        AnimatorSpriteDS_Draw(&w->spr0);
        AnimatorSpriteDS_Draw(&w->spr1);
    }
    w->timer++;
}
