#include <nitro/types.h>

typedef struct UnkStruct_022f1aac {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022f1aac;

void func_ov000_022f9188(UnkStruct_022f1aac *s);
void func_ov000_022f19a0(void);

void func_ov000_022f1aac(UnkStruct_022f1aac *s) {
    func_ov000_022f9188(s);
    s->func = func_ov000_022f19a0;
}
