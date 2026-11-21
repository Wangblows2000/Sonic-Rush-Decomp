#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
    u8 pad4[4];
    u16 unk8;
    u8 mac[6];
    u8 pad10[0x26];
    u16 unk36;
    u8 pad38[4];
    u32 unk3c;
    u8 pad40[3];
    u8 unk43;
} UnkRes_0203f5a0;

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkStruct_022c1a00;

extern s32 sSysState;
extern void (*sTraceFunc)(const char *fmt, ...);
extern void (*data_022c1968)(void *p, UnkRes_0203f5a0 *r);
extern s32 data_022c1974;
extern u16 data_022c1940;
extern UnkStruct_022c1a00 sParentParam;
extern u8 data_022c1ae0[];
extern char data_0207893c[];
extern char data_02078974[];
extern char data_0207898c[];
extern char data_020789a4[];
extern char data_020789b8[];

void WH_SetError(s32 r);
void WH_ChangeSysState(s32 a);
BOOL WH_StateInEndScan(void);
BOOL WH_StateInStartScan(void);
void DC_InvalidateRange(void *p, u32 size);
BOOL CHT_IsPictochatParent(void *p);

void WH_StateOutStartScan(UnkRes_0203f5a0 *r) {
    if (r->unk2 != 0) {
        WH_SetError(r->unk2);
        WH_ChangeSysState(9);
        return;
    }
    if (sSysState != 2) {
        if (!WH_StateInEndScan())
            WH_ChangeSysState(9);
        return;
    }
    if (r->unk8 == 3)
        return;
    if (r->unk8 != 4 && r->unk8 == 5) {
        if (sTraceFunc != NULL)
            sTraceFunc(data_0207893c, r->mac[0], r->mac[1], r->mac[2], r->mac[3], r->mac[4], r->mac[5]);
        DC_InvalidateRange(data_022c1ae0, 0xc0);
        if (data_022c1974 != 0 && CHT_IsPictochatParent(data_022c1ae0)) {
            if (sTraceFunc != NULL)
                sTraceFunc(data_02078974);
            if (data_022c1968 != NULL)
                data_022c1968(data_022c1ae0, r);
        } else if (r->unk36 < 8 || r->unk3c != sParentParam.unk8) {
            if (sTraceFunc != NULL)
                sTraceFunc(data_0207898c);
        } else if ((r->unk43 & 3) != 1) {
            if (sTraceFunc != NULL)
                sTraceFunc(data_020789a4);
        } else {
            if (sTraceFunc != NULL)
                sTraceFunc(data_020789b8);
            if (data_022c1968 != NULL)
                data_022c1968(data_022c1ae0, r);
            if (data_022c1940 != 0) {
                if (!WH_StateInEndScan())
                    WH_ChangeSysState(9);
                return;
            }
        }
    }
    if (!WH_StateInStartScan())
        WH_ChangeSysState(9);
}
