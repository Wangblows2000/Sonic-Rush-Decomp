#include <nitro/types.h>

typedef struct {
    s32 a;
    s32 b;
} UnkPair_02046550;

typedef struct {
    void (*func)(void *);
    s32 timer;
    UnkPair_02046550 pairs[6];
    s16 scale[5];
    u8 pad42[0x42];
    s16 digits[6];
    s16 target[6];
} UnkSub_02046550;

typedef struct {
    u8 pad0[0x274];
    UnkSub_02046550 sub;
    u8 pad[0x344 - 0x274 - sizeof(UnkSub_02046550)];
    u32 flags;
    u8 pad348[0x1630 - 0x348];
    void *unk1630;
    u8 unk1634[4];
} UnkObj_02046550;

extern u32 data_0208fe70;
void func_02046490(void *);

#pragma thumb on
void func_02046550(UnkObj_02046550 *o, UnkSub_02046550 *s) {
    s32 i;
    s->timer++;
    if (s->timer & 1) {
        for (i = 0; i < 6; i++) {
            data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
            s->digits[i] = (u16)(data_0208fe70 >> 16) % 10;
        }
    }
    if (s->timer >= 0x2d) {
        s->func = func_02046490;
        s->timer = 0;
    }
}
#pragma thumb reset
