#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x48];
    s32 unk48;
    s32 unk4C;
} UnkStruct_Cam;

typedef struct UnkStruct_022f1df0 {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    u16 unk434;
    u8 pad436[0x57C - 0x436];
    u8 unk57C;
} UnkStruct_022f1df0;

extern UnkStruct_Cam data_ov000_0236a48c;
extern UnkStruct_StageObj *gPlayers;

BOOL func_ov000_022f46f4(UnkStruct_022f1df0 *s);
BOOL func_ov000_022f8f38(void);
BOOL func_ov000_022f8f00(void);
void func_ov000_022f8df8(UnkStruct_022f1df0 *s, s32 *x, s32 *y, s32 c);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022f1d88(UnkStruct_022f1df0 *s);

void func_ov000_022f1df0(UnkStruct_022f1df0 *s) {
    s32 x;
    s32 y;
    UnkStruct_Cam *cam;

    func_ov000_022f46f4(s);
    if (s->unk434 != 0)
        s->unk434--;
    if (func_ov000_022f8f38() && func_ov000_022f8f00()) {
        cam = (UnkStruct_Cam *)(u32)&data_ov000_0236a48c;
        func_ov000_022f8df8(s, &x, &y, 0);
        cam->unk48 = ((x - gPlayers->unk10) << 4) + 0x14000;
        cam->unk4C = 0x32000 - ((y - gPlayers->unk14) << 4);
    }
    if ((s->unk434 & 0x3f) == 0)
        Sound_PlaySeqArc(0, 0, 0xbe);
    if (!(s->unk57C & 2))
        return;
    if (s->unk434 != 0)
        return;
    s->base.unk68 = func_ov000_022f1d88;
    ((void (*)(UnkStruct_022f1df0 *))s->base.unk68)(s);
}
