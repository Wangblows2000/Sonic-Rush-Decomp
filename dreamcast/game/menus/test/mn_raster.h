/* Host-only: software composite of the menu quad list into a 640x480 RGB
 * image (nearest sampling, alpha 1-bit + quad alpha), for visual checks. */
#include <stdio.h>
#include "../mn_internal.h"

static unsigned char g_img[480][640][3];

static void raster_quad(const MnQuad *q, const MnOut *o) {
    int x0 = (int)(o->x + 0.5f), y0 = (int)(o->y + 0.5f), x1 = (int)(o->x + o->w + 0.5f), y1 = (int)(o->y + o->h + 0.5f);
    int x, y;
    float qa = (q->argb >> 24) / 255.0f;
    for (y = y0; y < y1; y++) {
        if (y < 0 || y >= 480)
            continue;
        for (x = x0; x < x1; x++) {
            float u, v;
            unsigned short p;
            int r, g, b;
            if (x < 0 || x >= 640)
                continue;
            u = o->u0 + (o->u1 - o->u0) * ((x + 0.5f - o->x) / o->w);
            v = o->v0 + (o->v1 - o->v0) * ((y + 0.5f - o->y) / o->h);
            if (q->kind == MNQ_FLAT) {
                r = q->argb >> 16 & 255;
                g = q->argb >> 8 & 255;
                b = q->argb & 255;
            } else {
                int iu = (int)u, iv = (int)v;
                if (q->kind == MNQ_SPRITE) {
                    const MnRes *res = q->res;
                    if (iu < 0 || iv < 0 || iu >= res->pages[q->page].w || iv >= res->pages[q->page].h)
                        continue;
                    if (res->texels[q->page] && q->image < res->pal.n_images &&
                        res->pal.image_pal[q->image] < res->pal.n_pal) {   /* PAL4 page */
                        int t = sprpal_texel(res->texels[q->page], res->pages[q->page].w, res->pages[q->page].h, iu, iv);
                        p = t ? res->pal.pal[16u * res->pal.image_pal[q->image] + t] : 0;
                    } else if (res->pixels[q->page]) {
                        p = res->pixels[q->page][iv * res->pages[q->page].w + iu];
                    } else {
                        continue;
                    }
                } else {
                    MnBgImg *bg = (MnBgImg *)q->bg;
                    if (bg->dirty || !bg->argb) {
                        mn_bg_expand(bg);
                        bg->dirty = 0;
                    }
                    if (iu < 0 || iv < 0 || iu >= bg->w || iv >= bg->h)
                        continue;
                    p = bg->argb[iv * bg->tw + iu];
                }
                if (!(p & 0x8000))
                    continue;
                r = (p >> 10 & 31) * 255 / 31;
                g = (p >> 5 & 31) * 255 / 31;
                b = (p & 31) * 255 / 31;
            }
            g_img[y][x][0] = (unsigned char)(g_img[y][x][0] * (1 - qa) + r * qa);
            g_img[y][x][1] = (unsigned char)(g_img[y][x][1] * (1 - qa) + g * qa);
            g_img[y][x][2] = (unsigned char)(g_img[y][x][2] * (1 - qa) + b * qa);
        }
    }
}

static void raster_frame(const char *path) {
    const MnDraw *s;
    int n = mn_sorted(&s), i, x, y;
    MnOut o;
    FILE *f;
    int b = g_mn_bright[g_mn_comp.base];
    memset(g_img, 0, sizeof(g_img));
    if (g_mn_comp.fill)
        for (y = 0; y < 480; y++)
            for (x = 0; x < 640; x++) {
                g_img[y][x][0] = g_mn_comp.fill >> 16 & 255;
                g_img[y][x][1] = g_mn_comp.fill >> 8 & 255;
                g_img[y][x][2] = g_mn_comp.fill & 255;
            }
    for (i = 0; i < n; i++)
        if (mn_project(s[i].q, s[i].r, &o))
            raster_quad(s[i].q, &o);
    if (g_mn_comp.nbr > 0) {   /* Battle Play: a brightness per player half */
        int k2;
        for (k2 = 0; k2 < g_mn_comp.nbr && k2 < 2; k2++) {
            int bb = g_mn_bright[g_mn_comp.br[k2].engine & 3];
            float a = (bb < 0 ? -bb : bb) / 16.0f;
            int bx0 = (int)g_mn_comp.br[k2].x, by0 = (int)g_mn_comp.br[k2].y;
            int bx1 = bx0 + (int)g_mn_comp.br[k2].w, by1 = by0 + (int)g_mn_comp.br[k2].h;
            if (bb == 0)
                continue;
            for (y = by0 < 0 ? 0 : by0; y < by1 && y < 480; y++)
                for (x = bx0 < 0 ? 0 : bx0; x < bx1 && x < 640; x++) {
                    int k;
                    for (k = 0; k < 3; k++)
                        g_img[y][x][k] = (unsigned char)(g_img[y][x][k] * (1 - a) + (bb > 0 ? 255 : 0) * a);
                }
        }
        b = 0;
    }
    if (b != 0) {   /* one brightness pass over the frame */
        float a = (b < 0 ? -b : b) / 16.0f;
        for (y = 0; y < 480; y++)
            for (x = 0; x < 640; x++) {
                int k;
                for (k = 0; k < 3; k++)
                    g_img[y][x][k] = (unsigned char)(g_img[y][x][k] * (1 - a) + (b > 0 ? 255 : 0) * a);
            }
    }
    f = fopen(path, "wb");
    if (!f)
        return;
    fprintf(f, "P6\n640 480\n255\n");
    fwrite(g_img, 1, sizeof(g_img), f);
    fclose(f);
}
