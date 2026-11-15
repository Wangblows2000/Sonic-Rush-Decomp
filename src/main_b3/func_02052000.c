#include <nitro/types.h>

typedef struct { u8 pad0[4]; s32 unk4; u8 pad8[4]; void *task0; void *task1; void *snd[5]; } UnkWork_02052000;
extern UnkWork_02052000 *data_022c465c;
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
void func_02052000(void) {
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
        data_022c465c->unk4 = 7;
    }
}
#pragma thumb reset
