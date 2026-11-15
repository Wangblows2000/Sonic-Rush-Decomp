#include <nitro/types.h>

typedef struct UnkStruct_022c6924 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x2bc - 0x6c];
    u16 unk2bc;
} UnkStruct_022c6924;

void func_ov000_022c6900(void);

void func_ov000_022c6924(UnkStruct_022c6924 *s) {
    s->unk2bc = 0;
    s->func = func_ov000_022c6900;
}
