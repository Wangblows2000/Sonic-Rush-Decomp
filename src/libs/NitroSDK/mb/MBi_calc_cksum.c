#include <nitro/types.h>

u16 MBi_calc_cksum(const u16 *p, s32 len) {
    u32 sum = 0;
    s32 n = len >> 1;
    while (n > 0) { sum += *p++; n--; }
    sum = (sum & 0xffff) + (sum >> 16);
    sum += sum >> 16;
    return (u16)(sum ^ 0xffff);
}
