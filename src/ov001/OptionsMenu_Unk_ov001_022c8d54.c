#include <nitro/types.h>

s32 WirelessLobby_GetChildCount(void *p);

#pragma thumb on
s32 OptionsMenu_Unk_ov001_022c8d54(u8 *p) {
    if (*(s32 *)(p + 0x10) == 0) {
        return WirelessLobby_GetChildCount(p);
    }
    return *(s32 *)(p + 8);
}
#pragma thumb reset
