#ifndef MAIN_SDK2_BLK2_H
#define MAIN_SDK2_BLK2_H
#include <nitro/types.h>

typedef struct {
    u8 data[15][32];
    u32 recvMask[15]; /* 0x1e0 */
} UnkStruct_02080f44;

typedef struct {
    u32 size;
    s32 count;
} UnkStruct_02080f48;

extern UnkStruct_02080f44 *data_02080f44;
extern UnkStruct_02080f48 data_02080f48;
#endif

#ifndef MAIN_SDK2_BLK2_PKT
#define MAIN_SDK2_BLK2_PKT
typedef struct {
    u8 type;
    u8 pad1;
    union {
        struct {
            u8 seq;
            u8 data[0x11];
        } s;
        struct {
            u16 val;
            u16 val2;
        } h;
    } u;
} UnkPacket_0201a770;
#endif
