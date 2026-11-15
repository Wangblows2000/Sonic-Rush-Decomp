#include <nitro/types.h>

typedef struct { u8 pad0[0xc]; void *task0; void *task1; void *snd[5]; } UnkWork_020521b4;
extern UnkWork_020521b4 *data_022c465c;
extern void *data_0208fe80;
extern u8 data_0225044c;
extern u8 data_02250444;
extern u8 data_02250448;

void func_0201c070(void *p, s32 a);
void func_0201bf1c(void *p);
void func_0201b348(void);
void func_02036894(void);
void func_020371c8(void *t);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

#pragma thumb on
static inline void Free_020521b4(u8 *blk) {
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

void func_020521b4(void) {
    if (data_022c465c != NULL) {
        if (data_022c465c->snd[0] != NULL) {
            func_0201c070(&data_022c465c->snd[0], 0);
            func_0201bf1c(&data_022c465c->snd[0]);
        }
        if (data_022c465c->snd[1] != NULL) {
            func_0201c070(&data_022c465c->snd[1], 0);
            func_0201bf1c(&data_022c465c->snd[1]);
        }
        if (data_022c465c->snd[2] != NULL) {
            func_0201c070(&data_022c465c->snd[2], 0);
            func_0201bf1c(&data_022c465c->snd[2]);
        }
        if (data_022c465c->snd[3] != NULL) {
            func_0201c070(&data_022c465c->snd[3], 0);
            func_0201bf1c(&data_022c465c->snd[3]);
        }
        if (data_022c465c->snd[4] != NULL) {
            func_0201c070(&data_022c465c->snd[4], 0);
            func_0201bf1c(&data_022c465c->snd[4]);
        }
        func_0201b348();
        func_02036894();
        if (data_022c465c->task0 != NULL) {
            func_020371c8(data_022c465c->task0);
            data_022c465c->task0 = NULL;
        }
        if (data_022c465c->task1 != NULL) {
            func_020371c8(data_022c465c->task1);
            data_022c465c->task1 = NULL;
        }
        Free_020521b4((u8 *)data_022c465c);
        data_022c465c = NULL;
        data_0225044c = 0x7f;
        data_02250444 = 0x7f;
        data_02250448 = 0x7f;
    }
}
#pragma thumb reset
