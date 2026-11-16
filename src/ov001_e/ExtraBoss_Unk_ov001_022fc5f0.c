#include <nitro/types.h>

typedef struct UnkStruct_022fc5f0 {
    u8 pad0[0x14];
    u32 flags;
    s32 x;
    s32 y;
    u8 pad20[0x50 - 0x20];
    s32 unk50;
    u8 pad54[0x68 - 0x54];
    u16 unk68;
    u16 unk6A;
    u16 unk6C;
    u8 pad6E[0x74 - 0x6e];
    void (*state)(struct UnkStruct_022fc5f0 *p);
    u8 pad78[0x84 - 0x78];
    void *unk84;
    u8 pad88[0x1c8 - 0x88];
    s32 unk1C8;
} UnkStruct_022fc5f0;

extern void *gMainHeap;

void ExtraBoss_Unk_ov001_02303d28(s32 x, s32 y, s32 count);
void Model_Release(void *p);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void ExtraBossObj_Unk_ov001_022fa574(UnkStruct_022fc5f0 *p);
void *ExtraBossPlayer_Unk_ov001_022fd2dc(s32 a, s32 b);
void ExtraBossObj_State_Unk_ov001_022fd220(UnkStruct_022fc5f0 *p);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);

static inline void Free(void *p) {
    u8 *blk = p;
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

void ExtraBoss_Unk_ov001_022fc5f0(void *self, UnkStruct_022fc5f0 **other) {
    UnkStruct_022fc5f0 *p = *other;
    s32 n = 3;

    if (p->unk1C8 == 4) {
        ExtraBoss_Unk_ov001_02303d28(p->x, p->y, 5);
        n = 5;
    }
    Model_Release(p->unk84);
    if (!(p->flags & 0x20000000))
        Free(p->unk84);
    ExtraBossObj_Unk_ov001_022fa574(p);
    {
        void *r = ExtraBossPlayer_Unk_ov001_022fd2dc(0, n);
        p->unk1C8 = n;
        p->unk84 = r;
    }
    p->state = ExtraBossObj_State_Unk_ov001_022fd220;
    p->unk50 = 0;
    p->unk68 = p->unk6A = p->unk6C = 0;
    p->flags |= 0x1000;
    Sound_PlaySeqArc(0, 0, 0xa7);
}
