#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} UnkRect_0200d714;

typedef struct {
    u8 pad0[0x18];
    s32 x;        /* 0x18 */
    s32 w;        /* 0x1c */
    s32 invW;     /* 0x20 */
    s32 y;        /* 0x24 */
    s32 h;        /* 0x28 */
    s32 invH;     /* 0x2c */
    u16 enable;   /* 0x30 */
} UnkWork_0200d714;

extern UnkWork_0200d714 data_0207f02c;
extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);

#define REG_DIVCNT (*(vu16 *)0x04000280)
#define REG_DIV_NUMER_L (*(vu32 *)0x04000290)
#define REG_DIV_DENOM (*(volatile u64 *)0x04000298)
#define REG_DIV_RESULT_L (*(vu32 *)0x040002a0)

static inline void SetDiv32_32(u32 numer, u32 denom)
{
    REG_DIVCNT = 0;
    REG_DIV_NUMER_L = numer;
    REG_DIV_DENOM = denom;
}

static inline s32 GetDivResult32(void)
{
    while (REG_DIVCNT & 0x8000) {
    }
    return (s32)REG_DIV_RESULT_L;
}

void TP_SetCalibrateParam(const UnkRect_0200d714 *rect)
{
    u32 state;
    if (rect == NULL) {
        data_0207f02c.enable = 0;
        return;
    }
    state = OS_DisableInterrupts();
    if (rect->w != 0) {
        SetDiv32_32(0x10000000, (u32)rect->w);
        data_0207f02c.x = rect->x;
        data_0207f02c.w = rect->w;
        data_0207f02c.invW = GetDivResult32();
    } else {
        data_0207f02c.x = 0;
        data_0207f02c.w = 0;
        data_0207f02c.invW = 0;
    }
    if (rect->h != 0) {
        SetDiv32_32(0x10000000, (u32)rect->h);
        data_0207f02c.y = rect->y;
        data_0207f02c.h = rect->h;
        data_0207f02c.invH = GetDivResult32();
    } else {
        data_0207f02c.y = 0;
        data_0207f02c.h = 0;
        data_0207f02c.invH = 0;
    }
    OS_RestoreInterrupts(state);
    data_0207f02c.enable = 1;
}
