#include <nitro/types.h>

typedef struct UnkStruct_02339db8 {
    u8 pad[0xc8];
    s32 unkC8;
    s32 unkCC;
} UnkStruct_02339db8;

extern UnkStruct_02339db8 data_ov000_0236aad8;

void func_ov000_0233e668(void);

void func_ov000_02339db8(void) {
    data_ov000_0236aad8.unkCC = data_ov000_0236aad8.unkC8;
    func_ov000_0233e668();
}
