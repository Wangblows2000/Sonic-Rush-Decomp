#include <nitro/types.h>

typedef struct { u8 pad[4]; u8 *p; void *x8; u8 xc; u8 pad2[3]; u8 x10; } UnkStruct_02080e74;
extern UnkStruct_02080e74 data_02080e74;
void func_02017e74(void) {
    u8 *p;
    if (data_02080e74.xc != 2) return;
    p = data_02080e74.p;
    if (p[0x4b2] == 0) { data_02080e74.x10 = 9; data_02080e74.x8 = p; }
    else { data_02080e74.x10 = 4; data_02080e74.x8 = p + 0x220; }
    data_02080e74.xc = 3;
}
