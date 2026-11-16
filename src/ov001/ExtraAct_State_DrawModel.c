#include <nitro/types.h>

extern u8 *data_ov001_0231f084;
void Model_Draw(void *p);

void ExtraAct_State_DrawModel(void) {
    Model_Draw(data_ov001_0231f084 + 0xc90);
}
