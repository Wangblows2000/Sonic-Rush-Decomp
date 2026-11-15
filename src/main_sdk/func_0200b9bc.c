#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
typedef struct { u8 pad[0x10]; u32 flags; } UnkStruct_0200b9bc_A;
typedef struct { u8 pad[8]; UnkStruct_0200b9bc_A *arc; vu32 flags; } UnkStruct_0200b9bc;
void func_0200b9bc(UnkStruct_0200b9bc *p) {
    u32 e = OS_DisableInterrupts();
    if ((p->flags & 1) ? 1 : 0) { p->flags |= 2; p->arc->flags |= 0x20; }
    OS_RestoreInterrupts(e);
}
