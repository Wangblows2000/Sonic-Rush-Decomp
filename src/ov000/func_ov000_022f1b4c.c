#include <nitro/types.h>

typedef struct UnkStruct_022f1b4c {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x510];
    u8 unk57C;
} UnkStruct_022f1b4c;

void func_ov000_022f89a8(UnkStruct_022f1b4c *s);
void func_ov000_022f1b20(void);

void func_ov000_022f1b4c(UnkStruct_022f1b4c *s) {
    func_ov000_022f89a8(s);
    if (s->unk57C & 2)
        s->func = func_ov000_022f1b20;
}
