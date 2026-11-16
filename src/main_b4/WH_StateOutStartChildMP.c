#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
} UnkRes_0203ee8c;

extern s32 data_022c196c;
extern s32 sSysState;
extern u16 data_022c1948;
extern u16 data_022c1950;
extern u8 data_022c2440[];
extern void (*data_022c197c)(const char *fmt, ...);
extern char data_020787d4[];
extern char data_020787fc[];
extern char data_02078830[];

void WH_SetError(s32 r);
void WH_ChangeSysState(s32 a);
void WH_Reset(void);
void WH_Finalize(void);
BOOL WH_StateInStartChildKeyShare(void);
s32 func_02012a58(void *buf, s32 a, u16 mask, u16 b, s32 c);
void WH_CallbackForWFS(UnkRes_0203ee8c *r);

void WH_StateOutStartChildMP(UnkRes_0203ee8c *r) {
    s32 ret;

    if (r->unk2 != 0) {
        if (r->unk2 == 0xf)
            return;
        if (r->unk2 == 9)
            return;
        if (r->unk2 == 0xd)
            return;
        WH_SetError(r->unk2);
        WH_Reset();
        return;
    }
    switch (r->unk4) {
    case 10:
        if (data_022c196c == 3) {
            if (sSysState == 6)
                return;
            if (sSysState == 4) {
                if (WH_StateInStartChildKeyShare())
                    return;
                if (data_022c197c != NULL)
                    data_022c197c(data_020787d4);
                WH_Finalize();
                return;
            }
        } else if (data_022c196c == 5) {
            ret = func_02012a58(data_022c2440, 0xd, (1 << (data_022c1948 + 1)) - 1, data_022c1950, 1);
            if (ret != 0) {
                WH_SetError(ret);
                WH_Finalize();
                return;
            }
            if (data_022c197c != NULL)
                data_022c197c(data_020787fc);
            WH_ChangeSysState(5);
            return;
        } else if (data_022c196c == 7) {
            WH_CallbackForWFS(r);
        }
        WH_ChangeSysState(4);
        return;
    case 12:
    case 13:
        break;
    case 11:
    default:
        if (data_022c197c != NULL)
            data_022c197c(data_02078830, r->unk4);
        break;
    }
}
