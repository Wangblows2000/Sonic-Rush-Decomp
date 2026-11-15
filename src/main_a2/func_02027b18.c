#include <nitro/types.h>

u16 func_02027b18(const u8 *anm, u32 info, u32 frame) {
    const u8 *data;
    u32 last;
    if (info & 0x20000000) return (u16)info;
    data = anm + (info & 0xFFFF);
    if (!(info & 0xC0000000)) return data[frame];
    last = (info & 0x1FFF0000) >> 16;
    if (info & 0x40000000) {
        if (frame & 1) {
            if (frame > last) return data[(last >> 1) + 1];
            return (u16)((data[frame >> 1] + data[(frame >> 1) + 1]) >> 1);
        }
        return data[frame >> 1];
    } else {
        if (frame & 3) {
            if (frame > last) return data[(last >> 2) + (frame & 3)];
            if (frame & 1) {
                u32 i1;
                if (frame & 2) {
                    i1 = frame >> 2;
                    frame = i1 + 1;
                } else {
                    frame = frame >> 2;
                    i1 = frame + 1;
                }
                frame = data[frame];
                i1 = data[i1];
                return (u16)((frame * 3 + i1) >> 2);
            }
            return (u16)((data[frame >> 2] + data[(frame >> 2) + 1]) >> 1);
        }
        return data[frame >> 2];
    }
}
