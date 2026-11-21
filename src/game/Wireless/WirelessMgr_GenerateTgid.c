#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkc;
} UnkTime_02042774;

extern s32 gWirelessTgidCounter;
extern u16 gWirelessTgidSeeded;

void RTC_GetTime(UnkTime_02042774 *t);

u16 WirelessMgr_GenerateTgid(u16 v) {
    if (gWirelessTgidCounter == 0) {
        UnkTime_02042774 t;
        gWirelessTgidCounter = 1;
        RTC_GetTime(&t);
        gWirelessTgidSeeded = (t.unk8 & 0x1f) | ((t.unk4 & 0x1f) << 5);
    } else {
        gWirelessTgidSeeded = (gWirelessTgidSeeded + 1) & 0x3ff;
    }
    return (v & 0x1f) | ((gWirelessTgidSeeded & 0x3ff) << 5);
}
