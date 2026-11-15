#include <nitro/types.h>

typedef struct { u8 unk0[0xFC]; u32 unkFC; } UnkStruct_020220c0;
extern UnkStruct_020220c0 data_02084d38;
extern u8 data_02084e98[];
void func_02022148(void);

void *func_020220c0(void) {
    if (!(data_02084d38.unkFC & 0x80)) {
        func_02022148();
        data_02084d38.unkFC |= 0x80;
    }
    return data_02084e98;
}
