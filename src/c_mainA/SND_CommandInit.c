#include <nitro/types.h>

typedef struct UnkNode_02009654 {
    struct UnkNode_02009654 *next;
    u32 id;
    u32 arg[4];
} UnkNode_02009654;

extern UnkNode_02009654 data_0207d6c0[256];
extern UnkNode_02009654 *data_0207d3f4;
extern UnkNode_02009654 *data_0207d3fc;
extern UnkNode_02009654 *data_0207d400;
extern u32 data_0207d410;
extern u32 data_0207d408;
extern u32 data_0207d40c;
extern u32 data_0207d3f8;
extern UnkNode_02009654 *data_0207d404;
extern u32 data_0207d414;
extern u8 data_0207d440[];
extern void *data_0207ef20;

void InitPXI(void);
void SNDi_InitSharedWork(void *work);
UnkNode_02009654 *SND_AllocCommand(u32 a);
void SND_PushCommand(UnkNode_02009654 *n);
BOOL SND_FlushCommand(u32 a);

void SND_CommandInit(void)
{
    UnkNode_02009654 *command;
    int i;

    InitPXI();
    data_0207d3f4 = &data_0207d6c0[0];
    for (i = 0; i < 255; i++) {
        data_0207d6c0[i].next = &data_0207d6c0[i + 1];
    }
    data_0207d6c0[255].next = NULL;
    data_0207d404 = &data_0207d6c0[255];
    data_0207d3fc = NULL;
    data_0207d400 = NULL;
    data_0207d410 = 0;
    data_0207d408 = 0;
    data_0207d40c = 0;
    data_0207d414 = 1;
    data_0207d3f8 = 0;
    data_0207ef20 = data_0207d440;
    SNDi_InitSharedWork(data_0207ef20);
    command = SND_AllocCommand(1);
    if (command != NULL) {
        command->id = 0x1d;
        command->arg[0] = (u32)data_0207ef20;
        SND_PushCommand(command);
        SND_FlushCommand(1);
    }
}
