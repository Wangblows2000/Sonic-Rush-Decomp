#include <nitro/types.h>

typedef struct UnkStruct_0231bf68 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_0231bf68;

void func_ov000_0231bef8(UnkStruct_0231bf68 *s);

void func_ov000_0231bf68(UnkStruct_0231bf68 *s) {
    s->unkC--;
    if (s->unkC != 0)
        return;
    func_ov000_0231bef8(s);
}
