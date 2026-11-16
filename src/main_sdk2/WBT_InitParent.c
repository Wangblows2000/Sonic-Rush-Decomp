#include <nitro/types.h>
#include <main_sdk2/mbctx.h>

extern u32 data_02080600;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void WBT_SetOwnAid(s32 a);
void WBT_Unk_0201557c(u32 a);
UnkMbCtx *WBT_Unk_02015640(void);
void WBT_Unk_0201559c(UnkMbParam *p);
void WBT_Unk_0201568c(void);
void WBT_Unk_0201564c(s32 a);
void WBT_Unk_0201565c(s32 a);

void WBT_InitParent(s32 a, s32 b, u32 arg)
{
    UnkMbParam param;
    UnkMbCtx *ctx;
    s32 i;
    u32 e = OS_DisableInterrupts();
    if (data_02080600 == 1) {
        OS_RestoreInterrupts(e);
        return;
    }
    data_02080600 = 1;
    WBT_SetOwnAid(0);
    WBT_Unk_0201557c(0);
    ctx = WBT_Unk_02015640();
    ctx->unk0 = 0;
    ctx->unk8 = 0;
    ctx->unkA = 0;
    ctx->unk10 = 0;
    ctx->unk138 = 0;
    ctx->unk140 = 0;
    ctx->unk142 = 0;
    ctx->unk148 = arg;
    for (i = 0; i < 16; i++) {
        ctx->entries[i].unk3 = 0;
    }
    param.type = 0;
    WBT_Unk_0201559c(&param);
    WBT_Unk_0201568c();
    WBT_Unk_0201564c(a - 14);
    WBT_Unk_0201565c(b - 14);
    OS_RestoreInterrupts(e);
}
