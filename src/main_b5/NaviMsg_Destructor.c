#include <nitro/types.h>

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

typedef struct {
    s32 mode;
    u8 pad4[4];
    u8 *buf8;
    u8 padc[4];
    u8 *buf10;
    u8 pad14[0xc0];
    u32 unkd4;
    u8 padd8[0x40];
    u8 unk118[4];
} UnkWork_020532b0;

typedef struct {
    u8 pad0[0x10];
    UnkWork_020532b0 *work;
} UnkTask_020532b0;

extern void *gMainHeap;
extern u32 gNaviMsgTask;

void NaviMsg_RestoreSubDisplay(BOOL a);
void NNS_SndHandleReleaseSeq(void *p);

static inline void Free_020532b0(u8 *p) {
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

#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

void NaviMsg_Destructor(UnkTask_020532b0 *t) {
    UnkWork_020532b0 *w = t->work;

    Free_020532b0(w->buf8);
    Free_020532b0(w->buf10);
    if (w->mode == 1)
        NaviMsg_RestoreSubDisplay((w->unkd4 & 0x1000000) ? TRUE : FALSE);
    NNS_SndHandleReleaseSeq(w->unk118);
    REG_DB_DISPCNT &= ~0x1f00;
    gNaviMsgTask = 0;
}
