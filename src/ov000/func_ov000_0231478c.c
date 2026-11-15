#include <nitro/types.h>

typedef struct UnkStruct_0231478c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231478c;

void func_ov000_02314758(void);

void func_ov000_0231478c(UnkStruct_0231478c *s) {
    s->func = func_ov000_02314758;
}
