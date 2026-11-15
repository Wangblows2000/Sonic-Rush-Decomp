#include <nitro/types.h>

typedef struct { u8 pad0[0x34]; void *unk34; u8 pad38[0x10]; } UnkSpr_02063178;
typedef struct {
    s32 unk0;
    u8 pad4[0xc];
    UnkSpr_02063178 spr[4]; /* 0x10 */
    u8 bg[0x80];            /* 0x130 */
    u8 *buf[4];             /* 0x1b0 */
} UnkWork_02063178;
extern void *data_0208fe78;

void func_02039bb0(s32 a, void *b);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

#pragma thumb on
static inline void Free_02063178(u8 *blk) {
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
    func_020349b8(data_0208fe78, blk);
}

void func_02063178(UnkWork_02063178 *w) {
    s32 i;
    vu16 zero;

    if (w->unk0 != 0) {
        for (i = 0; i < 4; i++) {
            if (w->spr[i].unk34 != NULL) {
                func_02039bb0(0, w->spr[i].unk34);
                w->spr[i].unk34 = NULL;
            }
        }
        for (i = 0; i < 4; i++) {
            if (w->buf[i] != NULL) {
                Free_02063178(w->buf[i]);
                w->buf[i] = NULL;
            }
        }
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_02063178));
    }
}
#pragma thumb reset
