#include <nitro/types.h>

void func_0201fac8(void *s);
void FS_CloseFile(void *f);
void RemoveCommandByPlayer(void *list, void *s);
void func_0201ff9c(void *s);
extern u8 data_020847ac[];
extern u8 *data_02083f48;

typedef struct { u8 unk0[0x5C]; u8 file[0xB0]; s32 unk10C_0 : 1; } UnkStruct_0201fb50;

void func_0201fb50(UnkStruct_0201fb50 *s) {
    if (!s->unk10C_0) return;
    func_0201fac8(s);
    FS_CloseFile(s->file);
    RemoveCommandByPlayer(data_020847ac, s);
    if (data_02083f48 != NULL) {
        RemoveCommandByPlayer(data_02083f48 + 0x4AC, s);
    }
    func_0201ff9c(s);
}
