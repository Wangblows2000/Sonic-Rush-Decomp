#include <nitro/types.h>

typedef struct {
    u8 unk0[0x11C];
    s32 unk11C;
    u8 unk120;
    u8 unk121[7];
    u32 unk128;
    u32 unk12C;
} UnkStruct_0201f858;

extern u8 data_02084798[];
extern u8 *data_02083f48;

void func_02006698(void *m);
void func_02006628(void *m);
void func_0201fbc0(UnkStruct_0201f858 *s);
void func_0201c6b4(void *s);

void func_0201f858(u32 id, u32 unused, UnkStruct_0201f858 *s) {
    if (id != s->unk128) return;
    func_02006698(data_02084798);
    if (data_02083f48 != NULL) func_02006698(data_02083f48 + 0x498);
    func_0201fbc0(s);
    s->unk128 = 0;
    s->unk12C = 0;
    s->unk120 = 0;
    if (s->unk11C > 0) {
        func_0201c6b4(s);
        s->unk11C = 0;
    }
    func_02006628(data_02084798);
    if (data_02083f48 != NULL) func_02006628(data_02083f48 + 0x498);
}
