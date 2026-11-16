#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u16 unk2;
    u8 pad4[3];
    s8 unk7;
    u8 unk8;
} UnkStruct_0230f280_1e0;

typedef struct {
    u8 pad0[0x80];
    void *unk80;
} UnkStruct_0230f280_1bc;

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_0230f280_pos;

typedef struct {
    s16 x;
    s16 y;
} UnkStruct_0230f280_vel;

typedef struct {
    u8 pad0[7];
    u8 unk7;
    u8 pad8[4];
    s32 unkC;
    u8 pad10[0x1b8 - 0x10];
    s32 unk1B8;
    UnkStruct_0230f280_1bc *unk1BC;
    u8 pad1C0[0x1d4 - 0x1c0];
    s16 unk1D4;
    u8 pad1D6[2];
    s16 unk1D8;
    u8 pad1DA[0x1e0 - 0x1da];
    UnkStruct_0230f280_1e0 *unk1E0;
    u8 pad1E4[0x20c - 0x1e4];
    UnkStruct_0230f280_pos pos[16];
    UnkStruct_0230f280_vel vel[16];
} UnkStruct_0230f280;

s16 ObjSpd_UpSet(s32 a, s32 b, s32 c);

void GmkLand_State_Crumble(UnkStruct_0230f280 *p) {
    s32 delay;
    s32 step;
    s32 n;
    s32 k;
    UnkStruct_0230f280_1e0 *r;
    u8 y;
    u8 x;
    u8 cnt;
    s32 t;
    s32 idx;
    s32 v;

    delay = 10;
    p->unkC++;
    step = 8;
    r = p->unk1E0;
    if (r->unk2 == 0x76) {
        if (r->unk7 != 0)
            step = r->unk7;
        if (r->unk8 != 0)
            delay = (s8)r->unk8;
    }
    if (p->unk1BC != NULL && p->unk1BC->unk80 == p)
        p->unk7 = 2;
    n = 3;
    y = n;
    k = 0x3000;
    cnt = 0;
    while (y-- != 0) {
        x = n;
        while (x-- != 0) {
            t = cnt * step - (3 - y) * delay;
            if (t < p->unkC) {
                idx = y + x * 4;
                p->pos[idx].x += p->vel[idx].x;
                p->pos[idx].y += p->vel[idx].y;
                v = p->pos[idx].x;
                if (v < -0x100000)
                    v = -0x100000;
                else if (v > 0x100000)
                    v = 0x100000;
                p->pos[idx].x = v;
                v = p->pos[idx].y;
                if (v < -0x100000)
                    v = -0x100000;
                else if (v > 0x100000)
                    v = 0x100000;
                p->pos[idx].y = v;
                p->vel[idx].y = ObjSpd_UpSet(p->vel[idx].y, (p->unkC - t) * 2 + 8, k);
            }
            cnt++;
        }
    }
    if (p->pos[2].y != 0) {
        p->unk1D8 = 4;
        p->unk1D4 = -3;
    }
    if (p->pos[1].y != 0) {
        p->unk1D8 = 2;
        p->unk1D4 = 0xd;
    }
    if (p->pos[0].y != 0)
        p->unk1B8 = 0;
}
