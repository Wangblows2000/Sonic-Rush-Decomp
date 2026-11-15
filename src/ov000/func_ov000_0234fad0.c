#include <nitro/types.h>

typedef struct UnkStruct_0234fad0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0234fad0;

void func_ov000_0234fa14(void);

void func_ov000_0234fad0(UnkStruct_0234fad0 *s) {
    s->func = func_ov000_0234fa14;
}
