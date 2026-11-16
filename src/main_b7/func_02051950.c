#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 from;
    s32 to;
    s32 delay;
    s32 duration;
} UnkWork_02051950;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_02051950 *work;
} UnkTask_02051950;

extern UnkTask_02051950 *data_022b4574;
void func_020371c8(UnkTask_02051950 *t);
void func_02051e90(u8 v);

#pragma thumb on
void func_02051950(void) {
    UnkWork_02051950 *w = data_022b4574->work;
    s32 t = w->timer - w->delay;
    if (t < 0) {
        w->timer++;
        return;
    }
    func_02051e90(w->from + (t + 1) * (w->to - w->from) / w->duration);
    w->timer++;
    if (w->timer >= w->delay + w->duration)
        func_020371c8(data_022b4574);
}
#pragma thumb reset
