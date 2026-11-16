#include <nitro/types.h>

typedef struct {
    s32 idx;
    u8 sub[0x1c];
    s32 unk90;
    s32 unk94;
    s32 unk98;
    u8 pad9c[8];
} UnkEnt_022dab54;

typedef struct {
    u8 pad0[0x70];
    UnkEnt_022dab54 ent[8];
} UnkStruct_022dab54;

void WirelessLobbyChild_RefreshParentList(void);
s32 WirelessLobbyChild_GetParentCount(void);
s32 WirelessLobbyChild_GetParentLinkLevel(s32 i);
s32 WirelessLobbyChild_Unk_02050ec0(s32 i);
s32 WirelessLobbyChild_GetParentName(s32 i, u8 *buf, s32 n);
void VsMenu_BuildGlyphString(void *p, u8 *buf, u16 n);
void VsMenu_ClearNameSlot(UnkStruct_022dab54 *p, s32 i);

#pragma thumb on
BOOL VsMenu_RefreshParentList(UnkStruct_022dab54 *p) {
    s32 n;
    s32 i;
    u8 buf[20];

    WirelessLobbyChild_RefreshParentList();
    n = WirelessLobbyChild_GetParentCount();
    for (i = 0; i < 8; i++) {
        if (i < n) {
            p->ent[i].unk90 = WirelessLobbyChild_GetParentLinkLevel(i);
            p->ent[i].unk98 = WirelessLobbyChild_Unk_02050ec0(i);
            p->ent[i].unk94 = 2;
            p->ent[i].idx = i;
            VsMenu_BuildGlyphString(p->ent[i].sub, buf, WirelessLobbyChild_GetParentName(i, buf, 7));
        } else {
            p->ent[i].unk90 = 0;
            p->ent[i].unk98 = 0;
            p->ent[i].unk94 = 2;
            p->ent[i].idx = i;
            VsMenu_ClearNameSlot(p, i);
        }
    }
    return TRUE;
}
#pragma thumb reset
