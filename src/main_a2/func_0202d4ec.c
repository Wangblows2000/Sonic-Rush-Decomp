#include <nitro/types.h>

typedef struct { u8 unk0[4]; u8 len; u8 digits[1]; } UnkStruct_0202d4ec;

s32 func_0202d4ec(UnkStruct_0202d4ec *s, s32 n) {
    u8 *p = &s->digits[n];
    u8 *end;
    if (*p > 5) return 1;
    if (*p < 5) return -1;
    end = s->digits + s->len;
    for (p++; p < end; p++) {
        if (*p != 0) return 1;
    }
    if (s->digits[(u32)(n - 1)] & 1) return 1;
    return -1;
}
