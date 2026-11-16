#include <nitro/types.h>

typedef struct UnkStruct_0230ba9c_t {
    u32 v[8];
} UnkStruct_0230ba9c_t;

typedef struct UnkStruct_0230ba9c {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[8];
    s32 timer;
} UnkStruct_0230ba9c;

extern UnkStruct_0230ba9c *gSpecialStageWork;
extern UnkStruct_0230ba9c_t data_ov001_0231e690;

void func_ov001_023157d8(void);
void func_ov001_0230d708(void);
void func_ov001_0230f438(s32 a);
void func_ov001_023070b0(void);
void func_ov001_0231316c(void);
void func_ov001_02309c14(void);
void func_0200412c(s32 a, s32 b, s32 c, s32 d);
void func_020221fc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void func_02004070(UnkStruct_0230ba9c_t *t);

void func_ov001_0230ba9c(void) {
    UnkStruct_0230ba9c *g = gSpecialStageWork;
    UnkStruct_0230ba9c_t tbl;
    s32 i;
    u32 v;

    func_ov001_023157d8();
    func_ov001_0230d708();
    func_ov001_0230f438(g->unk1C);
    func_ov001_023070b0();
    if (g->unk18 != 0)
        func_ov001_0231316c();
    func_ov001_02309c14();
    if (g->timer < 0x78) {
        tbl = data_ov001_0231e690;
        func_020221fc(0, 0, 3, 0, 0x1f, 0x8000);
        for (i = 0; i < 8; i++) {
            v = (0x7f - g->timer * 0x7f / 120) & 0x7f;
            tbl.v[i] = v | ((v << 24) | (v << 16) | (v << 8));
        }
        func_0200412c(1, 0, 4, 0);
        *(vu32 *)0x4000358 = 0x1f7fff;
        func_02004070(&tbl);
    } else {
        func_0200412c(0, 0, 4, 0);
    }
}
