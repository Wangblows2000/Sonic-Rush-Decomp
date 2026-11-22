/* Host compositor for the cutscene quad list (same quads the PVR path
 * submits): parallelogram quads, nearest texel, alpha blend, then the
 * brightness overlay; 3D instances are shown as their projected vertices. */
#ifndef CS_RASTER_H
#define CS_RASTER_H
#include <stdio.h>
#include "../cs_internal.h"

static unsigned char g_fb[480][640][3];

static void fb_clear(u32 argb) {
    int x, y;
    for (y = 0; y < 480; y++)
        for (x = 0; x < 640; x++) {
            g_fb[y][x][0] = argb >> 16 & 255;
            g_fb[y][x][1] = argb >> 8 & 255;
            g_fb[y][x][2] = argb & 255;
        }
}

static void fb_blend(int x, int y, u32 c, u32 mod) {
    u32 a = (c >> 24) * (mod >> 24) / 255;
    int k;
    for (k = 0; k < 3; k++) {
        u32 s = (c >> (16 - 8 * k) & 255) * (mod >> (16 - 8 * k) & 255) / 255;
        g_fb[y][x][k] = (unsigned char)((s * a + g_fb[y][x][k] * (255 - a)) / 255);
    }
}

static u32 texel(const CsTex *t, int u, int v) {
    u16 p;
    if (u < 0 || v < 0 || u >= t->tw || v >= t->th)
        return 0;
    p = t->argb[v * t->tw + u];
    if (!(p & 0x8000))
        return 0;
    return 0xff000000u | (u32)((p >> 10 & 31) * 255 / 31) << 16 | (u32)((p >> 5 & 31) * 255 / 31) << 8 |
           (u32)((p & 31) * 255 / 31);
}

static void fb_quad(const CsQuad *q) {
    float ax = q->x[1] - q->x[0], ay = q->y[1] - q->y[0], bx = q->x[2] - q->x[0], by = q->y[2] - q->y[0];
    float det = ax * by - ay * bx, minx = 1e9f, miny = 1e9f, maxx = -1e9f, maxy = -1e9f;
    int k, x, y;
    if (det == 0)
        return;
    for (k = 0; k < 4; k++) {
        if (q->x[k] < minx) minx = q->x[k];
        if (q->x[k] > maxx) maxx = q->x[k];
        if (q->y[k] < miny) miny = q->y[k];
        if (q->y[k] > maxy) maxy = q->y[k];
    }
    if (minx < 0) minx = 0;
    if (miny < 0) miny = 0;
    if (maxx > 640) maxx = 640;
    if (maxy > 480) maxy = 480;
    for (y = (int)miny; y < (int)maxy; y++)
        for (x = (int)minx; x < (int)maxx; x++) {
            float px = x + 0.5f - q->x[0], py = y + 0.5f - q->y[0];
            float s = (px * by - py * bx) / det, t = (ax * py - ay * px) / det;
            if (s < 0 || s >= 1 || t < 0 || t >= 1)
                continue;
            if (q->kind == CSQ_FLAT) {
                fb_blend(x, y, q->argb | 0, 0xffffffffu);
            } else {
                float u = q->u[0] + s * (q->u[1] - q->u[0]) + t * (q->u[2] - q->u[0]);
                float v = q->v[0] + s * (q->v[1] - q->v[0]) + t * (q->v[2] - q->v[0]);
                u32 c = texel(q->tex, (int)u, (int)v);
                if (c)
                    fb_blend(x, y, c, q->argb);
            }
        }
}

int cs3d_project_all(float *xyz, int max);

static void fb_render(void) {
    static float xyz[3 * 200000];
    const CsQuad *q = cs_quads();
    int n = cs_quad_count(), i, nv;
    fb_clear(g_cs_backdrop ? g_cs_backdrop : 0x404060);
    nv = cs3d_project_all(xyz, 200000);
    for (i = 0; i < nv; i++) {
        int x = (int)xyz[3 * i], y = (int)xyz[3 * i + 1];
        if (x >= 0 && x < 640 && y >= 0 && y < 480 && xyz[3 * i + 2] > 0) {
            g_fb[y][x][0] = 255;
            g_fb[y][x][1] = 220;
            g_fb[y][x][2] = 120;
        }
    }
    for (i = 0; i < n; i++)
        fb_quad(&q[i]);
    if (g_cs_bright) {
        int b = g_cs_bright < 0 ? -g_cs_bright : g_cs_bright;
        u32 a = (u32)(b >= 16 ? 255 : b * 255 / 16);
        CsQuad o;
        memset(&o, 0, sizeof(o));
        o.kind = CSQ_FLAT;
        o.x[1] = 640; o.x[3] = 640; o.y[2] = 480; o.y[3] = 480;
        o.argb = a << 24 | (g_cs_bright > 0 ? 0xffffff : 0);
        fb_quad(&o);
    }
}

static void fb_save(const char *path) {
    FILE *f = fopen(path, "wb");
    if (!f)
        return;
    fprintf(f, "P6\n640 480\n255\n");
    fwrite(g_fb, 1, sizeof(g_fb), f);
    fclose(f);
}
#endif
