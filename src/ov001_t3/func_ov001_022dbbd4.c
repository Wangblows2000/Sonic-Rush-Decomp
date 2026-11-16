#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022dbbd4;

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0x10 - 8];
    s32 unk10;
    u8 pad14[0x1c - 0x14];
    s32 unk1c;
    u8 pad20[0xb44 - 0x20];
    UnkAnim_022dbbd4 anims[14];
} UnkWork_022dbbd4;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dbbd4 *work;
} UnkTask_022dbbd4;

extern UnkTask_022dbbd4 *data_022b4574;

void func_02040d14(UnkAnim_022dbbd4 *a);
BOOL func_02040cf4(UnkAnim_022dbbd4 *a);
void func_ov001_022d9eb4(UnkWork_022dbbd4 *w);
void func_ov001_022d9ef4(UnkWork_022dbbd4 *w);
void func_ov001_022db9d8(void);
void func_ov001_022d9e50(UnkWork_022dbbd4 *w, s32 x, s32 y);
void func_ov001_022d9e2c(UnkWork_022dbbd4 *w, s32 x, s32 y);
void func_ov001_022d9b3c(UnkWork_022dbbd4 *w, s32 i, s32 x, s32 y);

#pragma thumb on
void func_ov001_022dbbd4(void) {
    UnkWork_022dbbd4 *w;
    s32 i;
    BOOL done;
    UnkAnim_022dbbd4 *a;

    w = data_022b4574->work;
    func_02040d14(&w->anims[0]);
    func_02040d14(&w->anims[1]);
    for (i = 0; i < 8; i++) {
        func_02040d14(&w->anims[6 + i]);
    }
    a = &w->anims[0];
    func_ov001_022d9e50(w, a->x, a->y);
    a = &w->anims[1];
    func_ov001_022d9e2c(w, a->x, a->y);
    for (i = 0; i < 8; i++) {
        a = &w->anims[6 + i];
        func_ov001_022d9b3c(w, i, a->x, a->y);
    }
    done = TRUE;
    if (!func_02040cf4(&w->anims[0])) done = FALSE;
    if (!func_02040cf4(&w->anims[1])) done = FALSE;
    for (i = 0; i < 8 && done; i++) {
        if (!func_02040cf4(&w->anims[6 + i])) done = FALSE;
    }
    w->timer++;
    if (done) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022d9eb4(w);
        func_ov001_022d9ef4(w);
        w->unk1c = 0;
        w->unk10 = 0;
        data_022b4574->func = func_ov001_022db9d8;
    }
}
#pragma thumb reset
