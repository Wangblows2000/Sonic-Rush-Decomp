#include <nitro/types.h>
#include <main_b3/w50.h>

typedef struct { u8 pad0[0x18]; u16 unk18; u16 unk1A; } UnkRes_02063228;
typedef struct {
    s32 unk0;
    u8 pad4[4];
    s32 unk8;
    u8 padC[4];
    UnkSpr_W50 spr[4];        /* 0x10 */
    u8 bg0[0x40];             /* 0x130 */
    u8 bg1[0x40];             /* 0x170 */
    UnkRes_02063228 *buf[4];  /* 0x1b0 */
} UnkWork_02063228;

extern u8 gGameLanguage;
extern void **data_0207cf48[];
extern void *gSystemHeap;

s32 File_GetSize(void *path);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void *File_Load(void *a, void *b);
void *ObjVram_Alloc(u32 a, u32 b);
void AnimatorSprite_Init(UnkSpr_W50 *s, void *res, u32 idx, u32 c, u32 d, u32 e, void *f, u32 g, u32 h);
void Background_Init(void *buf, void *res, s32 a, s32 b, s32 c, s32 d, s32 e);
void Background_Draw(void *buf);

#pragma thumb on
static inline u8 *Alloc_02063228(u32 size) {
    u8 *p = Heap_Alloc(gSystemHeap, size, 0x20);
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

void func_02063228(UnkWork_02063228 *w) {
    u32 lang;
    void **files;
    s32 i;
    u32 size;
    UnkRes_02063228 *res;

    w->unk0 = 1;
    w->unk8 = 0;
    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        lang = gGameLanguage;
        break;
    default:
        lang = 1;
        break;
    }
    files = data_0207cf48[lang];
    for (i = 0; i < 4; i++) {
        size = (File_GetSize(files[i]) + 0x43) & ~0x1f;
        w->buf[i] = (UnkRes_02063228 *)Alloc_02063228(size);
        File_Load(files[i], w->buf[i]);
    }
    res = w->buf[1];
    AnimatorSprite_Init(&w->spr[0], res, 1, 0, 0, 0, ObjVram_Alloc(0, *(u16 *)((u8 *)res + *(u32 *)&res->unk18 + 8)), 0, 0x05000200);
    AnimatorSprite_Init(&w->spr[1], res, 2, 0, 0, 0, ObjVram_Alloc(0, *(u16 *)((u8 *)res + *(u32 *)&res->unk18 + 8)), 0, 0x05000200);
    res = w->buf[0];
    AnimatorSprite_Init(&w->spr[2], res, 5, 0, 0, 0, ObjVram_Alloc(0, *(u16 *)((u8 *)res + *(u32 *)&res->unk18 + 8)), 0, 0x05000200);
    res = w->buf[2];
    AnimatorSprite_Init(&w->spr[3], res, 1, 0, 0, 0, ObjVram_Alloc(0, *(u16 *)((u8 *)res + *(u32 *)&res->unk18 + 8)), 0, 0x05000200);
    res = w->buf[3];
    Background_Init(w->bg0, res, 0x38, 0, 3, res->unk18, res->unk1A);
    Background_Draw(w->bg0);
    Background_Init(w->bg1, res, 0x38, 1, 3, res->unk18, res->unk1A);
    Background_Draw(w->bg1);
}
#pragma thumb reset
