#include <nitro/types.h>

typedef struct {
    s32 m[4];
} UnkMtx22_02056b20;

extern s16 data_0206e470[];
extern u8 data_02089264[];

void func_02000cd8(UnkMtx22_02056b20 *m, s32 s, s32 c);
void func_02000d98(UnkMtx22_02056b20 *d, UnkMtx22_02056b20 *s, s32 x, s32 y);
void func_02003d94(void *p, UnkMtx22_02056b20 *m, s32 a, s32 b, s32 x, s32 y);

#pragma thumb on
void func_02056b20(s32 x, s32 y) {
    UnkMtx22_02056b20 m = {0, 0, 0, 0};

    func_02000cd8(&m, data_0206e470[0], data_0206e470[1]);
    func_02000d98(&m, &m, 0x1000, 0x1000);
    func_02003d94(data_02089264, &m, 0, 0, -x, -y);
}
#pragma thumb reset
