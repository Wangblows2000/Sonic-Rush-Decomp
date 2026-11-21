#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkFile_0203c350;

typedef struct {
    u8 pad0[0xaf8];
    void *unkaf8;
} UnkData_0203c350;

typedef struct {
    s32 unk0;
    u8 pad4[8];
    s32 unkc;
    u32 pad10;
    void (*fn14)(void *a, s32 b, void *p);
    void *unk18;
    UnkFile_0203c350 *unk1c;
    void *unk20;
    s32 unk24;
    u8 pad28[8];
    u16 unk30;
    u8 pad32[0x40e];
    UnkData_0203c350 data;
} UnkCtx_0203c350;

extern UnkCtx_0203c350 data_022b8be0;
extern s32 data_022b8bc0;

u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 e);
void FS_NotifyArchiveAsyncEnd(u32 a, s32 b);
void WFSi_EndTaskThread(void);
void WBT_End(void);
void WM_SetPortCallback(u16 a, s32 b, s32 c);

static inline void Free_0203c350(UnkCtx_0203c350 *cp, void *p) {
    if (p != NULL) {
        u32 e = OS_DisableInterrupts();
        UnkCtx_0203c350 *ctx = cp;
        ctx->fn14(ctx->unk18, 0, p);
        OS_RestoreInterrupts(e);
    }
}

void WFS_End(void) {
    UnkCtx_0203c350 *c = &data_022b8be0;
    u32 e = OS_DisableInterrupts();

    if (data_022b8bc0 != 0) {
        if (c->unk20 != NULL) {
            Free_0203c350(&data_022b8be0, c->unk20);
            c->unk20 = NULL;
            c->unk24 = 0;
        }
        c->unkc = 0;
        if (data_022b8be0.unk0 == 0) {
            UnkData_0203c350 *d = &c->data;
            if (c->unk1c != NULL) {
                u32 v = c->unk1c->unk8;
                c->unk1c = NULL;
                FS_NotifyArchiveAsyncEnd(v, 5);
            }
            Free_0203c350(&data_022b8be0, d->unkaf8);
        } else {
            WFSi_EndTaskThread();
        }
        WBT_End();
        WM_SetPortCallback(data_022b8be0.unk30, 0, 0);
        data_022b8bc0 = 0;
    }
    OS_RestoreInterrupts(e);
}
