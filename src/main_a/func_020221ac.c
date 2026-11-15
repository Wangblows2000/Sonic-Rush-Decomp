#include <nitro/types.h>

typedef struct { u8 unk0[0xFC]; u32 unkFC; } UnkStruct_020221ac;
extern UnkStruct_020221ac data_02084d38;
extern u8 data_02084d84[];
extern u8 data_02084e38[];
void func_0200182c(void *a, void *b);

void *func_020221ac(void) {
    if (!(data_02084d38.unkFC & 8)) {
        func_0200182c(data_02084d84, data_02084e38);
        data_02084d38.unkFC |= 8;
    }
    return data_02084e38;
}
