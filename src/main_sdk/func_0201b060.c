#include <nitro/types.h>

void func_0201b060(const u32 *table, u32 *ctx, const u8 *data, u32 len) {
    u32 crc = *ctx;
    u32 i;
    for (i = 0; i < len; i++) { crc = table[(crc ^ *data) & 0xff] ^ (crc >> 8); data++; }
    *ctx = crc;
}
