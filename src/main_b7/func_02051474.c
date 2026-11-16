#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[0x14];
    u16 unk1c;
    u8 pad1e[0x16];
    u16 unk34;
} UnkWork_02051474;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_02051474 *work;
} UnkTask_02051474;

extern UnkTask_02051474 *data_022c4638;
extern u16 gWirelessSession[];
extern u32 gWirelessRecvBuffers[];

#pragma thumb on
u32 func_02051474(void) {
    if (data_022c4638 == NULL) {
        switch (gWirelessSession[1]) {
        case 0:
        case 1:
            return gWirelessRecvBuffers[1];
        case 2:
        case 3:
            return gWirelessRecvBuffers[0];
        }
        return gWirelessRecvBuffers[1];
    } else {
        UnkWork_02051474 *w = data_022c4638->work;
        switch (w->unk4) {
        case 0:
        case 1:
            return gWirelessRecvBuffers[w->unk34];
        case 2:
        case 3:
            return gWirelessRecvBuffers[0];
        }
        return 0;
    }
}
#pragma thumb reset
