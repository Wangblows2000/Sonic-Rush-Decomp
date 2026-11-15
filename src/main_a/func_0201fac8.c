#include <nitro/types.h>

void func_0201c6b4(void *p);

typedef struct { u8 unk0[0x11C]; u32 unk11C; } UnkStruct_0201fac8;

void func_0201fac8(UnkStruct_0201fac8 *s) {
    if (s->unk11C == 0) return;
    s->unk11C--;
    if (s->unk11C != 0) return;
    func_0201c6b4(s);
}
