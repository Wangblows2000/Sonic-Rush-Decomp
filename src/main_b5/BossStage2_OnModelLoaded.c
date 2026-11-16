#include <nitro/types.h>

typedef struct {
    const char *name;
    u32 unk4;
    u32 unk8;
    u32 unkc;
} UnkEntry_0204a880;

typedef struct {
    u8 pad0[0x5c];
    void *res;
} UnkWork_0204a880;

extern UnkEntry_0204a880 data_020747a0[];
extern u8 *gBossStage2Models[];
extern void *gMainHeap;
extern u32 gGameState;

s32 strcmp(const char *a, const char *b);
void NNS_G3dResDefaultSetup(void *p);
u32 Asset3D_GetResourceSize(void *p);
void Asset3D_CopyResource(void *p, void *buf);
void NaviBg_Load(s32 a, u32 b);
void NaviB_Load(void);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);

static inline u8 *Alloc_0204a880(u32 sz) {
    u32 size = sz;
    u8 *p = Heap_Alloc(gMainHeap, size, 0x20);
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

void BossStage2_OnModelLoaded(UnkWork_0204a880 *w, const char *name) {
    UnkEntry_0204a880 *tbl;
    s32 i;

    tbl = data_020747a0;
    for (i = 0; i < 11; i++) {
        if (strcmp(tbl[i].name, name) == 0)
            break;
    }
    NNS_G3dResDefaultSetup(w->res);
    gBossStage2Models[i] = Alloc_0204a880((Asset3D_GetResourceSize(w->res) + 0x43) & ~0x1f);
    Asset3D_CopyResource(w->res, gBossStage2Models[i]);
    if (i == 10) {
        NaviBg_Load(1, gGameState);
        NaviB_Load();
    }
}
