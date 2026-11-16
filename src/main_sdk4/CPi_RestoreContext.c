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

void CPi_RestoreContext(const CPContext *ctx)
{
    REG_DIV_NUMER = ctx->div_numer;
    REG_DIV_DENOM = ctx->div_denom;
    REG_DIVCNT = ctx->div_mode;
    REG_SQRT_PARAM = ctx->sqrt_param;
    REG_SQRTCNT = ctx->sqrt_mode;
}
