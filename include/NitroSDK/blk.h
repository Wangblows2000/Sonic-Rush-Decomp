#ifndef MAIN_SDK2_BLK_H
#define MAIN_SDK2_BLK_H
#include <nitro/types.h>

typedef struct {
    u32 base;
    u32 size;
    u8 *ptr;
    u32 state;
} UnkBlock_0201a16c;

typedef struct {
    u32 unk0;
    u8 pad4[0x2c];
    UnkBlock_0201a16c blocks[4]; /* 0x30 */
} UnkStruct_0201a16c;
#endif
