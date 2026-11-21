#include <nitro/types.h>

typedef volatile u64 vu64_;

typedef struct {
    u64 div_numer;
    u64 div_denom;
    u64 sqrt_param;
    u16 div_mode;
    u16 sqrt_mode;
} CPContext;

#define REG_DIV_NUMER  (*(vu64_ *)0x04000290)
#define REG_DIV_DENOM  (*(vu64_ *)0x04000298)
#define REG_DIVCNT     (*(vu16 *)0x04000280)
#define REG_SQRT_PARAM (*(vu64_ *)0x040002b8)
#define REG_SQRTCNT    (*(vu16 *)0x040002b0)

void CP_SaveContext(CPContext *ctx)
{
    ctx->div_numer = REG_DIV_NUMER;
    ctx->div_denom = REG_DIV_DENOM;
    ctx->div_mode = (u16)(REG_DIVCNT & 3);
    ctx->sqrt_param = REG_SQRT_PARAM;
    ctx->sqrt_mode = (u16)(REG_SQRTCNT & 1);
}
