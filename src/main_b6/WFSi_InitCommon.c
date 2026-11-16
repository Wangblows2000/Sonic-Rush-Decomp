#include <nitro/types.h>

typedef struct {
    u32 pad0;
    s32 x4;
    s32 x8;
    s32 xc;
    u32 x10;
    u32 x14;
    u32 x18;
    u32 x1c;
    u32 x20;
    u32 x24;
    u32 x28;
    u32 x2c;
    u16 h30;
} UnkCtx_0203c7dc;

extern UnkCtx_0203c7dc data_022b8be0;
extern s32 data_022b8bbc;

void FS_Init(u32 a);
s32 func_020114f0(u16 port, void (*cb)(void), void *arg);
void OS_Terminate(void);
void WFSi_PortCallback(void);

void WFSi_InitCommon(u32 id, u32 a, u32 b, u32 c) {
    UnkCtx_0203c7dc *x = (UnkCtx_0203c7dc *)((u32)&data_022b8be0 | 0);

    FS_Init(-1);
    x->h30 = id;
    x->x4 = 0;
    x->x8 = 0;
    x->xc = 0;
    x->x10 = a;
    x->x14 = b;
    x->x18 = c;
    x->x1c = 0;
    x->x24 = 0;
    x->x20 = 0;
    x->x28 = 0;
    x->x2c = 0xe;
    data_022b8bbc = 0;
    if (func_020114f0(id, WFSi_PortCallback, NULL) != 0)
        OS_Terminate();
}
