#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 src;   /* 0x18 */
    u32 dst;   /* 0x1c */
    u32 len;   /* 0x20 */
    u32 dma;   /* 0x24 */
} UnkCardCommon_02010018;

typedef struct {
    u32 unk0;
    u32 ctrl;  /* 0x04 */
} UnkCardInfo_02010018;

extern UnkCardCommon_02010018 data_0207f2c0;

u32 func_02006c7c(void);
u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void DC_StoreRange(void *p, u32 size);
void DC_InvalidateRange(void *p, u32 size);
void IC_InvalidateRange(void *p, u32 size);
void func_02005520(u32 mask, void (*fn)(void));
u32 OS_ResetRequestIrqMask(u32 mask);
u32 OS_EnableIrqMask(u32 mask);
void DC_WaitWriteBufferEmpty(void);
void func_0201016c(void);
void func_020101e0(void);

static inline BOOL func_02010018_IsInTcm(u32 buf, u32 len)
{
    const u32 i = 0x01ff8000;
    const u32 d = func_02006c7c();
    return ((i < buf + len) && (i + 0x8000 > buf)) || ((d < buf + len) && (d + 0x4000 > buf));
}

BOOL func_02010018(UnkCardInfo_02010018 *p)
{
    UnkCardCommon_02010018 *c;
    u32 len;
    BOOL isReady;
    BOOL isSizeAligned;
    BOOL isAccessible;
    u32 dst;
    BOOL isAligned;
    c = &data_0207f2c0;
    isReady = FALSE;
    isSizeAligned = FALSE;
    isAccessible = FALSE;
    isAligned = FALSE;
    dst = c->dst;

    if (!(dst & 3) && c->dma <= 3) {
        isAligned = TRUE;
    }
    if (isAligned && !func_02010018_IsInTcm(dst, len = c->len)) {
        isAccessible = TRUE;
    }
    if (isAccessible && !((c->src | c->len) & 0x1ff)) {
        isSizeAligned = TRUE;
    }
    if (isSizeAligned && c->len != 0) {
        isReady = TRUE;
    }
    p->ctrl = (*(u32 *)0x027ffe60 & ~0x07000000) | 0xa1000000;
    if (isReady) {
        u32 enabled = OS_DisableInterrupts();
        DC_StoreRange((void *)c->dst, c->len);
        DC_InvalidateRange((void *)c->dst, c->len);
        IC_InvalidateRange((void *)c->dst, c->len);
        func_02005520(0x80000, func_0201016c);
        OS_ResetRequestIrqMask(0x80000);
        OS_EnableIrqMask(0x80000);
        DC_WaitWriteBufferEmpty();
        OS_RestoreInterrupts(enabled);
        func_020101e0();
    }
    return isReady;
}
