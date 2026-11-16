#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 pad4[0x18];
    u8 zone;
    u8 act;
} UnkState_020554dc;

typedef struct {
    u8 pad0[0xc];
    u16 unkc;
} UnkProg_020554dc;

extern UnkState_020554dc gGameState;
extern UnkProg_020554dc gSaveData;

void func_0204ed24(void);
void func_0205a4ec(void (*f)(void));
BOOL func_0204eb7c(void);
void SceneChange_ToZoneActSelectMode(void);
void SceneChange_ToZoneMap_1_0(void);
void SceneChange_ToZoneMap_1_1(void);

void func_020554dc(void) {
    func_0204ed24();
    func_0205a4ec(NULL);
    if (gGameState.unk0 != 1 && gGameState.zone == 0 && gSaveData.unkc < 2) {
        SceneChange_ToZoneActSelectMode();
        return;
    }
    if (func_0204eb7c() || (gGameState.zone == 7 && gGameState.act == 2))
        SceneChange_ToZoneMap_1_0();
    else
        SceneChange_ToZoneMap_1_1();
}
