#include <nitro/types.h>

typedef struct { u8 pad[4]; s16 x; s16 y; u8 pad2[0x3c]; u16 h44; u16 pad3; } UnkStruct_022f8330;
extern u8 *data_ov001_0231f084;
extern u16 data_ov001_02316abc[];
extern u16 data_ov001_02316adc[];
void AnimatorSprite_Draw(void *p);

void ExtraAct_State_Unk_ov001_022f8330(void) {
    s32 i;
    s16 x;
    for (i = 0, x = 0x24; i < 0x10; i++) {
        u16 c = data_ov001_02316adc[i];
        UnkStruct_022f8330 *o = &((UnkStruct_022f8330 *)(data_ov001_0231f084 + 0x18))[c + 0x18];
        if (c != 0xe) {
            o->x = x;
            o->y = 0xb4;
            o->h44 = 1;
            AnimatorSprite_Draw(o);
        }
        x += data_ov001_02316abc[c];
    }
}
