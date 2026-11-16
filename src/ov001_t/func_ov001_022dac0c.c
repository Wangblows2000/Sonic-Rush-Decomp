#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad[0x28];
} UnkAnim_022dac0c;

typedef struct {
    s32 step;
    s32 unk4;
    u8 pad8[0xb44 - 8];
    UnkAnim_022dac0c anim[6];
} UnkWork_022dac0c;

typedef struct {
    u8 pad[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dac0c *work;
} UnkTask_022dac0c;

extern UnkTask_022dac0c *data_022b4574;

void func_02040d14(UnkAnim_022dac0c *a);
BOOL func_02040cf4(UnkAnim_022dac0c *a);
void func_02050e4c(void);
void func_ov001_022dabe4(void);
void func_ov001_022d9e50(UnkWork_022dac0c *w, s16 x, s16 y);
void func_ov001_022d9e2c(UnkWork_022dac0c *w, s16 x, s16 y);
void func_ov001_022d9d98(UnkWork_022dac0c *w, s32 i, s16 x, s16 y);
void func_ov001_022d9d64(UnkWork_022dac0c *w, s32 i, s16 x, s16 y);

#pragma thumb on
void func_ov001_022dac0c(void) {
    UnkWork_022dac0c *w = data_022b4574->work;
    BOOL done;
    UnkAnim_022dac0c *a;

    func_02040d14(&w->anim[0]);
    func_02040d14(&w->anim[1]);
    func_02040d14(&w->anim[2]);
    func_02040d14(&w->anim[3]);
    func_02040d14(&w->anim[4]);
    func_02040d14(&w->anim[5]);
    done = TRUE;
    if (!func_02040cf4(&w->anim[0])) done = FALSE;
    if (!func_02040cf4(&w->anim[1])) done = FALSE;
    if (!func_02040cf4(&w->anim[2])) done = FALSE;
    if (!func_02040cf4(&w->anim[3])) done = FALSE;
    if (!func_02040cf4(&w->anim[4])) done = FALSE;
    if (!func_02040cf4(&w->anim[5])) done = FALSE;
    a = &w->anim[0];
    func_ov001_022d9e50(w, a->x, a->y);
    a = &w->anim[1];
    func_ov001_022d9e2c(w, a->x, a->y);
    a = &w->anim[2];
    func_ov001_022d9d98(w, 0, a->x, a->y);
    a = &w->anim[3];
    func_ov001_022d9d98(w, 1, a->x, a->y);
    a = &w->anim[4];
    func_ov001_022d9d64(w, 0, a->x, a->y);
    a = &w->anim[5];
    func_ov001_022d9d64(w, 1, a->x, a->y);
    w->step++;
    if (done) {
        w->step = 0;
        w->unk4 = 0;
        func_02050e4c();
        data_022b4574->func = func_ov001_022dabe4;
    }
}
#pragma thumb reset
