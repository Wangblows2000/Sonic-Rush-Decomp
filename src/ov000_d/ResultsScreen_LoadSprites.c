#include <nitro/types.h>

typedef struct {
    u8 pad0[0xE14];
    void *res[8];
} UnkStruct_0235f024;

extern void *gSystemHeap;
extern const char *data_ov000_023692b4[];
extern const char *data_ov000_023692d4[];
extern u8 gGameLanguage;

u32 File_GetSize(const char *path);
void *File_Load(const char *path, void *dst);
void *Heap_Alloc(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);

static inline void *UnkAlloc_0235f024(u32 size, s32 align) {
    u8 *p;
    size = (size + 0x43) & ~0x1f;
    p = Heap_Alloc(gSystemHeap, size, align);
    if (func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x2467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

static inline s32 UnkLang_0235f024(u8 cur) {
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

void ResultsScreen_LoadSprites(UnkStruct_0235f024 *w) {
    s32 i;
    const char **tbl;
    u8 cur;

    i = 0;
    tbl = data_ov000_023692b4;
    cur = gGameLanguage;
    for (; i < 8; i++) {
        if (i == 1) {
            w->res[i] = UnkAlloc_0235f024(File_GetSize(data_ov000_023692d4[UnkLang_0235f024(cur)]), -0x20);
            File_Load(data_ov000_023692d4[UnkLang_0235f024(cur)], w->res[i]);
        } else {
            w->res[i] = UnkAlloc_0235f024(File_GetSize(tbl[i]), -0x20);
            File_Load(tbl[i], w->res[i]);
        }
    }
}

#pragma thumb reset
