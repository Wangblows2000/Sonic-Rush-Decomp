#include <nitro/types.h>

typedef struct {
    u8 pad0[0xBB8];
    s32 unkBB8;
    u8 padBBC[4];
    s32 unkBC0;
    u8 padBC4[4];
    s32 unkBC8;
} UnkStruct_022de288;

float Boss4_GetHealthSpinScale(UnkStruct_022de288 *s);
void Boss4_StepTiltToward(UnkStruct_022de288 *s, s32 a);

void Boss4_SubState_Spin(UnkStruct_022de288 *s) {
    s32 lim = 96.0f * Boss4_GetHealthSpinScale(s);

    if (s->unkBC0 > lim) {
        s->unkBC0 -= 8;
        if (s->unkBC0 < lim)
            s->unkBC0 = lim;
    } else {
        s->unkBC0 += 8;
        if (s->unkBC0 > lim)
            s->unkBC0 = lim;
    }
    s->unkBB8 += s->unkBC0;
    Boss4_StepTiltToward(s, s->unkBC8);
}
