#include <nitro/types.h>

extern u8 data_02080e74[];
void MBi_ClearSendStatus(void) { data_02080e74[0xd] = 0; data_02080e74[0xe] = 0; data_02080e74[0xf] = 0; data_02080e74[0x10] = 0; data_02080e74[0x11] = 0; }
