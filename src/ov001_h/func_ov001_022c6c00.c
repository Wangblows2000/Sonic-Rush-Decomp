#include <nitro/types.h>

typedef struct UnkMtx22_022c6c00 {
    s32 m[4];
} UnkMtx22_022c6c00;

typedef struct UnkStruct_022c6c00_s {
    u8 pad0[0x18];
    u16 unk18;
    u16 unk1A;
} UnkStruct_022c6c00_s;

typedef struct UnkStruct_022c6c00 {
    u8 pad0[0xc];
    s32 unkC;
    u8 pad10[0x50 - 0x10];
    UnkStruct_022c6c00_s *unk50;
} UnkStruct_022c6c00;

extern s16 data_0206e470[];
extern u8 data_02089264[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

s32 func_0200225c(s32 n, s32 d);
void func_02000cd8(UnkMtx22_022c6c00 *m, s32 s, s32 c);
void func_02000d98(UnkMtx22_022c6c00 *d, UnkMtx22_022c6c00 *s, s32 x, s32 y);
void func_02003d94(void *p, UnkMtx22_022c6c00 *m, s32 x, s32 y, s32 a, s32 b);

void func_ov001_022c6c00(UnkStruct_022c6c00 *w) {
    UnkMtx22_022c6c00 mtx;
    float t = (float)w->unkC / 335.0f;
    s32 scale;
    u16 angle;

    scale = func_0200225c(0x64000, 0x64000 - (s32)(405504.0f * (1.0f - t * t)));
    angle = 0x7555 + (w->unkC << 9);
    func_02000cd8(&mtx, SIN(angle), COS(angle));
    func_02000d98(&mtx, &mtx, scale, scale);
    func_02003d94(data_02089264, &mtx, (w->unk50->unk18 << 3) / 2, (w->unk50->unk1A << 3) / 2, 0, 0);
}
