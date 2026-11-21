#include <nitro/types.h>

typedef struct { u8 unk0[0x20]; u16 plttInfo; } UnkTex_020257f8;

extern u32 (*NNS_GfdDefaultFuncAllocTexVram)(u32 size, BOOL is4x4, u32 opt);
extern u32 (*NNS_GfdDefaultFuncAllocPlttVram)(u32 size, u32 is4pltt, u32 opt);
extern void (*NNS_GfdDefaultFuncFreePlttVram)(u32 key);
extern void (*NNS_GfdDefaultFuncFreeTexVram)(u32 key);

UnkTex_020257f8 *NNS_G3dGetTex(u32 *res);
void *NNS_G3dGetMdlSet(u32 *res);
u32 NNS_G3dTexGetRequiredSize(UnkTex_020257f8 *tex);
u32 NNS_G3dTex4x4GetRequiredSize(UnkTex_020257f8 *tex);
u32 NNS_G3dPlttGetRequiredSize(UnkTex_020257f8 *tex);
void NNS_G3dTexSetTexKey(UnkTex_020257f8 *tex, u32 key, u32 key4x4);
void NNS_G3dPlttSetPlttKey(UnkTex_020257f8 *tex, u32 key);
void NNS_G3dTexLoad(UnkTex_020257f8 *tex, BOOL lock);
void NNS_G3dPlttLoad(UnkTex_020257f8 *tex, BOOL lock);
void NNS_G3dBindMdlSet(void *mdlSet, UnkTex_020257f8 *tex);

BOOL NNS_G3dResDefaultSetup(u32 *res) {
    switch (*res) {
    case 0x30444D42:
    case 0x30585442: {
        BOOL okTex;
        u32 tex1;
        UnkTex_020257f8 *tex;
        u32 tex4;
        u32 pltt;
        BOOL okTex4x4;
        BOOL okPltt;
        okTex = TRUE;
        okTex4x4 = TRUE;
        okPltt = TRUE;
        tex = NNS_G3dGetTex(res);
        if (tex != NULL) {
            tex1 = NNS_G3dTexGetRequiredSize(tex);
            tex4 = NNS_G3dTex4x4GetRequiredSize(tex);
            pltt = NNS_G3dPlttGetRequiredSize(tex);
            if (tex1 != 0) {
                tex1 = NNS_GfdDefaultFuncAllocTexVram(tex1, FALSE, 0);
                if (tex1 == 0) okTex = FALSE;
            } else {
                tex1 = 0;
            }
            if (tex4 != 0) {
                tex4 = NNS_GfdDefaultFuncAllocTexVram(tex4, TRUE, 0);
                if (tex4 == 0) okTex4x4 = FALSE;
            } else {
                tex4 = 0;
            }
            if (pltt != 0) {
                pltt = NNS_GfdDefaultFuncAllocPlttVram(pltt, tex->plttInfo & 0x8000, 0);
                if (pltt == 0) okPltt = FALSE;
            } else {
                pltt = 0;
            }
            if (!okTex || !okTex4x4 || !okPltt) {
                NNS_GfdDefaultFuncFreePlttVram(pltt);
                NNS_GfdDefaultFuncFreeTexVram(tex4);
                NNS_GfdDefaultFuncFreeTexVram(tex1);
                return FALSE;
            }
            NNS_G3dTexSetTexKey(tex, tex1, tex4);
            NNS_G3dPlttSetPlttKey(tex, pltt);
            NNS_G3dTexLoad(tex, TRUE);
            NNS_G3dPlttLoad(tex, TRUE);
        }
        if (*res == 0x30444D42) {
            void *mdlSet = NNS_G3dGetMdlSet(res);
            if (tex != NULL) NNS_G3dBindMdlSet(mdlSet, tex);
        }
        return TRUE;
    }
    case 0x30415642:
    case 0x30414D42:
    case 0x30414342:
    case 0x30415442:
    case 0x30505442:
        return TRUE;
    default:
        return FALSE;
    }
}
