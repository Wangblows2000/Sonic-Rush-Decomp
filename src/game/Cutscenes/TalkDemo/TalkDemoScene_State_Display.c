#include <nitro/types.h>

typedef struct {
    u8 pad0[0x28];
    s32 unk28;
} UnkCfg_020628c4;

typedef struct {
    void (*state)(void *);
    u8 pad4[8];
    s32 timer;
    s32 unk10;
    u8 pad14[0x58];
    UnkCfg_020628c4 *cfg;
} UnkWork_020628c4;

u32 TalkDemo_GetFlags(s32 a);
void TalkDemoScene_State_StartBg1Exit(void *w);
void TalkDemoScene_State_WaitInput(void *w);

#pragma thumb on
void TalkDemoScene_State_Display(UnkWork_020628c4 *w) {
    w->timer--;
    if (w->timer <= 0) {
        if (w->cfg->unk28 == 3) {
            w->state = TalkDemoScene_State_StartBg1Exit;
            return;
        }
        w->unk10 = 7;
        w->state = TalkDemoScene_State_WaitInput;
    }
    if (TalkDemo_GetFlags(3) & 0x40)
        w->state = TalkDemoScene_State_StartBg1Exit;
}
#pragma thumb reset
