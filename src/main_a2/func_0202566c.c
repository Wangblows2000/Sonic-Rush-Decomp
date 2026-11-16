#include <nitro/types.h>

extern void (*NNS_GfdDefaultFuncFreePlttVram)(u32 key);
extern void (*NNS_GfdDefaultFuncFreeTexVram)(u32 key);

void *NNS_G3dGetMdlSet(u32 *res);
void *NNS_G3dGetTex(u32 *res);
void NNS_G3dReleaseMdlSet(void *mdlSet);
u32 NNS_G3dPlttReleasePlttKey(void *tex);
void NNS_G3dTexReleaseTexKey(void *tex, u32 *k1, u32 *k2);

void func_0202566c(u32 *res) {
    switch (*res) {
    case 0x30444D42: {
        void *mdlSet = NNS_G3dGetMdlSet(res);
        if (NNS_G3dGetTex(res) != NULL) NNS_G3dReleaseMdlSet(mdlSet);
    }
    case 0x30585442: {
        void *tex = NNS_G3dGetTex(res);
        if (tex != NULL) {
            u32 texKey = NNS_G3dPlttReleasePlttKey(tex);
            u32 k1;
            u32 k2;
            NNS_G3dTexReleaseTexKey(tex, &k1, &k2);
            if (texKey != 0) NNS_GfdDefaultFuncFreePlttVram(texKey);
            if (k2 != 0) NNS_GfdDefaultFuncFreeTexVram(k2);
            if (k1 != 0) NNS_GfdDefaultFuncFreeTexVram(k1);
        }
        break;
    }
    case 0x30415642:
    case 0x30414D42:
    case 0x30414342:
    case 0x30415442:
    case 0x30505442:
        break;
    }
}
