#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x9F0 - 0x20C];
    u8 unk9F0[0xF54 - 0x9F0];
    s32 unkF54;
    s32 unkF58;
    s32 unkF5C;
    s16 unkF60;
    u8 padF62[2];
    s16 unkF64;
} UnkStruct_Boss;

extern UnkStruct_StageObj *gPlayers;
extern UnkStruct_Boss *data_ov000_02369b4c;

void func_02021c38(void *p, void (*fn)(void), s32 a, s32 b, s32 c);
void func_ov000_022e60a4(void);
void func_ov000_022e5078(void);
void func_ov000_022e51a4(void);

void func_ov000_022e5310(void) {
    UnkStruct_Boss *w = data_ov000_02369b4c;
    s32 x = gPlayers->unk10;

    func_02021c38(w->unk9F0, func_ov000_022e60a4, 0, 6, 3);
    if (x < w->base.unk10 + 0x3200 && x > w->base.unk10 - 0x3200) {
        w->base.unk68 = func_ov000_022e5078;
        if (w->unkF54 >= 0) {
            if (w->unkF58 > 0) {
                w->unkF54 = -w->unkF54;
                w->unkF60 = -w->unkF60;
            }
        } else {
            if (w->unkF58 < 0) {
                w->unkF54 = -w->unkF54;
                w->unkF60 = -w->unkF60;
            }
        }
        w->unkF64 = 0;
        w->unkF5C = 0;
        return;
    }
    w->base.unk68 = func_ov000_022e51a4;
}
