#include <nitro/types.h>

typedef struct UnkStruct_0231394c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231394c;

void func_02069818(UnkStruct_0231394c *s, s32 a);
void func_ov000_02313948(void);

void func_ov000_0231394c(UnkStruct_0231394c *s) {
    func_02069818(s, 0);
    s->func = func_ov000_02313948;
}
