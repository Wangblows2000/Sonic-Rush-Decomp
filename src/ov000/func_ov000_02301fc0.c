#include <nitro/types.h>

typedef struct UnkStruct_02301fc0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02301fc0;

void func_02069818(UnkStruct_02301fc0 *s, s32 a);
void func_ov000_02301e54(void);

void func_ov000_02301fc0(UnkStruct_02301fc0 *s) {
    func_02069818(s, 0);
    s->func = func_ov000_02301e54;
}
