#include <nitro/types.h>

void MATHi_CRC32UpdateRev(const u32 *table, u32 *ctx, const u8 *data, u32 len) {
    u32 crc = *ctx;
    u32 i;
    for (i = 0; i < len; i++) { crc = table[(crc ^ *data) & 0xff] ^ (crc >> 8); data++; }
    *ctx = crc;
}
