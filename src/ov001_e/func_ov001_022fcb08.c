#include <nitro/types.h>

typedef struct UnkStruct_022fcb08 {
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
    void (*state)(struct UnkStruct_022fcb08 *p);
    u8 pad78[0x84 - 0x78];
    void *unk84;
    u8 pad88[0x1c8 - 0x88];
    s32 unk1C8;
} UnkStruct_022fcb08;

extern void *data_0208fe80;

void func_ov001_02303d28(s32 x, s32 y, s32 count);
void func_02031768(void *p);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
void func_ov001_022fa574(UnkStruct_022fcb08 *p);
void *func_ov001_022fd2dc(s32 a, s32 b);
void func_ov001_022fd220(UnkStruct_022fcb08 *p);
void func_02036754(s32 a, s32 b, s32 c);

static inline void Free(void *p) {
    u8 *blk = p;
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
    func_020349b8(data_0208fe80, blk);
}

void func_ov001_022fcb08(UnkStruct_022fcb08 *p, s32 n) {
    if (p->unk1C8 == n)
        return;
    func_02031768(p->unk84);
    if (!(p->flags & 0x20000000))
        Free(p->unk84);
    func_ov001_022fa574(p);
    {
        void *r = func_ov001_022fd2dc(0, n);
        p->unk1C8 = n;
        p->unk84 = r;
    }
}
