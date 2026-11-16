#include <nitro/types.h>

typedef struct { s32 x; s32 y; s32 z; s32 timer; } UnkWork_02057174;
typedef struct { u8 pad0[0x10]; UnkWork_02057174 *work; } UnkTask_02057174;
extern UnkTask_02057174 *data_022b4574;
extern void *data_022c467c;
extern u32 data_0208fe70;

void func_020371c8(UnkTask_02057174 *t);

#pragma thumb on
static inline u16 Rand_02057174(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_02057174(void) {
    UnkWork_02057174 *w = data_022b4574->work;

    if (data_022c467c == NULL) {
        func_020371c8(data_022b4574);
        return;
    }
    w->timer--;
    if (w->timer == 0) {
        func_020371c8(data_022b4574);
        return;
    }
    w->x = 0xfff - (Rand_02057174() & 0x1ffe);
    w->y = 0xfff - (Rand_02057174() & 0x1ffe);
    w->z = 0xfff - (Rand_02057174() & 0x1ffe);
}
#pragma thumb reset
