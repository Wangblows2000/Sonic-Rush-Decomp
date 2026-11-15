#include <nitro/types.h>

typedef struct UnkStruct_02312c70 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02312c70;

void func_02069818(UnkStruct_02312c70 *s, s32 a);
void func_ov000_02312c20(void);

void func_ov000_02312c70(UnkStruct_02312c70 *s) {
    func_02069818(s, 2);
    s->func = func_ov000_02312c20;
}
