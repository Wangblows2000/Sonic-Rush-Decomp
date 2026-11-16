#include <nitro/types.h>

typedef struct { s32 m[9]; } UnkMtx33_020693f0;
typedef struct { s32 x; s32 y; s32 z; } UnkVec_020693f0;
extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void func_02000dfc(UnkMtx33_020693f0 *m);
void func_02000e98(UnkMtx33_020693f0 *m, s32 s, s32 c);
void func_02000eb0(UnkVec_020693f0 *v, UnkMtx33_020693f0 *m, UnkVec_020693f0 *dst);

void Math_RotateVec2S16(s16 *x, s16 *y, s32 angle) {
    s32 a = 0;
    s32 b = 0;
    UnkMtx33_020693f0 m;
    UnkVec_020693f0 v;

    if (x != NULL)
        a = *x;
    if (y != NULL)
        b = *y;
    func_02000dfc(&m);
    func_02000e98(&m, SIN((u16)(angle << 8)), COS((u16)(angle << 8)));
    v.x = a << 4;
    v.y = b << 4;
    v.z = 0;
    func_02000eb0(&v, &m, &v);
    if (x != NULL)
        *x = v.x >> 4;
    if (y != NULL)
        *y = v.y >> 4;
}
