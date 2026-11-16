#include <nitro/types.h>

typedef struct {
    void (*state)(void *);
    u8 pad4[0xc];
    s32 unk10;
    u8 pad14[0x60];
    s32 count;
    s32 limit;
    u8 pad7c[2];
    u16 timer;
    u16 period;
} UnkWork_02062738;

void func_02060e54(s32 a, s32 b);
void func_020610e8(UnkWork_02062738 *w);
void func_02062ae4(void *w);

#pragma thumb on
void func_02062738(UnkWork_02062738 *w) {
    w->timer++;
    if (w->timer < w->period)
        return;
    w->count++;
    if (w->count >= w->limit) {
        func_02060e54(3, 0x100);
        w->unk10 = 0xc;
        w->state = NULL;
    } else {
        w->unk10 = 0xb;
        w->state = func_02062ae4;
    }
    func_020610e8(w);
}
#pragma thumb reset
