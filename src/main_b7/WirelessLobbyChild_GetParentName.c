#include <nitro/types.h>

typedef struct {
    u16 name[10];
    u16 len;
} UnkName_02050f04;

typedef struct {
    u8 pad0[0x34];
    u16 count;
    u8 pad36[2];
    UnkName_02050f04 names[1];
} UnkWork_02050f04;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_02050f04 *work;
} UnkTask_02050f04;

extern UnkTask_02050f04 *gWirelessLobbyTask;
void MIi_CpuCopy16(const void *src, void *dst, u32 size);

#pragma thumb on
s32 WirelessLobbyChild_GetParentName(s32 idx, u16 *dst, s32 len) {
    UnkWork_02050f04 *w = gWirelessLobbyTask->work;
    if (len > 10)
        len = 10;
    if (idx >= w->count) {
        s32 i;
        for (i = 0; i < len; i++)
            dst[i] = 0x20;
        return 0;
    }
    MIi_CpuCopy16(w->names[idx].name, dst, len * 2);
    if (len >= w->names[idx].len)
        len = w->names[idx].len;
    return len;
}
#pragma thumb reset
