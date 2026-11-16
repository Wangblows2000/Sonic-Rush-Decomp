#include <nitro/types.h>

typedef struct {
    s16 idx;
} UnkState_02055e48;

typedef struct {
    u8 pad0[9];
    u8 act;
} UnkObj_02055e48;

extern UnkState_02055e48 gPlayerDemoId;

u16 *func_02056e94(u8 zone, u8 act);
s16 (*func_02056f10(u8 zone, u8 act))[3];
void func_02022270(s32 a, u16 b);
void func_02022284(s32 a, s32 x, s32 y, s32 z);

#pragma thumb on
void func_02055e48(UnkObj_02055e48 *o) {
    u8 zone = gPlayerDemoId.idx;
    u8 act = o->act;
    u8 i;
    s16 (*p)[3];
    u16 *c;

    c = func_02056e94(zone, act);
    func_02022270(0, c[0]);
    func_02022270(1, c[1]);
    func_02022270(2, c[2]);
    p = func_02056f10(zone, act);
    for (i = 0; i < 3; i++)
        func_02022284(i, p[i][0], p[i][1], p[i][2]);
}
#pragma thumb reset
