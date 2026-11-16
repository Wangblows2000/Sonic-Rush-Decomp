#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
    u8 pad4[4];
    u16 unk8;
    u8 pada[6];
    u16 unk10;
} UnkRes_02040048;

extern void (*data_022c197c)(const char *fmt, ...);
extern BOOL (*data_022c1978)(UnkRes_02040048 *r);
extern s32 data_022c196c;
extern u16 sConnectBitmap;
extern char data_02078a60[];
extern char data_02078a8c[];
extern char data_02078ab8[];

void WH_SetError(s32 r);
void WH_ChangeSysState(s32 a);
s32 func_02011598(s32 a, u16 b);
void WH_CallbackForWFS(UnkRes_02040048 *r);
BOOL WH_StateInStartParentMP(void);

void WH_StateOutStartParent(UnkRes_02040048 *r) {
    u16 bit = 1 << r->unk10;
    s32 ret;

    if (r->unk2 != 0) {
        WH_SetError(r->unk2);
        WH_ChangeSysState(9);
        return;
    }
    switch (r->unk8) {
    case 7:
        if (data_022c197c != NULL)
            data_022c197c(data_02078a60, r->unk10);
        if (data_022c1978 != NULL && !data_022c1978(r)) {
            ret = func_02011598(0, r->unk10);
            if (ret == 2)
                return;
            WH_SetError(ret);
            WH_ChangeSysState(9);
            return;
        }
        if (data_022c196c == 6 || data_022c196c == 7)
            WH_CallbackForWFS(r);
        sConnectBitmap |= bit;
        return;
    case 9:
        if (data_022c197c != NULL)
            data_022c197c(data_02078a8c, r->unk10);
        sConnectBitmap &= ~bit;
        WH_CallbackForWFS(r);
        return;
    case 0:
        if (!WH_StateInStartParentMP())
            WH_ChangeSysState(9);
        return;
    case 2:
        return;
    default:
        if (data_022c197c != NULL)
            data_022c197c(data_02078ab8, r->unk8);
        return;
    }
}
