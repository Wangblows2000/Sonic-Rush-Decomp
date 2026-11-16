#include <nitro/types.h>

typedef struct { u32 flags; s32 v4; u8 pad[0x10]; u16 h18; u16 h1a; u16 h1c; u16 h1e; u16 h20; } UnkStruct_0231f52c;
extern UnkStruct_0231f52c *gExtraBossWork;
void func_ov001_022fea98(void);
void func_ov001_02304e80(void);
void func_ov001_022f7224(s32 a);
void func_ov001_02300db4(void);
void func_ov001_022ffcf8(void);
void ExtraBoss_AdvancePhase(void);

void ExtraBoss_State_CharacterSwitch(void) {
    gExtraBossWork->h20 = 0;
    gExtraBossWork->h1e = gExtraBossWork->h20;
    gExtraBossWork->h1c = gExtraBossWork->h1e;
    gExtraBossWork->h1a = gExtraBossWork->h1c;
    gExtraBossWork->h18 = gExtraBossWork->h1a;
    func_ov001_022fea98();
    func_ov001_02304e80();
    func_ov001_022f7224(gExtraBossWork->v4);
    func_ov001_02300db4();
    if (gExtraBossWork->flags & 0x400) {
        gExtraBossWork->flags &= ~0x400;
        func_ov001_022ffcf8();
        ExtraBoss_AdvancePhase();
    }
}
