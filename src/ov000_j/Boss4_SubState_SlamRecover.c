#include <nitro/types.h>

typedef struct {
    u8 pad0[0x220];
    void *unk220;
    s32 unk224;
    u8 pad228[0xBBC - 0x228];
    s32 unkBBC;
} UnkStruct_022dd9e8;

void Boss4_SubState_SpinUp(void);
void Boss4_SetCameraDefaultDistance(void);
BOOL Runtime_Unk_0202e5e4(float a, float b);

void Boss4_SubState_SlamRecover(UnkStruct_022dd9e8 *s) {
    float t;

    t = (float)s->unk224 / 45.0f;
    if (Runtime_Unk_0202e5e4(t, 1.0f))
        t = 1.0f;
    t = 1.0f - t;
    t = 1.0f - t * (t * t);
    s->unkBBC = -0x1300;
    s->unkBBC = s->unkBBC + 3328.0f * t;
    if (s->unk224 > 0x2d) {
        s->unk220 = Boss4_SubState_SpinUp;
        s->unk224 = 0;
        Boss4_SetCameraDefaultDistance();
    }
}
