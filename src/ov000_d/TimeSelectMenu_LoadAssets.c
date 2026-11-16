#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1010];
    void *res[11];
} UnkStruct_0236290c;

extern void *gSystemHeap;
extern const char **data_ov000_0236995c[];
extern u8 gGameLanguage;

u32 File_GetSize(const char *path);
void *File_Load(const char *path, void *dst);
void *Heap_Alloc(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);

static inline void *UnkAlloc_0236290c(u32 size, s32 align) {
    u32 sz = (size + 0x43) & ~0x1f;
    u8 *p = Heap_Alloc(gSystemHeap, sz, align);
    if (func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, sz);
        *(u32 *)p = sz;
        *(u32 *)(p + sz - 4) = 0x2467531;
        DC_StoreRange(p, sz);
        p += 0x20;
    }
    return p;
}

static inline s32 UnkLang_0236290c(u8 cur) {
    switch (cur) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return cur;
    default:
        return 1;
    }
}

#pragma thumb on

void TimeSelectMenu_LoadAssets(UnkStruct_0236290c *w) {
    const char **tbl = data_ov000_0236995c[UnkLang_0236290c(gGameLanguage)];
    s32 i;

    for (i = 0; i < 11; i++) {
        w->res[i] = UnkAlloc_0236290c(File_GetSize(tbl[i]), -0x20);
        File_Load(tbl[i], w->res[i]);
    }
}

#pragma thumb reset
