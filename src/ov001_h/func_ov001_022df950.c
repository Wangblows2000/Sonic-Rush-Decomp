#pragma opt_dead_assignments off
#include <nitro/types.h>

typedef struct UnkStruct_022df950_p {
    u16 a;
    u16 b;
} UnkStruct_022df950_p;

typedef struct UnkStruct_022df950_q {
    UnkStruct_022df950_p v[2];
} UnkStruct_022df950_q;

typedef struct UnkStruct_022df950 {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022df950;

extern u16 data_ov001_023161f8[];
extern s16 data_ov001_0231620c[][3];
extern u16 data_ov001_023161fe[];
extern s16 data_ov001_0231621e[][3];
extern UnkStruct_022df950_q data_ov001_023161e4;

void func_02022270(s32 a, u16 b);
void func_02022284(s32 i, s32 r, s32 g, s32 b);
void func_02004084(u16 *p);

void func_ov001_022df950(UnkStruct_022df950 *s) {
    u16 *col;
    s16 (*tab)[3];
    u8 i;

    if (s->unk10 != 2) {
        col = data_ov001_023161f8;
        tab = data_ov001_0231620c;
    } else {
        col = data_ov001_023161fe;
        tab = data_ov001_0231621e;
    }
    func_02022270(0, col[0]);
    func_02022270(1, col[1]);
    func_02022270(2, col[2]);
    for (i = 0; i < 3; i++) {
        s16 *row = tab[i];
        func_02022284(i, row[0], row[1], row[2]);
    }
    {
        u16 edge[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        UnkStruct_022df950_p *d = (UnkStruct_022df950_p *)&edge[4];
        UnkStruct_022df950_p *src = data_ov001_023161e4.v;
        d[0] = src[0];
        d[1] = src[1];
        func_02004084(edge);
    }
}
