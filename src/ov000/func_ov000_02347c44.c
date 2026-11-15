#include <nitro/types.h>

typedef struct UnkStruct_02347c44 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02347c44;

void func_ov000_0234cb70(UnkStruct_02347c44 *s);
void func_ov000_0234c9e8(UnkStruct_02347c44 *s);
void func_ov000_02347c40(void);

void func_ov000_02347c44(UnkStruct_02347c44 *s) {
    func_ov000_0234cb70(s);
    func_ov000_0234c9e8(s);
    s->func = func_ov000_02347c40;
}
