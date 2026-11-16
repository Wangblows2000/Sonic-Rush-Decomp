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

extern UnkStruct_022b89e8 sMbpState;
extern u8 data_022b8a0e[];
extern u8 data_022b8a0f[];
extern u8 data_022b8a10[];
extern u8 data_022b8a11[];
extern u8 data_022b8a12[];
extern u8 data_022b8a13[];
extern u8 data_022b8a14[];
extern u8 sMbpChildInfo[];
extern u8 *sMbpFilebuf;
extern u8 *sMbpCWork;
extern void *gMainHeap;

u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 e);
void OS_Terminate(void);
s32 MBP_GetState(void);
void MBP_ChangeState(u16 v);
s32 MBP_GetChildState(void);
void MBP_KickChild(u32 n);
void MBP_AcceptChild(u32 n);
void *MB_CommGetChildUser(u32 n);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
void MB_End(void);
void MBP_StartDownload(u32 n);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

static inline void Free_0203b0ac(u8 *p) {
    if ((u8 *)OS_GetArenaLo(0) != p) {
        u32 size;
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
}

void MBP_ParentStateCallback(u32 n, u32 type, UnkArg_0203b0ac *arg) {
    switch (type) {
    case 2: {
        u32 e;
        s32 o;
        if (MBP_GetState() != 2)
            return;
        e = OS_DisableInterrupts();
        sMbpState.unk2 |= 1 << n;
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
        if (MBP_GetChildState() == 6)
            return;
        mask = ~(1 << n);
        e = OS_DisableInterrupts();
        sMbpState.unk2 &= mask;
        sMbpState.unk4 &= mask;
        sMbpState.unk6 &= mask;
        sMbpState.unk8 &= mask;
        sMbpState.unka &= mask;
        sMbpState.unkc &= mask;
        OS_RestoreInterrupts(e);
        break;
    }
    case 10: {
        void *p;
        if (MBP_GetState() != 2) {
            MBP_KickChild(n);
            return;
        }
        sMbpState.unk4 |= 1 << n;
        MBP_AcceptChild(n);
        p = MB_CommGetChildUser(n);
        if (p == NULL)
            return;
        MI_CpuCopy8(p, sMbpChildInfo + (n - 1) * 0x1e, 0x16);
        break;
    }
    case 14:
        sMbpState.unk4 &= ~(1 << n);
        sMbpState.unk6 |= 1 << n;
        MBP_StartDownload(n);
        break;
    case 7:
        sMbpState.unk8 &= ~(1 << n);
        sMbpState.unka |= 1 << n;
        break;
    case 9:
        sMbpState.unkc |= 1 << n;
        sMbpState.unka &= ~(1 << n);
        if (sMbpState.unk2 == sMbpState.unkc)
            MB_End();
        break;
    case 12:
        if (MBP_GetState() == 4)
            MBP_ChangeState(5);
        else
            MBP_ChangeState(0);
        if (sMbpFilebuf != NULL) {
            Free_0203b0ac(sMbpFilebuf);
            sMbpFilebuf = NULL;
        }
        if (sMbpCWork != NULL) {
            Free_0203b0ac(sMbpCWork);
            sMbpCWork = NULL;
        }
        break;
    case 13:
        switch (arg->unk0) {
        case 1:
        case 2:
        case 9:
            MBP_ChangeState(7);
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
