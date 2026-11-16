#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u16 w18;
    u16 w1a;
} UnkRes_02053c58;

typedef struct {
    u32 w[16];
} UnkBg_02053c58;

typedef struct {
    u8 pad0[0x20];
    u32 start;
    u32 end;
    u32 cur;
    u8 pad2c[0x18];
} UnkFile_02053c58;

typedef struct {
    s32 mode;
    s32 idx;
    UnkRes_02053c58 *buf;
    u32 pad0c;
    UnkRes_02053c58 *buf2;
    UnkBg_02053c58 bg1;
    UnkBg_02053c58 bg2;
    UnkBg_02053c58 bg3;
    u32 flags;
    u8 padd8[0x28];
    s32 fidx;
    u32 pad104;
    s32 unk108;
    s32 unk10c;
    u32 pad110[2];
    u32 unk118;
} UnkWork_02053c58;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02053c58 *work;
} UnkTask_02053c58;

typedef struct {
    s32 unk0;
    u32 pad4[3];
    s32 unk10;
} UnkState_02053c58;

typedef struct {
    u8 pad0[0xc];
    u16 unkc;
    u8 pade[0x56];
    u16 unk64;
} UnkProg_02053c58;

typedef struct {
    u32 pad0;
    u8 *hdr;
} UnkInfo_02053c58;

extern s32 data_0207b894;
extern s32 data_0207b8a4;
extern void *data_0207b8e4[][2];
extern UnkInfo_02053c58 **data_0207b8d4[];
extern UnkState_02053c58 gGameState;
extern UnkProg_02053c58 gSaveData;
extern UnkTask_02053c58 *gNaviMsgTask;
extern u32 data_022c4670;
extern u32 data_022c4668;
extern UnkFile_02053c58 *data_022c466c;
extern UnkFile_02053c58 *data_022c4664;
extern void *gMainHeap;

void func_02052874(s32 p0);
void *File_Load(void *a, s32 b);
void Background_InitEx(void *b, void *res, u32 a, u32 idx, u8 e, u32 v14, u32 base, u32 v1c, u32 v20, u16 v24, u16 v26, u16 v28, u16 v2a, u16 v2c, u16 v2e);
void Background_Draw(void *buf);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
s32 FS_ReadFile(void *f, void *buf, u32 size);
void func_02052e1c(UnkWork_02053c58 *w);
void func_02052d90(UnkWork_02053c58 *w, s32 n);
void NNS_SndHandleInit(void *p);
void NaviMsg_Main(void);
void NaviMsg_Destructor(void);
void func_020529d8(void);

#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

static inline void Free_02053c58(u8 *p) {
    if ((u8 *)OS_GetArenaLo(0) != p) {
        u32 size;
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
}

static inline u8 *Alloc_02053c58(u32 sz) {
    u32 size = sz;
    u8 *p = Heap_Alloc(gMainHeap, size, -0x20);
    if ((u8 *)OS_GetArenaLo(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

static inline void MI_CpuClear16_(void *p, u32 size) {
    vu16 z = 0;
    MIi_CpuClear16(z, p, size);
}

static inline void MI_CpuClear32_(void *p, u32 size) {
    vu32 z = 0;
    MIi_CpuClear32(z, p, size);
}

static inline void SetPlane_02053c58(void) {
    u32 plane = (REG_DB_DISPCNT & 0x1f00) >> 8;
    REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | (((plane & ~3) | 4) << 8);
}

static inline void LoadFile_02053c58(UnkFile_02053c58 *f, void *buf) {
    FS_ReadFile(f, buf, f->end - f->start);
    f->cur = f->start;
}

void NaviMsg_Create(void) {
    UnkWork_02053c58 *w;
    UnkRes_02053c58 *res;
    UnkFile_02053c58 *f;
    UnkBg_02053c58 bg;
    UnkBg_02053c58 bg2;
    vu16 z16;
    vu32 z1;
    vu32 z2;

    if (data_0207b894 == 1 && gGameState.unk10 == 0
        && ((gGameState.unk0 == 0 && gSaveData.unkc < 3)
            || (gGameState.unk0 == 1 && (gSaveData.unk64 < 4 || (gSaveData.unk64 >= 0x1b && gSaveData.unk64 < 0x1c))))) {
        func_02052874(data_0207b894);
        res = File_Load(data_0207b8e4[data_0207b8a4][data_0207b894], -1);
        Background_InitEx(&bg, res, 0xe, 1, 2, 0, 0x05000400, 0x06211600, 0xc, 0, 8, 0, 0, res->w18, res->w1a);
        Background_Draw(&bg);
        Free_02053c58((u8 *)res);
        SetPlane_02053c58();
        return;
    }

    REG_DB_DISPCNT &= ~0x1f00;
    gNaviMsgTask = Task_Create(NaviMsg_Main, NaviMsg_Destructor, 0, 0x3000, 3, 0x11c);
    w = gNaviMsgTask->work;
    z16 = 0;
    MIi_CpuClear16(z16, w, 0x11c);
    w->mode = data_0207b894;
    w->idx = data_0207b8a4;
    func_02052874(w->mode);
    z1 = 0;
    MIi_CpuClear32(z1, (void *)0x06203800, 0x800);
    z2 = 0;
    MIi_CpuClear32(z2, (void *)0x06205000, 0x800);

    w->buf = (UnkRes_02053c58 *)Alloc_02053c58((data_022c4670 + 0x43) & ~0x1f);
    res = w->buf;
    f = &data_022c466c[(*data_0207b8d4[w->idx])->hdr[1]];
    LoadFile_02053c58(f, res);
    Background_InitEx(&w->bg1, res, 0x38, 1, 1, 0, 0x05000400, 0x06208000, 0xc, 0, 7, 0x10, 6, res->w18, res->w1a);
    Background_Draw(&w->bg1);
    w->bg1.w[0] &= ~0x3f;
    w->bg1.w[0] |= 0x23;
    w->bg1.w[14] = (u32)w->buf;
    Background_InitEx(&w->bg2, res, 0x23, 1, 1, 0, 0x05000400, 0x06208000, 0xc, 0, 10, 0x10, 6, res->w18, res->w1a);
    w->bg2.w[7] += 0x2000;
    w->bg2.w[14] = (u32)w->buf;

    w->buf2 = (UnkRes_02053c58 *)Alloc_02053c58((data_022c4668 + 0x43) & ~0x1f);
    res = w->buf2;
    if (w->mode == 0)
        w->fidx = 3;
    else
        w->fidx = 0xf;
    f = &data_022c4664[w->fidx];
    LoadFile_02053c58(f, res);
    Background_InitEx(&w->bg3, res, 0x38, 1, 0, 0, 0x05000400, 0x06213200, 0xc, 0, 6, 2, 2, res->w18, res->w1a);
    Background_Draw(&w->bg3);
    w->bg3.w[0] &= ~0x3f;
    w->bg3.w[0] |= 0x31;

    res = File_Load(data_0207b8e4[w->idx][w->mode], -1);
    Background_InitEx(&bg2, res, 0xe, 1, 2, 0, 0x05000400, 0x06211600, 0xc, 0, 8, 0, 0, res->w18, res->w1a);
    Background_Draw(&bg2);
    Free_02053c58((u8 *)res);

    if (w->mode == 0)
        func_02052e1c(w);
    else
        func_02052d90(w, 10);
    SetPlane_02053c58();
    w->flags |= 2;
    Task_Create(func_020529d8, 0, 0, 0xf100, 3, 0);
    w->unk108 = -1;
    w->unk10c = 0;
    NNS_SndHandleInit(&w->unk118);
}
