#include <nitro/types.h>

typedef struct { u8 pad[4]; s16 x; s16 y; u8 pad2[0x40]; } UnkStruct_022f7880;
extern u8 *data_ov001_0231f084;
void AnimatorSprite_Draw(void *p);
void ExtraAct_Unk_ov001_022f7790(void);

void ExtraAct_State_Unk_ov001_022f7880(void) {
    UnkStruct_022f7880 *o = (UnkStruct_022f7880 *)(data_ov001_0231f084 + 0x648);
    s32 i;
    s16 x;
    o->x = 0x56;
    o->y = 0x7c;
    AnimatorSprite_Draw(o);
    for (i = 0, x = 0x60; i < 6; i++, x += 0xa) {
        s16 c = ((s16 *)(data_ov001_0231f084 + 0xe76))[i];
        if (c != -1) {
            UnkStruct_022f7880 *e = &((UnkStruct_022f7880 *)(data_ov001_0231f084 + 0x18))[c + 0xa];
            e->x = x;
            e->y = 0x7c;
            AnimatorSprite_Draw(e);
        }
    }
    ExtraAct_Unk_ov001_022f7790();
}
