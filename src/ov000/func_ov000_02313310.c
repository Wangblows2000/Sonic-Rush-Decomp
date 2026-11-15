#include <nitro/types.h>

typedef struct UnkStruct_02313310 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02313310;

void func_ov000_0231301c(void);

void func_ov000_02313310(UnkStruct_02313310 *s) {
    s->func = func_ov000_0231301c;
}
