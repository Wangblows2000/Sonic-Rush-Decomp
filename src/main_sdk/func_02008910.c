#include <nitro/types.h>

typedef struct { u32 next; u32 cmd; u32 a; u32 b; u32 c; u32 d; } UnkStruct_02008910;
UnkStruct_02008910 *func_02009484(u32 a);
void func_0200943c(UnkStruct_02008910 *p);
void func_02008910(u32 cmd, u32 a, u32 b, u32 c, u32 d) {
    UnkStruct_02008910 *p = func_02009484(1);
    if (p == 0) return;
    p->cmd = cmd; p->a = a; p->b = b; p->c = c; p->d = d;
    func_0200943c(p);
}
