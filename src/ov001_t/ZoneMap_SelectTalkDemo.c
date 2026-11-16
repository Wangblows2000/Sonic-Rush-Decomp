#include <nitro/types.h>

extern u16 gTalkDemoId;

#pragma thumb on
void ZoneMap_SelectTalkDemo(s32 v) {
    switch (v) {
    case 1:
        gTalkDemoId = 0x28;
        break;
    case 4:
        gTalkDemoId = 0x29;
        break;
    case 7:
        gTalkDemoId = 0x2a;
        break;
    case 15:
        gTalkDemoId = 0x2b;
        break;
    case 0x13:
        gTalkDemoId = 0x2c;
        break;
    case 0x17:
        gTalkDemoId = 0x2d;
        break;
    case 0x1b:
        gTalkDemoId = 0x2e;
        break;
    case 0x1e:
        gTalkDemoId = 0x2f;
        break;
    }
}
#pragma thumb reset
