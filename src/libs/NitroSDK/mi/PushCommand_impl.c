#include <nitro/types.h>

typedef struct { u32 next; u32 cmd; u32 a; u32 b; u32 c; u32 d; } UnkStruct_02008910;
UnkStruct_02008910 *SND_AllocCommand(u32 a);
void SND_PushCommand(UnkStruct_02008910 *p);
void PushCommand_impl(u32 cmd, u32 a, u32 b, u32 c, u32 d) {
    UnkStruct_02008910 *p = SND_AllocCommand(1);
    if (p == 0) return;
    p->cmd = cmd; p->a = a; p->b = b; p->c = c; p->d = d;
    SND_PushCommand(p);
}
