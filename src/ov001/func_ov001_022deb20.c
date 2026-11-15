#include <nitro/types.h>

extern void *data_0208fe80;
u32 func_02006abc(s32 i);
u8 *func_02034a40(void *heap, u32 size, s32 align);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
typedef struct { u8 data[0x138]; } UnkStruct_022deb20_e;
typedef struct { u8 pad[0x18]; u8 *res[2]; u8 pad2[0x3c]; UnkStruct_022deb20_e e[2]; } UnkStruct_022deb20;
void func_02031768(void *p);
void func_0202566c(void *p);

void func_ov001_022deb20(void *a, UnkStruct_022deb20 *p) {
    u8 i;
    for (i = 0; i < 2; i++) {
        func_02031768(&p->e[i]);
        if (p->res[i] != NULL) {
            u8 *blk;
            func_0202566c(p->res[i]);
            blk = p->res[i];
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
        p->res[i] = NULL;
    }
}
