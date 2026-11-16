#include <nitro/types.h>

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

extern void *gSystemHeap;
extern u16 gSaveState;

void SaveData_SetDefaults(u8 *buf);
void func_0200f24c(u16 card);
void func_0200f23c(u16 card);
s32 func_0200f9a0(u32 ofs, void *buf, u32 size, s32 a, s32 b, s32 c);

static inline void Free_02058bbc(u8 *p) {
    if ((u8 *)func_02006abc(0) != p) {
        u32 size;
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gSystemHeap, p);
}

BOOL SaveData_FormatSlot(s32 idx) {
    u16 card;
    u8 *buf;
    s32 r;
    vu32 z;

    card = gSaveState;
    buf = Heap_Alloc(gSystemHeap, 0x340, 0x20);
    if ((u8 *)func_02006abc(0) != buf) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, buf, 0x340);
        *(u32 *)buf = 0x340;
        *(u32 *)(buf + 0x340 - 4) = 0x02467531;
        DC_StoreRange(buf, 0x340);
        buf += 0x20;
    }
    if (buf == (u8 *)func_02006abc(0))
        return FALSE;
    z = 0;
    MIi_CpuClear32(z, buf, 0x314);
    SaveData_SetDefaults(buf);
    func_0200f24c(card);
    r = func_0200f9a0(idx * 0x320 + 0xc, buf, 0x314, 0, 0, 0);
    func_0200f23c(card);
    Free_02058bbc(buf);
    return r ? TRUE : FALSE;
}
