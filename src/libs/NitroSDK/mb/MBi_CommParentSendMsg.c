#include <nitro/types.h>

extern u8 *data_02080f1c;
void MBi_MakeParentSendBuffer(void *p, void *q);
void MBi_BlockHeaderEnd(u32 a, u32 b, void *c);
void MBi_CommParentSendMsg(u8 v, u32 b) { u8 t[8]; t[0] = v; MBi_MakeParentSendBuffer(t, data_02080f1c); MBi_BlockHeaderEnd(6, b, data_02080f1c); }
