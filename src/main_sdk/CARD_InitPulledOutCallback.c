#include <nitro/types.h>

void PXI_Init(void);
void PXI_SetFifoRecvCallback(u32 tag, void (*cb)(void));
void CARDi_PulledOutCallback(void);
extern u32 data_0207fae4;
void CARD_InitPulledOutCallback(void) { PXI_Init(); PXI_SetFifoRecvCallback(0xe, CARDi_PulledOutCallback); data_0207fae4 = 0; }
