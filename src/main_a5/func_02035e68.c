#include <nitro/types.h>

typedef struct {
    u8 unk0[4];
    u16 unk4;
    u16 dstPitch;
    u32 dstOfs;
    u32 dstBase;
    u16 width;
    u16 height;
    void *src;
    u32 srcPitch;
    u32 srcOfs;
} UnkXfer_02035e68;

typedef struct {
    u16 pitch;
    u16 unk2;
} UnkBgInfo_02035e68;

extern u32 data_02078044[];
extern u16 data_0207316c[];
extern UnkBgInfo_02035e68 data_0207319c[];

UnkXfer_02035e68 *func_02035c58(void);
void *func_02006abc(u32 a);

void func_02035e68(void *src, u16 srcX, u16 srcY, u32 srcW, BOOL skip, int size, int p7, int p8, u16 x, u16 y, u16 w, u16 h) {
    UnkXfer_02035e68 *e;
    u32 isSub;
    u32 base[2];
    u16 unit;
    BOOL done;

    if (size >= 0 && size < 12) {
        isSub = 0;
    } else {
        isSub = 1;
    }
    base[0] = data_02078044[0];
    base[1] = data_02078044[1];
    unit = data_0207316c[size];
    done = FALSE;
    if (skip == 0) {
        switch (size) {
        case 3:
        case 15:
            if (x < 32 && y + h > 32) {
                u16 sy, dy, hh, ww;
                e = func_02035c58();
                if (func_02006abc(0) == e) return;
                if (y > 32) {
                    sy = srcY;
                    dy = y;
                    hh = h;
                } else {
                    sy = srcY + (32 - y);
                    hh = h - (32 - y);
                    dy = 0;
                }
                if (x + w <= 32) {
                    ww = w;
                } else {
                    ww = 32 - x;
                }
                e->unk4 = 0;
                e->dstPitch = unit << 5;
                e->dstOfs = unit * (x + (dy << 5) + 0x800);
                e->dstBase = base[isSub] + ((p7 << 16) + (p8 << 11));
                e->src = src;
                e->srcPitch = unit * srcW;
                e->srcOfs = unit * (srcW * sy + srcX);
                e->width = unit * ww;
                e->height = hh;
            }
            if (x + w > 32 && y + h > 32) {
                u16 sx, dx, ww, sy, dy, hh;
                e = func_02035c58();
                if (func_02006abc(0) == e) return;
                if (x > 32) {
                    sx = srcX;
                    dx = x - 32;
                    ww = w;
                } else {
                    sx = srcX + (32 - x);
                    ww = w - (32 - x);
                    dx = 0;
                }
                if (y > 32) {
                    sy = srcY;
                    dy = y;
                    hh = h;
                } else {
                    sy = srcY + (32 - y);
                    hh = h - (32 - y);
                    dy = 0;
                }
                e->unk4 = 0;
                e->dstPitch = unit << 5;
                e->dstOfs = unit * (dx + (dy << 5) + 0xc00);
                e->dstBase = base[isSub] + ((p7 << 16) + (p8 << 11));
                e->src = src;
                e->srcPitch = unit * srcW;
                e->srcOfs = unit * (srcW * sy + sx);
                e->width = unit * ww;
                e->height = hh;
            }
        case 1:
        case 13:
            done = TRUE;
            if (x < 32 && y < 32) {
                u16 ww, hh;
                e = func_02035c58();
                if (func_02006abc(0) == e) return;
                if (x + w < 32) {
                    ww = w;
                } else {
                    ww = 32 - x;
                }
                if (y + h <= 32) {
                    hh = h;
                } else {
                    hh = 32 - y;
                }
                e->unk4 = 0;
                e->dstPitch = unit << 5;
                e->dstOfs = unit * (x + (y << 5));
                e->dstBase = base[isSub] + ((p7 << 16) + (p8 << 11));
                e->src = src;
                e->srcPitch = unit * srcW;
                e->srcOfs = unit * (srcW * srcY + srcX);
                e->width = unit * ww;
                e->height = hh;
            }
            if (x + w > 32 && y < 32) {
                u16 sx, dx, ww, hh;
                e = func_02035c58();
                if (func_02006abc(0) == e) return;
                if (x > 32) {
                    sx = srcX;
                    dx = x - 32;
                    ww = w;
                } else {
                    sx = srcX + (32 - x);
                    ww = w - (32 - x);
                    dx = 0;
                }
                if (y + h <= 32) {
                    hh = h;
                } else {
                    hh = 32 - y;
                }
                e->unk4 = 0;
                e->dstPitch = unit << 5;
                e->dstOfs = unit * (dx + (y << 5) + 0x400);
                e->dstBase = base[isSub] + ((p7 << 16) + (p8 << 11));
                e->src = src;
                e->srcPitch = unit * srcW;
                e->srcOfs = unit * (srcW * srcY + sx);
                e->width = unit * ww;
                e->height = hh;
            }
            break;
        }
    }
    if (done) return;
    e = func_02035c58();
    if (func_02006abc(0) == e) return;
    e->unk4 = 0;
    e->dstPitch = unit * data_0207319c[size].pitch;
    e->dstOfs = unit * (y * data_0207319c[size].pitch + x);
    e->dstBase = base[isSub] + ((p7 << 16) + (p8 << 11));
    e->src = src;
    e->srcPitch = unit * srcW;
    e->srcOfs = unit * (srcW * srcY + srcX);
    e->width = unit * w;
    e->height = h;
}
