#include <nitro/types.h>

extern void *data_0208fe80;
u32 func_02006abc(s32 i);
u8 *func_02034a40(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
typedef struct { void *a; u8 pad[0x30]; void *b; u8 pad2[0x10]; } UnkStruct_022fdb04;
extern u8 *data_ov001_0231f5a8;
void func_02039bb0(void *a, void *b);
void func_ov001_023063a0(void);

void func_ov001_02306674(void) {
    u8 *blk;
    s32 i;
    UnkStruct_022fdb04 *e;
    func_ov001_023063a0();
    e = (UnkStruct_022fdb04 *)(data_ov001_0231f5a8 + 0x98);
    for (i = 0; i < 8; i++) {
        func_02039bb0(e->a, e->b);
        e++;
    }
    blk = data_ov001_0231f5a8;
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
    data_ov001_0231f5a8 = NULL;
}
