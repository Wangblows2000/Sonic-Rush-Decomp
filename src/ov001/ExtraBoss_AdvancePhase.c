#include <nitro/types.h>

typedef struct { u8 pad[0x3c]; s32 v3c; s32 v40; s32 v44; s32 v48; } UnkStruct_0231f52c;
extern UnkStruct_0231f52c *gExtraBossWork;
void Sound_PlaySeq(s32 a, s32 b);
s32 FX_ModS32(s32 a, s32 b);
void ExtraBoss_LoadPlayerModel(s32 a);
void func_ov001_0230582c(void);
void BrightnessLerp_Create(s32 a, s32 b, s32 c, s32 d, s32 e);
void ExtraBoss_StartBattle(void);

void ExtraBoss_AdvancePhase(void) {
    gExtraBossWork->v40++;
    if (gExtraBossWork->v40 == 3) {
        Sound_PlaySeq(0, 0x15);
    }
    gExtraBossWork->v3c = FX_ModS32(gExtraBossWork->v40, 2);
    ExtraBoss_LoadPlayerModel(gExtraBossWork->v3c);
    func_ov001_0230582c();
    BrightnessLerp_Create(0x10, 0x10, 0, 3, 0);
    ExtraBoss_StartBattle();
    if (gExtraBossWork->v3c == 0) {
        gExtraBossWork->v48 = gExtraBossWork->v44;
    }
}
