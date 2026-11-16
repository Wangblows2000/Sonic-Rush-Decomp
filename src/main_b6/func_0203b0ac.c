#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unka;
    u16 unkc;
} UnkStruct_022b89e8;

typedef struct {
    u16 unk0;
    u8 pad2[8];
    u8 b[6];
} UnkArg_0203b0ac;

extern UnkStruct_022b89e8 data_022b89e8;
extern u8 data_022b8a0e[];
extern u8 data_022b8a0f[];
extern u8 data_022b8a10[];
extern u8 data_022b8a11[];
extern u8 data_022b8a12[];
extern u8 data_022b8a13[];
extern u8 data_022b8a14[];
extern u8 data_022b89f8[];
extern u8 *data_022b89e4;
extern u8 *data_022b89e0;
extern void *data_0208fe80;

u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 e);
void OS_Terminate(void);
s32 func_0203b08c(void);
void func_0203b09c(u16 v);
s32 func_0203af98(void);
void func_0203b828(u32 n);
void func_0203b8f4(u32 n);
void *func_02016ad0(u32 n);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
void func_020186f8(void);
void func_0203b75c(u32 n);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

static inline void Free_0203b0ac(u8 *p) {
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
    func_020349b8(data_0208fe80, p);
}

void func_0203b0ac(u32 n, u32 type, UnkArg_0203b0ac *arg) {
    switch (type) {
    case 2: {
        u32 e;
        s32 o;
        if (func_0203b08c() != 2)
            return;
        e = OS_DisableInterrupts();
        data_022b89e8.unk2 |= 1 << n;
        OS_RestoreInterrupts(e);
        o = (n - 1) * 0x1e;
        data_022b8a0e[o] = arg->b[0];
        data_022b8a0f[o] = arg->b[1];
        data_022b8a10[o] = arg->b[2];
        data_022b8a11[o] = arg->b[3];
        data_022b8a12[o] = arg->b[4];
        data_022b8a13[o] = arg->b[5];
        *(u16 *)(data_022b8a14 + o) = n;
        break;
    }
    case 3: {
        u32 e;
        u16 mask;
        if (func_0203af98() == 6)
            return;
        mask = ~(1 << n);
        e = OS_DisableInterrupts();
        data_022b89e8.unk2 &= mask;
        data_022b89e8.unk4 &= mask;
        data_022b89e8.unk6 &= mask;
        data_022b89e8.unk8 &= mask;
        data_022b89e8.unka &= mask;
        data_022b89e8.unkc &= mask;
        OS_RestoreInterrupts(e);
        break;
    }
    case 10: {
        void *p;
        if (func_0203b08c() != 2) {
            func_0203b828(n);
            return;
        }
        data_022b89e8.unk4 |= 1 << n;
        func_0203b8f4(n);
        p = func_02016ad0(n);
        if (p == NULL)
            return;
        MI_CpuCopy8(p, data_022b89f8 + (n - 1) * 0x1e, 0x16);
        break;
    }
    case 14:
        data_022b89e8.unk4 &= ~(1 << n);
        data_022b89e8.unk6 |= 1 << n;
        func_0203b75c(n);
        break;
    case 7:
        data_022b89e8.unk8 &= ~(1 << n);
        data_022b89e8.unka |= 1 << n;
        break;
    case 9:
        data_022b89e8.unkc |= 1 << n;
        data_022b89e8.unka &= ~(1 << n);
        if (data_022b89e8.unk2 == data_022b89e8.unkc)
            func_020186f8();
        break;
    case 12:
        if (func_0203b08c() == 4)
            func_0203b09c(5);
        else
            func_0203b09c(0);
        if (data_022b89e4 != NULL) {
            Free_0203b0ac(data_022b89e4);
            data_022b89e4 = NULL;
        }
        if (data_022b89e0 != NULL) {
            Free_0203b0ac(data_022b89e0);
            data_022b89e0 = NULL;
        }
        break;
    case 13:
        switch (arg->unk0) {
        case 1:
        case 2:
        case 9:
            func_0203b09c(7);
            break;
        case 8:
            break;
        }
        break;
    case 1:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 8:
        break;
    case 11:
        break;
    case 0:
    default:
        OS_Terminate();
        break;
    }
}
