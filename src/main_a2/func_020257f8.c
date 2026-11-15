#include <nitro/types.h>

typedef struct { u8 unk0[0x20]; u16 plttInfo; } UnkTex_020257f8;

extern u32 (*data_02077d40)(u32 size, BOOL is4x4, u32 opt);
extern u32 (*data_02077d48)(u32 size, u32 is4pltt, u32 opt);
extern void (*data_02077d4c)(u32 key);
extern void (*data_02077d44)(u32 key);

UnkTex_020257f8 *func_02025e8c(u32 *res);
void *func_02025ecc(u32 *res);
u32 func_02021c18(UnkTex_020257f8 *tex);
u32 func_02021c0c(UnkTex_020257f8 *tex);
u32 func_02021ad8(UnkTex_020257f8 *tex);
void func_02021bf8(UnkTex_020257f8 *tex, u32 key, u32 key4x4);
void func_02021ad0(UnkTex_020257f8 *tex, u32 key);
void func_02021b1c(UnkTex_020257f8 *tex, BOOL lock);
void func_02021a64(UnkTex_020257f8 *tex, BOOL lock);
void func_0202155c(void *mdlSet, UnkTex_020257f8 *tex);

BOOL func_020257f8(u32 *res) {
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
        tex = func_02025e8c(res);
        if (tex != NULL) {
            tex1 = func_02021c18(tex);
            tex4 = func_02021c0c(tex);
            pltt = func_02021ad8(tex);
            if (tex1 != 0) {
                tex1 = data_02077d40(tex1, FALSE, 0);
                if (tex1 == 0) okTex = FALSE;
            } else {
                tex1 = 0;
            }
            if (tex4 != 0) {
                tex4 = data_02077d40(tex4, TRUE, 0);
                if (tex4 == 0) okTex4x4 = FALSE;
            } else {
                tex4 = 0;
            }
            if (pltt != 0) {
                pltt = data_02077d48(pltt, tex->plttInfo & 0x8000, 0);
                if (pltt == 0) okPltt = FALSE;
            } else {
                pltt = 0;
            }
            if (!okTex || !okTex4x4 || !okPltt) {
                data_02077d4c(pltt);
                data_02077d44(tex4);
                data_02077d44(tex1);
                return FALSE;
            }
            func_02021bf8(tex, tex1, tex4);
            func_02021ad0(tex, pltt);
            func_02021b1c(tex, TRUE);
            func_02021a64(tex, TRUE);
        }
        if (*res == 0x30444D42) {
            void *mdlSet = func_02025ecc(res);
            if (tex != NULL) func_0202155c(mdlSet, tex);
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
