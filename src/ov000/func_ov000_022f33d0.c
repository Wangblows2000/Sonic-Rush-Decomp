#include <nitro/types.h>

typedef struct UnkStruct_022f33d0 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x510];
    u8 unk57C;
} UnkStruct_022f33d0;

void func_ov000_022f4954(UnkStruct_022f33d0 *s);
void func_ov000_022f338c(void);

void func_ov000_022f33d0(UnkStruct_022f33d0 *s) {
    func_ov000_022f4954(s);
    if (s->unk57C & 2)
        s->func = func_ov000_022f338c;
}
