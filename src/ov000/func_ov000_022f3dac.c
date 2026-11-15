#include <nitro/types.h>

typedef struct UnkStruct_022f3dac {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x510];
    u8 unk57C;
} UnkStruct_022f3dac;

void func_ov000_022f49f4(UnkStruct_022f3dac *s);
void func_ov000_022f3d48(void);

void func_ov000_022f3dac(UnkStruct_022f3dac *s) {
    func_ov000_022f49f4(s);
    if (s->unk57C & 2)
        s->func = func_ov000_022f3d48;
}
