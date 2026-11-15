#include <nitro/types.h>

void func_0201fac8(void *s);
void func_0200bad8(void *f);
void func_0201f9dc(void *list, void *s);
void func_0201ff9c(void *s);
extern u8 data_020847ac[];
extern u8 *data_02083f48;

typedef struct { u8 unk0[0x5C]; u8 file[0xB0]; s32 unk10C_0 : 1; } UnkStruct_0201fb50;

void func_0201fb50(UnkStruct_0201fb50 *s) {
    if (!s->unk10C_0) return;
    func_0201fac8(s);
    func_0200bad8(s->file);
    func_0201f9dc(data_020847ac, s);
    if (data_02083f48 != NULL) {
        func_0201f9dc(data_02083f48 + 0x4AC, s);
    }
    func_0201ff9c(s);
}
