#include <nitro/types.h>

void Model_Animate(void *p);
void Model_Draw(void *p);

void func_ov001_022dedd4(void *a, u8 *b) {
    void *q = b + 0x5c;
    Model_Animate(q);
    Model_Draw(q);
}
