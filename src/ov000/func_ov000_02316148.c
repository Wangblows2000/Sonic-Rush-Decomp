#include <nitro/types.h>

typedef struct UnkStruct_02316148 {
    u8 pad[0x1c];
    s32 unk1C;
} UnkStruct_02316148;

extern UnkStruct_02316148 data_0208922c;
extern UnkStruct_02316148 data_02087d04;

void func_0206bef4(void *p);

void func_ov000_02316148(void *p) {
    data_0208922c.unk1C = 0;
    data_02087d04.unk1C = 0;
    func_0206bef4(p);
}
