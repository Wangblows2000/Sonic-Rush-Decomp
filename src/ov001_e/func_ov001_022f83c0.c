#include <nitro/types.h>

typedef struct { u8 pad[4]; s16 x; s16 y; u8 pad2[0x3c]; u16 h44; u16 pad3; } UnkStruct_022f83c0_o;

typedef struct UnkStruct_022f83c0 {
    u8 pad0[0x18];
    UnkStruct_022f83c0_o objs[0x30];
    u8 pad[0xe38 - 0x18 - 0x30 * 0x48];
    u32 flags;
    u8 padE3C[0xe54 - 0xe3c];
    s32 unkE54;
} UnkStruct_022f83c0;

typedef struct UnkStruct_022b4574 {
    u8 pad0[8];
    void (*func)(void);
} UnkStruct_022b4574;

extern UnkStruct_022f83c0 *data_ov001_0231f084;
extern UnkStruct_022b4574 *gTaskCurrent;
extern u16 data_ov001_02316abc[];
extern u16 data_ov001_02316adc[];
void AnimatorSprite_Draw(void *p);
void func_ov001_022f8330(void);

void func_ov001_022f83c0(void) {
    s32 i;
    s16 x;

    if (data_ov001_0231f084->flags & 3) {
        gTaskCurrent->func = func_ov001_022f8330;
        func_ov001_022f8330();
        data_ov001_0231f084->flags &= ~0x1000;
        data_ov001_0231f084->flags |= 0x2000;
        return;
    }
    data_ov001_0231f084->unkE54 += 0x2000;
    for (i = 0, x = 0x24; i < 0x10; i++) {
        u16 c = data_ov001_02316adc[i];
        UnkStruct_022f83c0_o *o = &data_ov001_0231f084->objs[c + 0x18];
        s32 v = (data_ov001_0231f084->unkE54 << 4) >> 16;
        if (x < v)
            v = x;
        if (c != 0xe) {
            o->x = v;
            o->y = 0xb4;
            o->h44 = 1;
            AnimatorSprite_Draw(o);
        }
        x += data_ov001_02316abc[c];
    }
    if (x < data_ov001_0231f084->unkE54 >> 12) {
        gTaskCurrent->func = func_ov001_022f8330;
        data_ov001_0231f084->flags |= 0x1000;
    }
}
