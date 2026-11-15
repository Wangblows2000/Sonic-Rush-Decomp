#include <nitro/types.h>

typedef struct { s32 unk0; s32 unk4; s32 unk8; void *task0; void *task1; void *snd[5]; s32 unk28; s32 unk2C; u8 pad30[0x10]; } UnkWork_020522f4;
extern UnkWork_020522f4 *data_022c465c;
extern void *data_0208fe80;
extern u8 data_0225044c;
extern u8 data_02250444;
extern u8 data_02250448;
extern u8 data_0207b02c[];

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_0201bf34(void *p);
void func_02036854(void *p);
void func_0201be98(void *p, u8 v);

#pragma thumb on
static inline u8 *Alloc_020522f4(u32 size) {
    u8 *p = func_02034a40(data_0208fe80, size, 0x20);
    if ((u8 *)func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

void func_020522f4(void) {
    if (data_022c465c == NULL) {
        data_022c465c = (UnkWork_020522f4 *)Alloc_020522f4(0x60);
        data_022c465c->unk0 = 5;
        data_022c465c->unk4 = 7;
        data_022c465c->unk8 = 7;
        data_022c465c->task0 = NULL;
        data_022c465c->task1 = NULL;
        func_0201bf34(&data_022c465c->snd[0]);
        func_0201bf34(&data_022c465c->snd[1]);
        func_0201bf34(&data_022c465c->snd[2]);
        func_0201bf34(&data_022c465c->snd[3]);
        func_0201bf34(&data_022c465c->snd[4]);
        data_022c465c->unk28 = 0;
        data_022c465c->unk2C = 0;
        func_02036854(data_0207b02c);
        data_0225044c = 0x7f;
        data_02250444 = 0x7f;
        data_02250448 = 0x7f;
        func_0201be98(&data_022c465c->snd[0], 0x7f);
        func_0201be98(&data_022c465c->snd[1], data_0225044c);
        func_0201be98(&data_022c465c->snd[2], data_02250444);
        func_0201be98(&data_022c465c->snd[3], data_02250448);
        func_0201be98(&data_022c465c->snd[4], data_02250448);
    }
}
#pragma thumb reset
