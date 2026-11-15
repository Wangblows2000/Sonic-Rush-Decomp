#include <nitro/types.h>
#include <main_b3/w50.h>

typedef struct { s32 x; s32 y; } UnkVec_0206151c;
typedef struct { s16 v[6]; u8 b; u8 pad; } UnkEnt_02077654;
extern UnkEnt_02077654 data_02077654[];
extern u8 data_0207cc60[];
extern u8 data_0207cc78[];
extern u32 data_0208fe70;

typedef struct {
    u8 pad0[0x10];
    UnkSpr_W50 spr[2];     /* 0x10 */
    void *res[2];          /* 0xa0 */
    u16 pal[0x10];         /* 0xa8 */
    UnkVec_0206151c a[6];  /* 0xc8 */
    UnkVec_0206151c b[6];  /* 0xf8 */
    UnkVec_0206151c c[6];  /* 0x128 */
    u8 d[6];               /* 0x158 */
} UnkWork_0206151c;

void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void *func_02033fbc(void *a, u32 b);
void *func_02039c18(u32 a, u32 b);
void func_0203363c(UnkSpr_W50 *s, void *res, u32 idx, u32 c, u32 d, u32 e, void *f, u32 g, u32 h);
void func_0203344c(UnkSpr_W50 *s, u32 a, u32 b);

#pragma thumb on
static inline u16 Rand_0206151c(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_0206151c(UnkWork_0206151c *w) {
    u8 i;
    UnkSpr_W50 *spr;
    UnkEnt_02077654 *e;

    MIi_CpuCopy16((void *)0x050003e0, w->pal, sizeof(w->pal));
    w->res[0] = func_02033fbc(data_0207cc60, 0);
    w->res[1] = func_02033fbc(data_0207cc78, 0);
    for (i = 0; i < 2; i++) {
        spr = &w->spr[i];
        func_0203363c(spr, w->res[i], 0, 0x4404, 0, 0, func_02039c18(0, 8), 0, 0x05000200);
        spr->unk44 = 0xf;
        spr->unk46 = 0;
        func_0203344c(spr, 0, 0);
        spr->unk2C |= 0x10;
    }
    for (i = 0; i < 6; i++) {
        w->a[i].x = (data_02077654[i].v[0] << 8) + (0x300 - (Rand_0206151c() & 0x600));
        e = &data_02077654[i];
        w->a[i].y = (e->v[1] << 8) + (0x300 - (Rand_0206151c() & 0x600));
        w->b[i].x = (e->v[2] << 8) + (0x300 - (Rand_0206151c() & 0x600));
        w->b[i].y = (e->v[3] << 8) + (0x300 - (Rand_0206151c() & 0x600));
        w->c[i].x = e->v[4] + (7 - (Rand_0206151c() & 0xe));
        w->c[i].y = e->v[5] + (7 - (Rand_0206151c() & 0xe));
        w->d[i] = e->b;
    }
}
#pragma thumb reset
