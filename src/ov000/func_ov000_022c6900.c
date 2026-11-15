#include <nitro/types.h>

typedef struct UnkStruct_022c6900_2 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022c6900_2;

typedef struct UnkStruct_022c6900 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c6900;

extern UnkStruct_022c6900_2 *data_ov000_02369acc;

void func_ov000_022c68ac(void);

void func_ov000_022c6900(UnkStruct_022c6900 *s) {
    if (data_ov000_02369acc->unk20C & 1)
        s->func = func_ov000_022c68ac;
}
