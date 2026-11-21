#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s16 bright;
} UnkBright_022d4fa0;

extern UnkBright_022d4fa0 data_0208926c;
extern UnkBright_022d4fa0 data_02087d44;
extern s32 gSaveDataBusy;
extern u32 gSystemFlags;

s32 TitleScreen_Create(void);
void TitleScreen_LoadSound(void);
void TitleScreen_PlayMusic(s32 a);

#pragma thumb on
void TitleScreen_Start(void) {
    s32 r;

    if (data_0208926c.bright != 0x10 && data_0208926c.bright != -0x10) {
        data_0208926c.bright = -0x10;
        data_02087d44.bright = -0x10;
    }
    r = TitleScreen_Create();
    TitleScreen_LoadSound();
    TitleScreen_PlayMusic(r);
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset
