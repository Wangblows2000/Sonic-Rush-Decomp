#include <nitro/types.h>

typedef struct UnkStruct_02313ec8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02313ec8;

void func_02069818(UnkStruct_02313ec8 *s, s32 a);
void func_ov000_02313ec4(void);

void func_ov000_02313ec8(UnkStruct_02313ec8 *s) {
    func_02069818(s, 0);
    s->func = func_ov000_02313ec4;
}
