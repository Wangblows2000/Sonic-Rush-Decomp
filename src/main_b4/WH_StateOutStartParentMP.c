#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
} UnkRes_0203fdb0;

extern s32 data_022c196c;
extern s32 sSysState;
extern u16 data_022c1948;
extern u16 data_022c1950;
extern u8 data_022c2440[];
extern void (*data_022c197c)(const char *fmt, ...);
extern char data_02078a18[];
extern char data_02078a40[];

void WH_SetError(s32 r);
void WH_ChangeSysState(s32 a);
BOOL WH_StateInStartParentKeyShare(void);
s32 func_02012a58(void *buf, s32 a, u16 mask, u16 b, s32 c);
void WH_CallbackForWFS(UnkRes_0203fdb0 *r);

void WH_StateOutStartParentMP(UnkRes_0203fdb0 *r) {
    s32 ret;

    if (r->unk2 != 0) {
        WH_SetError(r->unk2);
        WH_ChangeSysState(9);
        return;
    }
    switch (r->unk4) {
    case 10:
        if (data_022c196c == 2) {
            if (sSysState == 4) {
                if (WH_StateInStartParentKeyShare())
                    return;
                if (data_022c197c != NULL)
                    data_022c197c(data_02078a18);
                WH_ChangeSysState(9);
                return;
            }
            if (sSysState == 6)
                return;
        } else if (data_022c196c == 4) {
            ret = func_02012a58(data_022c2440, 0xd, (1 << (data_022c1948 + 1)) - 1, data_022c1950, 1);
            if (ret != 0) {
                WH_SetError(ret);
                WH_ChangeSysState(9);
                return;
            }
            WH_ChangeSysState(5);
            return;
        } else if (data_022c196c == 6) {
            WH_CallbackForWFS(r);
        } else if (data_022c196c == 7) {
            WH_CallbackForWFS(r);
        }
        WH_ChangeSysState(4);
        return;
    case 11:
        break;
    case 12:
    case 13:
    default:
        if (data_022c197c != NULL)
            data_022c197c(data_02078a40, r->unk4);
        break;
    }
}
