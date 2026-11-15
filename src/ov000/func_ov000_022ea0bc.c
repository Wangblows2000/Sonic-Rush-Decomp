#include <nitro/types.h>

typedef struct UnkStruct_022ea0bc {
    u8 pad[0x43c];
    void (*func)(void);
    u8 pad1[0x454 - 0x440];
    u16 unk454;
} UnkStruct_022ea0bc;

extern u32 data_0208fe70;

void func_ov000_022ea090(void);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

s32 func_ov000_022ea0bc(UnkStruct_022ea0bc *s) {
    s->unk454 = (Rand() & 0x1f) + 0xf;
    s->func = func_ov000_022ea090;
    return 0;
}
