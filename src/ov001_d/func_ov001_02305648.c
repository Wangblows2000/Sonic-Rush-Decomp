#include <nitro/types.h>

typedef struct UnkStruct_02305648_g {
    u8 pad0[0x50];
    s32 top;
    s32 left;
    s32 right;
    s32 bottom;
    u8 pad60[0x31c - 0x60];
    s32 x;
    s32 y;
    s32 m324;
    s32 m328;
    s32 m32C;
    s32 m330;
    s32 speed;
} UnkStruct_02305648_g;

typedef struct UnkStruct_02305648_t {
    u8 pad0[0x18];
    s32 x;
    s32 y;
} UnkStruct_02305648_t;

extern s32 data_ov001_0231f520;
extern s32 data_ov001_0231f524;

UnkStruct_02305648_t *ExtraBoss_GetPlayer(void);
UnkStruct_02305648_g *ExtraBoss_GetWork(void);

void func_ov001_02305648(void) {
    UnkStruct_02305648_t *t;
    UnkStruct_02305648_g *g;
    s32 dl;
    s32 dr;
    s32 du;
    s32 dd;
    s32 sp;

    t = ExtraBoss_GetPlayer();
    g = ExtraBoss_GetWork();
    {
        s32 tx = t->x;
        s32 ty = t->y;
        dl = g->x - g->m328 - tx;
        dr = tx - (g->x + g->m32C);
        du = g->y - g->m324 - ty;
        dd = ty - (g->y + g->m330);
    }
    sp = g->speed;

    if (dl > 0) {
        if (dl > sp)
            dl = sp;
        g->x -= dl;
        if (g->x - 0x80000 < g->left)
            g->x = g->left + 0x80000;
    } else if (dr > 0) {
        if (dr > sp)
            dr = sp;
        g->x += dr;
        if (g->x + 0x80000 > g->right)
            g->x = g->right - 0x80000;
    }
    if (du > 0) {
        if (du > sp)
            du = sp;
        g->y -= du;
        if (g->y - 0x60000 < g->top)
            g->y = g->top + 0x60000;
    } else if (dd > 0) {
        if (dd > sp)
            dd = sp;
        g->y += dd;
        if (g->y + 0x60000 > g->bottom)
            g->y = g->bottom - 0x60000;
    }
    data_ov001_0231f524 = -(g->x - 0x80000);
    data_ov001_0231f520 = -(g->y - 0x60000);
}
