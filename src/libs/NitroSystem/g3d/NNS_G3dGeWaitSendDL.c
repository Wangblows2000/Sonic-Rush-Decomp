#include <nitro/types.h>

extern volatile u32 data_02087ca4;

void NNS_G3dGeWaitSendDL(void) {
    while (data_02087ca4 != 0) {
    }
}
