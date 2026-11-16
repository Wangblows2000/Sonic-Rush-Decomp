#include <nitro/types.h>

typedef struct UnkStruct_02308b74 {
    u8 pad0[0xb48];
    u16 src[16];
    u16 dst[16];
    u16 out[16];
    u8 padBA8[0xbac - 0xba8];
    s32 timer;
} UnkStruct_02308b74;

extern UnkStruct_02308b74 *data_ov001_0231f5bc;

void func_ov001_02308b74(void) {
    UnkStruct_02308b74 *g;
    s32 i;
    float f;
    s32 a;
    s32 sr;
    s32 sg;
    s32 sb;
    s32 c;
    s32 db;
    s32 dr;
    s32 dg;

    g = data_ov001_0231f5bc;

    if (g->timer < 0x3c) {
        f = 0.0f;
    } else if (g->timer <= 0x46) {
        f = (g->timer - 0x3c) / 10.0f;
    } else if (g->timer < 0x82) {
        f = 1.0f;
    } else if (g->timer <= 0x8c) {
        f = (0x8c - g->timer) / 10.0f;
    } else {
        g->timer = -1;
        f = 0.0f;
    }
    for (i = 0; i < 16; i++) {
        a = g->src[i];
        c = g->dst[i];
        sr = a & 0x1f;
        sg = (a >> 5) & 0x1f;
        sb = (a >> 10) & 0x1f;
        db = f * (((c >> 10) & 0x1f) - sb);
        dr = f * ((c & 0x1f) - sr);
        dg = f * (((c >> 5) & 0x1f) - sg);
        g->out[i] = ((sr + dr) & 0x1f) | (((sg + dg) & 0x1f) << 5) | (((sb + db) & 0x1f) << 10);
    }
    g->timer++;
}
