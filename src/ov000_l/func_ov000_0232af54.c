#include <nitro/types.h>

typedef struct {
    u8 pad0[0x134];
    s32 unk134[5];
} UnkWork_0232af54;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0232af54 *work;
} UnkTask_0232af54;

extern UnkTask_0232af54 *data_022b4574;

void func_ov000_0232ad98(UnkWork_0232af54 *w, s32 a, s32 b, s32 c, u16 i);

void func_ov000_0232af54(void) {
    u16 i;
    UnkWork_0232af54 *w = data_022b4574->work;

    for (i = 0; i < 5; i++)
        func_ov000_0232ad98(w, 0x800, i * 0x1000 + 0x7c00, w->unk134[i], i);
}
