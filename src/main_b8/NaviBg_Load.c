#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u16 w18;
    u16 w1a;
} UnkRes_0205453c;

typedef struct {
    u32 w[16];
} UnkBg_0205453c;

typedef struct {
    u8 pad0[0x20];
    u32 start;
    u32 end;
    u8 pad28[0x1c];
} UnkFile_0205453c;

typedef struct {
    char s[0x40];
} UnkPath_0205453c;

extern s32 data_0207b894;
extern s32 data_0207b8a4;
extern void *data_0207b8e4[][2];
extern void *data_0207b8bc[];
extern s32 data_02075674[];
extern s32 data_0207565c[];
extern const char *data_0207b8c4[];
extern const char **data_0207b8cc[];
extern const char **data_0207b8b4[];
extern const char *data_0207b8dc[];
extern const char *data_0207b9bc[];
extern char data_0207ba0c[];
extern char data_0207ba14[];
extern u8 gGameLanguage;
extern UnkFile_0205453c *data_022c466c;
extern UnkFile_0205453c *data_022c4664;
extern u32 data_022c4670;
extern u32 data_022c4668;
extern void *gMainHeap;

void *File_Load(void *a, s32 b);
void Background_InitEx(void *b, void *res, u32 a, u32 idx, u8 e, u32 v14, u32 base, u32 v1c, u32 v20, u16 v24, u16 v26, u16 v28, u16 v2a, u16 v2c, u16 v2e);
void Background_Draw(void *buf);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void func_0200be70(UnkFile_0205453c *f);
void func_0200bb20(UnkFile_0205453c *f, UnkPath_0205453c *path);
void func_0202c518(UnkPath_0205453c *dst, const char *src);
void func_0202c474(UnkPath_0205453c *dst, const char *src);

#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

#define Free_0205453c(p0) do { u8 *p = (p0); \
    if ((u8 *)func_02006abc(0) != p) { \
        p -= 0x20; \
        n = *(u32 *)p; \
        if (*(u32 *)(p + n - 4) == 0x02467531) { \
            vu32 fill = 0xcccccccc; \
            MIi_CpuClear32(fill, p, n); \
            DC_StoreRange(p, n); \
        } \
    } \
    Heap_Free(gMainHeap, p); } while (0)

static inline u8 *Alloc_0205453c(u32 sz) {
    u32 size = sz;
    u8 *p = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

void NaviBg_Load(s32 mode, s32 idx) {
    UnkRes_0205453c *res;
    UnkFile_0205453c *f;
    u32 max;
    s32 n;
    s32 i;
    u32 len;
    u8 lang;
    u32 k;
    UnkBg_0205453c bg;
    UnkPath_0205453c path;

    data_0207b894 = mode;
    data_0207b8a4 = idx;
    {
        u32 plane = (REG_DB_DISPCNT & 0x1f00) >> 8;
        REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | ((plane & ~5) << 8);
    }
    res = File_Load(data_0207b8e4[idx][mode], -1);
    Background_InitEx(&bg, res, 0x31, 1, 2, 0, 0x05000400, 0x06211600, 0xc, 0, 8, 0, 0, res->w18, res->w1a);
    Background_Draw(&bg);
    Free_0205453c((u8 *)res);

    res = File_Load(data_0207b8bc[mode], -1);
    Background_InitEx(&bg, res, 0x31, 1, 0, 0, 0x05000400, 0x06212f00, 0xc, 0, 6, 0, 0, res->w18, res->w1a);
    Background_Draw(&bg);
    Free_0205453c((u8 *)res);

    n = data_02075674[idx];
    max = 0;
    f = (UnkFile_0205453c *)Alloc_0205453c((n * 0x44 + 0x43) & ~0x1f);
    data_022c466c = f;
    i = 0;
    if (n > 0) do {
        func_0200be70(f);
        func_0202c518(&path, data_0207b8c4[idx]);
        func_0202c474(&path, data_0207b8cc[idx][i]);
        func_0202c474(&path, data_0207ba0c);
        func_0200bb20(f, &path);
        len = f->end - f->start;
        if (max < len)
            max = len;
        f++;
        i++;
    } while (i < data_02075674[idx]);

    n = data_0207565c[idx];
    data_022c4670 = max;
    f = (UnkFile_0205453c *)Alloc_0205453c((n * 0x44 + 0x43) & ~0x1f);
    data_022c4664 = f;
    i = 0;
    if (n > 0) {
    lang = gGameLanguage;
    do {
        func_0200be70(f);
        func_0202c518(&path, data_0207b8dc[idx]);
        switch (lang) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            k = lang;
            break;
        default:
            k = 1;
            break;
        }
        func_0202c474(&path, data_0207b9bc[k]);
        func_0202c474(&path, data_0207b8b4[idx][i]);
        func_0202c474(&path, data_0207ba14);
        func_0200bb20(f, &path);
        len = f->end - f->start;
        if (max < len)
            max = len;
        f++;
        i++;
    } while (i < data_0207565c[idx]);
    }
    data_022c4668 = max;
}
