#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void func_0201c224(void *s, u32 a);
void func_02008d1c(u32 a, u32 b, u32 c, u32 d);

typedef struct { u8 unk0[0x24]; u32 unk24_0 : 1; s32 unk24_1 : 1; u32 pad : 30; u8 unk28[0x10]; u32 unk38; u8 unk3C[4]; u32 unk40; u32 unk44; } UnkStruct_0201c158;

void func_0201c158(UnkStruct_0201c158 *s) {
    if (!s->unk24_1) return;
    while (s->unk38 != 0) {
        u32 e = OS_DisableInterrupts();
        func_0201c224(s, 1);
        OS_RestoreInterrupts(e);
    }
    func_02008d1c(s->unk44, 0, 1 << s->unk40, 0);
}
