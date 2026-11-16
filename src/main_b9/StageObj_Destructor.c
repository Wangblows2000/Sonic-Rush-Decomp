#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkRes_0206bef4;

typedef struct UnkNode_0206bef4 {
    u8 pad0[0xa0];
    void *unkA0;
    UnkRes_0206bef4 *unkA4;
    u32 unkA8;
    struct UnkNode_0206bef4 *next;
} UnkNode_0206bef4;

typedef struct {
    u8 pad0[0x138];
    void *unk138;
    void *unk13C[5];
    UnkRes_0206bef4 *unk150;
    UnkRes_0206bef4 *unk154[5];
    u32 unk168;
} UnkA_0206bef4;

typedef struct {
    u8 pad0[0xf4];
    void *unkF4;
    void *unkF8;
    u32 unkFC;
    void *unk100;
} UnkC_0206bef4;

typedef struct {
    u8 pad0[0x54];
    u32 flags;
    u8 pad58[0x8c - 0x58];
    void *unk8C;
    u8 pad90[0xa4 - 0x90];
    void *unkA4;
    u8 padA8[0xd8 - 0xa8];
    u16 unkD8;
    u8 padDA[0x124 - 0xda];
    UnkA_0206bef4 *unk124;
    UnkNode_0206bef4 *unk128;
    UnkC_0206bef4 *unk12C;
} UnkWork_0206bef4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0206bef4 *work;
} UnkTask_0206bef4;

extern void *gMainHeap;

u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void PaletteAnim_Release(u16 a);
void ObjDraw_ReleaseSpritePalette(s32 i);
void ObjData_Release(void *p);
void StageObj_ReleaseSpriteVram(UnkWork_0206bef4 *w);
void Model_Release(void *p);
void func_0202566c(void *p);
void func_020682f0(void *p);

static inline void Free_0206bef4(u8 *blk) {
    if ((u8 *)OS_GetArenaLo(0) != blk) {
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

void StageObj_Destructor(UnkTask_0206bef4 *task) {
    UnkWork_0206bef4 *work = task->work;
    UnkNode_0206bef4 *node;
    UnkC_0206bef4 *c;
    u8 i;

    if (work->flags & 0x800)
        PaletteAnim_Release(work->unkD8);
    ObjDraw_ReleaseSpritePalette(work->unkD8 & 0xff);
    if (work->unk8C != NULL) {
        ObjData_Release(work->unk8C);
    } else if (work->unkA4 != NULL && !(work->flags & 0x100)) {
        Free_0206bef4(work->unkA4);
    }
    StageObj_ReleaseSpriteVram(work);

    if (work->unk124 != NULL) {
        Model_Release(work->unk124);
        if (work->unk124->unk150 != NULL) {
            if (work->unk124->unk150->unk4 == 1)
                func_0202566c(work->unk124->unk138);
            ObjData_Release(work->unk124->unk150);
            work->unk124->unk138 = NULL;
        } else if (work->unk124->unk138 != NULL && !(work->unk124->unk168 & 1)) {
            func_0202566c(work->unk124->unk138);
            Free_0206bef4(work->unk124->unk138);
            work->unk124->unk138 = NULL;
        }
        for (i = 0; i < 5; i++) {
            if (work->unk124->unk154[i] != NULL) {
                if (work->unk124->unk154[i]->unk4 == 1)
                    func_0202566c(work->unk124->unk154[i]);
                ObjData_Release(work->unk124->unk154[i]);
                work->unk124->unk13C[i] = NULL;
            } else if (work->unk124->unk13C[i] != NULL && !(work->unk124->unk168 & (2 << i))) {
                func_0202566c(work->unk124->unk154[i]);
                Free_0206bef4(work->unk124->unk13C[i]);
                work->unk124->unk13C[i] = NULL;
            }
        }
    }

    for (node = work->unk128; node != NULL; node = node->next) {
        Model_Release(node);
        if (node->unkA4 != NULL) {
            if (node->unkA4->unk4 == 1)
                func_0202566c(node->unkA0);
            ObjData_Release(node->unkA4);
            node->unkA0 = NULL;
        } else if (node->unkA0 != NULL && !(node->unkA8 & 1)) {
            func_0202566c(node->unkA0);
            Free_0206bef4(node->unkA0);
            node->unkA0 = NULL;
        }
    }

    c = work->unk12C;
    if (c == NULL)
        return;
    if (c->unk100 != NULL)
        func_020682f0(c);
    else
        Model_Release(c);
    if (work->unk12C->unkF8 != NULL) {
        ObjData_Release(work->unk12C->unkF8);
        work->unk12C->unkF4 = NULL;
    } else if (work->unk12C->unkF4 != NULL && !(work->unk12C->unkFC & 1)) {
        Free_0206bef4(work->unk12C->unkF4);
        work->unk12C->unkF4 = NULL;
    }
}
