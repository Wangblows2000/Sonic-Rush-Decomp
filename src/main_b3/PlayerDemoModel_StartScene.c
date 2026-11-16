#include <nitro/types.h>

typedef struct { u8 pad0[4]; void **names; s32 count; } UnkEnt_02056908;
typedef struct UnkWork_02056908 {
    void (*fn)(struct UnkWork_02056908 *w);
    u8 pad4[5];
    s8 unk9;
    u8 padA[0xa];
    UnkEnt_02056908 *tbl;   /* 0x14 */
    UnkEnt_02056908 *ent;   /* 0x18 */
    void **names;           /* 0x1c */
    s8 count;               /* 0x20 */
    u8 pad21[0xab];
    u8 anm0[0x138];         /* 0xcc */
    u8 anm1[0x1130];        /* 0x204 */
    u8 unk1334[4];          /* 0x1334 */
} UnkWork_02056908;
extern u16 data_022c4674;

void func_02056140(UnkWork_02056908 *w, void *p);
void PlayerDemoModel_LoadScene(UnkWork_02056908 *w);
void NNS_G3dRenderObjSetCallBack(void *p, void (*cb)(void), s32 a, s32 b, s32 c);
void PlayerDemoModel_OnCameraNode(void);
void PlayerDemoModel_UpdateCamera(UnkWork_02056908 *w);
void PlayerDemoModel_State_Play(UnkWork_02056908 *w);

#pragma thumb on
void PlayerDemoModel_StartScene(UnkWork_02056908 *w) {
    func_02056140(w, w->unk1334);
    w->ent = &w->tbl[w->unk9];
    w->count = w->ent->count;
    w->names = w->ent->names;
    PlayerDemoModel_LoadScene(w);
    data_022c4674 = 0;
    NNS_G3dRenderObjSetCallBack(w->anm0, PlayerDemoModel_OnCameraNode, 0, 6, 3);
    NNS_G3dRenderObjSetCallBack(w->anm1, PlayerDemoModel_OnCameraNode, 0, 6, 3);
    PlayerDemoModel_UpdateCamera(w);
    w->fn = PlayerDemoModel_State_Play;
}
#pragma thumb reset
