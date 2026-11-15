#include <nitro/types.h>

typedef struct { u8 pad[8]; void *a[5]; void *b[10]; } UnkStruct_0231f530;
extern UnkStruct_0231f530 *data_ov001_0231f530;
void func_ov001_02301118(void *p);

void func_ov001_02300fe8(void) {
    s32 i;
    for (i = 0; i < 10; i++) {
        if (data_ov001_0231f530->b[i] != NULL) {
            func_ov001_02301118(data_ov001_0231f530->b[i]);
        }
    }
    for (i = 0; i < 5; i++) {
        if (data_ov001_0231f530->a[i] != NULL) {
            func_ov001_02301118(data_ov001_0231f530->a[i]);
        }
    }
}
