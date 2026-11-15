#include <nitro/types.h>

typedef struct UnkStruct_022f18cc {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x510];
    u8 unk57C;
} UnkStruct_022f18cc;

void func_ov000_022f490c(UnkStruct_022f18cc *s);
void func_ov000_022f18b0(void);

void func_ov000_022f18cc(UnkStruct_022f18cc *s) {
    func_ov000_022f490c(s);
    if (s->unk57C & 2)
        s->func = func_ov000_022f18b0;
}
