#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[0x14];
    u16 unk1c;
    u8 pad1e[0x16];
    u16 unk34;
} UnkWork_020513a8;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_020513a8 *work;
} UnkTask_020513a8;

extern UnkTask_020513a8 *data_022c4638;
extern u16 data_0207ad9c[];

#pragma thumb on
BOOL func_020513a8(void) {
    if (data_022c4638 == NULL) {
        switch (data_0207ad9c[1]) {
        case 0:
        case 2:
            return FALSE;
        case 1:
        case 3:
            return TRUE;
        }
        return FALSE;
    }
    switch (data_022c4638->work->unk4) {
    case 0:
    case 2:
        return FALSE;
    case 1:
    case 3:
        return TRUE;
    }
    return FALSE;
}
#pragma thumb reset
