#include <nitro/types.h>

typedef struct { u8 unk0[0xEC]; u32 unkEC; u32 unkF0; u32 unkF4; } UnkStruct_02022148;
extern UnkStruct_02022148 data_02084d38;
extern u8 data_02084df4[];
extern u8 data_02084d84[];
extern u8 data_02084e68[];
extern u8 data_02084e98[];
void func_0200160c(void *a, void *b, void *c);
void func_02001bc0(void *a, void *b, u32 c, u32 d, u32 e);
void func_0200182c(void *a, void *b);

void func_02022148(void) {
    func_0200160c(data_02084df4, data_02084d84, data_02084e68);
    func_02001bc0(data_02084e68, data_02084e68, data_02084d38.unkEC, data_02084d38.unkF0, data_02084d38.unkF4);
    func_0200182c(data_02084e68, data_02084e98);
}
