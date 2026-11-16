#include <nitro/types.h>

typedef struct {
    void *ent[16];
    u32 arr40[16];
    u32 arr80[16];
    u8 bufc0[0xa38];
    u32 unkaf8;
    u32 padafc;
    u32 unkb00;
    u32 unkb04;
} UnkData_0203c4e8;

typedef struct {
    s32 unk0;
    u8 pad4[8];
    s32 unkc;
} UnkCtx_0203c4e8;

extern s32 data_022b8bc0;
extern UnkCtx_0203c4e8 data_022b8be0;

u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 e);
void func_0203c7dc(u32 a, u32 b, u32 c, u32 d);
void DC_FlushRange(void *p, u32 size);
void DC_WaitWriteBufferEmpty(void);
void func_02014c70(void (*cb)(void));
void func_0203c870(void);

void WFS_InitChild(u32 a, u32 b, u32 c, u32 d4) {
    u32 e = OS_DisableInterrupts();
    UnkCtx_0203c4e8 *ctx;
    UnkData_0203c4e8 *d;
    s32 i;
    u8 *p;

    if (data_022b8bc0 != 0)
        return;
    data_022b8bc0 = 1;
    func_0203c7dc(a, b, c, d4);
    OS_RestoreInterrupts(e);
    ctx = &data_022b8be0;
    d = (UnkData_0203c4e8 *)((u32)ctx + 0x440);
    ctx->unk0 = 0;
    DC_FlushRange(d, sizeof(UnkData_0203c4e8));
    DC_WaitWriteBufferEmpty();
    p = d->bufc0;
    for (i = 0; i < 16; i++) {
        d->ent[i] = p;
        d->arr40[i] = 0;
        d->arr80[i] = 0;
        p += 0x28;
    }
    d->unkaf8 = 0;
    d->unkb00 = 0;
    d->unkb04 = 0;
    DC_FlushRange(d, sizeof(UnkData_0203c4e8));
    DC_WaitWriteBufferEmpty();
    func_02014c70(func_0203c870);
    ctx->unkc = 1;
}
