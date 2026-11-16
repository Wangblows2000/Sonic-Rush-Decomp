#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x220];
    void *unk220;
    s32 unk224;
    u8 pad228[0xB7C - 0x228];
    s32 unkB7C;
    u8 padB80[4];
    s32 unkB84;
    u8 padB88[0xBBC - 0xB88];
    s32 unkBBC;
    u8 padBC0[0xBF0 - 0xBC0];
    s32 unkBF0;
} UnkStruct_022ddb44;

UnkStruct_StageObj *func_ov000_022fad3c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void func_ov000_023288d0(s32 a, s32 b, s32 c, s32 d);
void ScreenShake_Start(s32 a);
void func_ov000_022ddab8(void);

void func_ov000_022ddb44(UnkStruct_022ddb44 *s) {
    s->unkBBC -= 0x700;
    if (s->unkBBC > -0x1300)
        return;
    s->unkBBC = -0x1300;
    s->unk220 = func_ov000_022ddab8;
    s->unk224 = 0;
    func_ov000_022fad3c(0xd2, 0, 0, 0, 0, 0, 0, 0, 0);
    func_ov000_023288d0(1, s->unkB7C >> 4, 0x20000, s->unkB84 >> 4);
    s->unkBF0 = 0;
    ScreenShake_Start(0xc);
}
