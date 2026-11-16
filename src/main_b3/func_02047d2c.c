#include <nitro/types.h>

typedef struct {
    u8 pad0[0x34];
    void *unk34;
    u8 pad38[0x10];
} UnkSpr_02047d2c;
typedef struct {
    u8 pad0[0x14];
    u8 obj14[0x384];       /* 0x14 */
    u8 *res398;            /* 0x398 */
    UnkSpr_02047d2c spr[0x33];  /* 0x39c */
    UnkSpr_02047d2c spr2[0xf];  /* 0x11f4 */
    u8 pad162C[8];
    u8 unk1634[4];         /* 0x1634 */
} UnkWork_02047d2c;
typedef struct { u8 pad0[0x10]; UnkWork_02047d2c *work; } UnkTask_02047d2c;
typedef struct { u8 pad0[0x10]; s32 unk10; u8 pad14[8]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
extern void *gMainHeap;

void ObjVram_Free(s32 a, void *b);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void Model_Release(void *p);
void func_0201bf1c(void *p);
void func_02053bec(s32 a);
void NaviMsg_Destroy(void);
void func_02044cf0(void);
void func_0205a3f0(void);
void func_0205a4ec(s32 a);
void func_0201b348(void);
void Sound_ResetHeap(void);

#pragma thumb on
static inline void Free_02047d2c(u8 *blk) {
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

void func_02047d2c(UnkTask_02047d2c *t) {
    UnkWork_02047d2c *w = t->work;
    u32 i;

    if (data_022c4560.unk10 == 2) {
        for (i = 0; i < 0xf; i++) {
            if (w->spr2[i].unk34 != NULL)
                ObjVram_Free(0, w->spr2[i].unk34);
        }
        Free_02047d2c(w->res398);
    }
    for (i = 0; i < 0x33; i++)
        ObjVram_Free(0, w->spr[i].unk34);
    Model_Release(w->obj14);
    func_0201bf1c(w->unk1634);
    if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2) {
        func_02053bec(1);
        NaviMsg_Destroy();
        func_02044cf0();
    }
    if (data_022c4560.unk10 != 2)
        func_0205a3f0();
    func_0205a4ec(0);
    func_0201b348();
    Sound_ResetHeap();
}
#pragma thumb reset
