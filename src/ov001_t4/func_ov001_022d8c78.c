#include <nitro/types.h>

#define REG16(a) (*(vu16 *)(a))
#define REG32(a) (*(vu32 *)(a))

typedef struct {
    u8 pad0[0x34];
    s32 unk34;
    u8 pad38[0x10];
} UnkSprite_022d8c78;

typedef struct {
    u8 pad0[0x25c];
    UnkSprite_022d8c78 sprites[19];
    s32 ids[19];
    u8 pad800[8];
    u8 *res[9];
} UnkWork_022d8c78;

typedef struct {
    s16 x;
    s16 y;
} UnkOfs_022d8c78;

typedef struct {
    UnkOfs_022d8c78 ofs[4];
} UnkBg_022d8c78;

extern void *gMainHeap;
extern UnkBg_022d8c78 gDisplayMain;
extern UnkBg_022d8c78 gDisplaySub;

void ObjVram_Free(s32 a, s32 b);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void func_02049070(void);

#pragma thumb on
static inline void Free_022d8c78(u8 *blk) {
    if ((u8 *)func_02006abc(0) != blk) {
        u32 size;
        blk -= 0x20;
        size = *(u32 *)blk;
        if (*(u32 *)(blk + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, blk, size);
            DC_StoreRange(blk, size);
        }
    }
    Heap_Free(gMainHeap, blk);
}

void func_ov001_022d8c78(UnkWork_022d8c78 *w) {
    s32 i;

    if (w != NULL) {
        ObjVram_Free(1, w->sprites[0].unk34);
        ObjVram_Free(1, w->sprites[1].unk34);
        ObjVram_Free(0, w->sprites[2].unk34);
        ObjVram_Free(0, w->sprites[3].unk34);
        ObjVram_Free(0, w->sprites[4].unk34);
        ObjVram_Free(0, w->sprites[5].unk34);
        ObjVram_Free(0, w->sprites[6].unk34);
        ObjVram_Free(0, w->sprites[7].unk34);
        ObjVram_Free(0, w->sprites[8].unk34);
        ObjVram_Free(1, w->sprites[9].unk34);
        ObjVram_Free(1, w->sprites[10].unk34);
        ObjVram_Free(1, w->sprites[11].unk34);
        ObjVram_Free(1, w->sprites[12].unk34);
        ObjVram_Free(1, w->sprites[13].unk34);
        ObjVram_Free(1, w->sprites[14].unk34);
        ObjVram_Free(1, w->sprites[15].unk34);
        ObjVram_Free(1, w->sprites[16].unk34);
        ObjVram_Free(1, w->sprites[17].unk34);
        ObjVram_Free(1, w->sprites[18].unk34);
        for (i = 0; i < 9; i++) {
            Free_022d8c78(w->res[i]);
        }
        REG16(0x5000000) = 0xffff;
        REG16(0x5000400) = 0xffff;
        REG32(0x4000000) = REG32(0x4000000) & ~0x1f00;
        REG32(0x4001000) = REG32(0x4001000) & ~0x1f00;
        func_02049070();
        gDisplayMain.ofs[0].x = 0;
        gDisplayMain.ofs[0].y = 0;
        gDisplayMain.ofs[1].x = 0;
        gDisplayMain.ofs[1].y = 0;
        gDisplayMain.ofs[2].x = 0;
        gDisplayMain.ofs[2].y = 0;
        gDisplayMain.ofs[3].x = 0;
        gDisplayMain.ofs[3].y = 0;
        gDisplaySub.ofs[0].x = 0;
        gDisplaySub.ofs[0].y = 0;
        gDisplaySub.ofs[1].x = 0;
        gDisplaySub.ofs[1].y = 0;
        gDisplaySub.ofs[2].x = 0;
        gDisplaySub.ofs[2].y = 0;
        gDisplaySub.ofs[3].x = 0;
        gDisplaySub.ofs[3].y = 0;
    }
}
#pragma thumb reset
