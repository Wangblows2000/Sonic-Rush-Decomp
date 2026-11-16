#include <nitro/types.h>

typedef struct UnkStruct_022dc20c {
    u8 pad[0x348];
    s32 unk348;
    u16 unk34C;
} UnkStruct_022dc20c;

void func_ov000_022dc1d4(UnkStruct_022dc20c *s);

void func_ov000_022dc20c(UnkStruct_022dc20c *s) {
    u16 t;
    u16 old = s->unk34C;
    if (s->unk348)
        t = 0x4000;
    else
        t = 0xc000;
    s->unk34C += 0x300;
    if (t > old && t <= s->unk34C) {
        s->unk34C = t;
        func_ov000_022dc1d4(s);
    }
}
